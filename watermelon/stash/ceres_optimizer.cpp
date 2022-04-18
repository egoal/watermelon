#include "ceres_optimizer.h"
#include "ceres/ceres.h"
#include "base_type.h"

namespace opt {

// todo: bad design
namespace g {
static bool disable_info = false;
}
#define DISABLE_INFO_SCOPE(...) \
  {                             \
    g::disable_info = true;     \
    __VA_ARGS__;                \
    g::disable_info = false;    \
  }

InformedCostFunction::InformedCostFunction(ceres::CostFunction* fun, const Eigen::MatrixXd& info)
    : ceres::CostFunction(), fun_(fun), info_(info) {
  set_num_residuals(fun->num_residuals());
  *mutable_parameter_block_sizes() = fun->parameter_block_sizes();
}

bool InformedCostFunction::Evaluate(double const* const* parameters, double* residuals,
                                    double** jacobians) const {
  if (!fun_->Evaluate(parameters, residuals, jacobians)) return false;

  if (g::disable_info) return true;

  //* info mod
  const int n = fun_->num_residuals();
  Eigen::MatrixXd infomed_res = info_ * ceres::MatrixRef(residuals, n, 1);
  ceres::MatrixRef(residuals, n, 1) = infomed_res;

  if (jacobians) {
    const auto& blocks = parameter_block_sizes();
    for (std::size_t i = 0; i < blocks.size(); ++i)
      if (jacobians[i]) {
        int32_t blocksize = blocks[i];
        ceres::MatrixRef(jacobians[i], n, blocksize) =
            info_ * ceres::MatrixRef(jacobians[i], n, blocksize);
      }
  }

  return true;
}

InformedCostFunction* InformedCostFunction::Create(ceres::CostFunction* fun,
                                                   const Eigen::MatrixXd& info) {
  CHECK(info.rows() == info.cols() && info.rows() == fun->num_residuals());
  return new InformedCostFunction(fun, info);
}

Optimizer::~Optimizer() {
  for (auto& a : allocated_) delete[] a;
  for (auto& p : params_) delete p;
  for (auto& f : funs_) delete f;
}

bool Optimizer::Execute(ceres::Solver::Options options) {
  return Build(options) && Solve(options);
}

bool Optimizer::Build(ceres::Solver::Options& options) {
  if (std::any_of(funs_.begin(), funs_.end(), [](Fun* f) { return f->BlockId() == nullptr; })) {
    LOG_INFO << "invalid funs detected, did you forget to set its params?";
    return false;
  }

  {
    std::unordered_set<int> groupids;
    std::transform(funs_.begin(), funs_.end(), std::inserter(groupids, groupids.end()),
                   [](Fun* f) { return f->GroupId(); });

    if (groupids.size() > 1) {
      multigroups_ = true;

      LOG_INFO << ll::unsafe_format(
          "multiple(%d) resudial groups found, residual "
          "log for each group is valid.",
          groupids.size());
    }
  }

  if (multigroups_ && !namefun_) namefun_ = [](int i) { return "cost-" + std::to_string(i); };

  //* setup options
  options.minimizer_progress_to_stdout = true;
  if (logtype_ == LoggingType::PerGroup) {
    options.logging_type = ceres::LoggingType::SILENT;
    if (multigroups_ && (FLAGS_v == 4)) {
      options.update_state_every_iteration = true;
      options.callbacks.emplace_back(this);
    }
  } else if (logtype_ == LoggingType::Ceres) {
  } else if (logtype_ == LoggingType::Silent) {
    options.logging_type = ceres::LoggingType::SILENT;
  }

  {
    std::unordered_set<int> orders;
    std::transform(params_.begin(), params_.end(), std::inserter(orders, orders.end()),
                   [](Param* p) { return p->Order(); });
    if (orders.size() > 1) {
      if (options.linear_solver_ordering)
        LOG_INFO << "should let Optimizer handle the linear_solver_ordering.";

      auto ordering = std::make_shared<ceres::ParameterBlockOrdering>();
      for (auto p : params_) ordering->AddElementToGroup(p->Data(), p->Order());

      options.linear_solver_ordering = ordering;

      LOG_INFO << ll::unsafe_format("enable ordering in elemination: %d.", orders.size());
    }
  }

  return true;
}

bool Optimizer::Solve(const ceres::Solver::Options& options) {
  PreSolve();

  ceres::Solver::Summary summary;
  ceres::Solve(options, &problem_, &summary);

  if (logtype_ != LoggingType::Silent) PostSolve(summary);

  return true;
}

void Optimizer::PreSolve() {
  initialcosts_ = CalcCostOfEachGroups();
  DISABLE_INFO_SCOPE(initialcosts_unweighted_ = CalcCostOfEachGroups());
}

void Optimizer::PostSolve(const ceres::Solver::Summary& summary) {
  auto cost_change_str = [](double init, double final, std::size_t cnt) {
    double change = final * 100. / init;
    change = change > 1000 ? 9999.99 : change;
    return ll::unsafe_format("%8d   %12.2f (%12.4f) -> %12.2f (%12.4f)   %4.2f%%", cnt, init,
                             init / cnt, final, final / cnt, change);
  };

  auto cost_changes_list_str = [&cost_change_str, this](
                                   const std::unordered_map<int, double>& initials,
                                   const std::unordered_map<int, double>& currents) {
    CHECK(initials.size() == currents.size() && !initials.empty());
    std::stringstream ss;
    if (initials.size() > 1)
      for (auto& pr : currents) {
        std::size_t cnt = std::count_if(funs_.begin(), funs_.end(),
                                        [i = pr.first](Fun* f) { return f->GroupId() == i; });
        double init = initials.at(pr.first);
        double fnal = pr.second;

        ss << std::setw(12) << namefun_(pr.first) << ": " << cost_change_str(init, fnal, cnt)
           << "\n";
      }

    auto totalinit = ll::sum_by([](auto&& pr) { return pr.second; }, initials);
    auto totalfinal = ll::sum_by([](auto&& pr) { return pr.second; }, currents);
    ss << std::setw(12) << "[TOTAL]"
       << ": " << cost_change_str(totalinit, totalfinal, funs_.size()) << "\n";
    return ss.str();
  };

  std::stringstream ss;
  ss << "cost changes:\n" << cost_changes_list_str(initialcosts_, CalcCostOfEachGroups());
  if (anyinformed_)
    DISABLE_INFO_SCOPE(
        ss << "error changes: \n"
           << cost_changes_list_str(initialcosts_unweighted_, CalcCostOfEachGroups()));

  LOG_INFO << summary.BriefReport() << "\n" << ss.str();
}

ceres::CallbackReturnType Optimizer::operator()(const ceres::IterationSummary& summary) {
  //! skip computation of each groups if less then 2 groups were set.
  auto groupcosts = multigroups_ ? CalcCostOfEachGroups() : std::unordered_map<int, double>();

  if (summary.iteration == 0) {
    std::stringstream ss;
    ss << "   | ";
    for (auto& pr : groupcosts) ss << std::setw(12) << namefun_(pr.first) << " ";
    ss << "       cost     cost-change(%)    |dx|       mu     time(s)";
    // ss << "\n---+";
    // LL_REPEAT(100) { ss << "-"; }
    std::cout << ss.str() << "\n";
  }

  std::stringstream ss;
  ss << ll::unsafe_format("% 3d| ", summary.iteration);
  for (auto& pr : groupcosts) ss << ll::unsafe_format("%12.2f ", pr.second);
  ss << ll::unsafe_format("  %8e    %5.2f        %3.2e   %3.2e   %.3f", summary.cost,
                          summary.iteration == 0 ? 0. : summary.cost_change * 100. / lastcost_,
                          summary.step_norm, summary.trust_region_radius,
                          summary.cumulative_time_in_seconds);
  std::cout << ss.str() << "\n";

  lastcost_ = summary.cost;

  return ceres::CallbackReturnType::SOLVER_CONTINUE;
}

std::unordered_map<int, double> Optimizer::CalcCostOfEachGroups() const {
  std::unordered_map<int, double> groupcosts;
  for (const auto& f : funs_) {
    double cost{0.};
    problem_.EvaluateResidualBlock(f->BlockId(), false, &cost, nullptr, nullptr);

    groupcosts[f->GroupId()] += cost;
  }

  return groupcosts;
}

}  // namespace opt
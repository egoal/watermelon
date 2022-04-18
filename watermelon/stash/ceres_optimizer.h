#pragma once

#include "ll.hpp"
#include <ceres/ceres.h>

namespace Eigen {
template <typename T, typename Alloc = Eigen::aligned_allocator<T>>
using vector = std::vector<T, Alloc>;
}

namespace opt {

class InformedCostFunction : public ceres::CostFunction {
 public:
  InformedCostFunction(ceres::CostFunction *fun, const Eigen::MatrixXd &info);
  bool Evaluate(double const *const *parameters, double *residuals,
                double **jacobians) const;

  static InformedCostFunction *Create(ceres::CostFunction *fun,
                                      const Eigen::MatrixXd &info);

 private:
  ceres::CostFunction *fun_;
  Eigen::MatrixXd info_;
};

class Param {
 public:
  Param(ceres::Problem *prob, double *data, std::size_t size)
      : prob_(prob), data_(data), size_(size) {}

  Param *SetLocalParameterization(ceres::LocalParameterization *lp) {
    prob_->SetParameterization(data_, lp);
    return this;
  }

  Param *SetOrder(int order) {
    order_ = order;
    return this;
  }

  Param *SetConstant() {
    prob_->SetParameterBlockConstant(data_);
    return this;
  }

  Param *SetValue(const double value) {
    CHECK(1 == size_);
    *data_ = value;
    return this;
  }

  //* handy eigen interface
  Param *SetValue(const Eigen::VectorXd &value) {
    CHECK(static_cast<size_t>(value.rows()) == size_);
    Eigen::Map<Eigen::VectorXd>(data_, size_, 1) = value;
    return this;
  }

  Eigen::VectorXd GetValue() const {
    return Eigen::Map<Eigen::VectorXd>(data_, size_, 1);
  }

  int GetId() const { return id_; }
  Param *SetId(const int id) {
    id_ = id;
    return this;
  }

  double *Data() const { return data_; }
  int Order() const { return order_; }

 private:
  ceres::Problem *prob_;
  double *const data_;
  const std::size_t size_;
  int order_{0};
  int id_;
};

class Fun {
 public:
  Fun(ceres::Problem *prob, ceres::CostFunction *f) : prob_(prob), f_(f) {}

  Fun *SetLossFunction(ceres::LossFunction *lf) {
    CHECK(!bId_);
    lf_ = lf;
    return this;
  }

  template <typename... Ts>
  Fun *SetParams(const Param *x0, Ts... xs) {
    CHECK(!bId_);
    const std::array<const Param *, sizeof...(Ts) + 1> params({x0, xs...});

    bId_ = prob_->AddResidualBlock(
        f_, lf_,
        ll::mapf([](auto &&p) -> double * { return p->Data(); }, params));
    return this;
  }

  Fun *SetGroup(int group) {
    group_ = group;
    return this;
  }

  ceres::ResidualBlockId BlockId() const { return bId_; }
  int GroupId() const { return group_; }

  InformedCostFunction *GetInformedFun() {
    return static_cast<InformedCostFunction *>(f_);
  }

  int GetId() const { return id_; }
  Fun *SetId(const int id) {
    id_ = id;
    return this;
  }

 private:
  ceres::Problem *prob_;
  ceres::CostFunction *f_;
  ceres::LossFunction *lf_{nullptr};
  ceres::ResidualBlockId bId_{nullptr};
  int group_{0};
  int id_ = -1;
};

enum class LoggingType {
  Silent,
  Ceres,
  PerGroup,  //* would log cost of each residual groups in each iteration,
};

class Optimizer : public ceres::IterationCallback {
 public:
  Optimizer() = default;
  LL_BAN_COPY(Optimizer);
  ~Optimizer();

  //* let optimizer handle memory for you
  Param *NewParam(int size) {
    allocated_.emplace_back(new double[size]{0.});
    return NewParam(allocated_.back(), size);
  }
  //* on your own, manage life time for 'data'
  Param *NewParam(double *data, int size) {
    problem_.AddParameterBlock(data, size);
    params_.emplace_back(new Param(&problem_, data, size));
    return params_.back();
  }

  Fun *NewFun(ceres::CostFunction *f) {
    funs_.emplace_back(new Fun(&problem_, f));
    return funs_.back();
  }
  Fun *NewInformedFun(ceres::CostFunction *f, const Eigen::MatrixXd &W) {
    anyinformed_ = true;
    return NewFun(InformedCostFunction::Create(f, W));
  }

  void SetGroupNames(std::function<std::string(int)> namefun) {
    namefun_ = namefun;
  }
  void SetLoggingType(LoggingType logtype) { logtype_ = logtype; }

  bool Execute() {
    //* default options:
    ceres::Solver::Options options;
    options.linear_solver_type =
        ceres::LinearSolverType::SPARSE_NORMAL_CHOLESKY;
    return Execute(options);
  }
  bool Execute(ceres::Solver::Options options);

 private:
  std::vector<double *> allocated_;
  std::vector<Param *> params_;
  std::vector<Fun *> funs_;
  ceres::Problem problem_;
  std::function<std::string(int)> namefun_{nullptr};

  LoggingType logtype_{LoggingType::Silent};
  bool multigroups_{false};
  bool anyinformed_{false};

  double lastcost_{0.};
  std::unordered_map<int, double> initialcosts_;
  std::unordered_map<int, double> initialcosts_unweighted_;

  bool Build(ceres::Solver::Options &options);
  void PreSolve();
  bool Solve(const ceres::Solver::Options &options);
  void PostSolve(const ceres::Solver::Summary &summary);
  ceres::CallbackReturnType operator()(const ceres::IterationSummary &summary);
  std::unordered_map<int, double> CalcCostOfEachGroups() const;
};

}  // namespace opt

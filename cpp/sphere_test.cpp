#include <cmath>

#include <ceres/ceres.h>
#include <gflags/gflags.h>

// test: 
// find a point on the unit sphere which is nearest to a point

DEFINE_bool(on_sphere, false, "enable sphere constraint");
DEFINE_int64(num_iters, 10, "max iterations");

constexpr double pi =   3.14159265;

class CFDistance2: public ceres::SizedCostFunction<1, 3>{
public:
    CFDistance2(double px, double py, double pz): px_(px), py_(py), pz_(pz){}

    bool Evaluate(double const* const* parameters, double* residuals, 
        double** jacobians) const override{
        const double* xyz   =   parameters[0];

        double dx   =   xyz[0]-px_;
        double dy   =   xyz[1]-py_;
        double dz   =   xyz[2]-pz_;
        residuals[0]    =   dx*dx+dy*dy+dz*dz;

        if(jacobians && jacobians[0]){
            jacobians[0][0] =   2*dx;
            jacobians[0][1] =   2*dy;
            jacobians[0][2] =   2*dz;
        }

        return true;
    }

private:
    double px_, py_, pz_;
};

// try use uv coordinate, which leads to a constraint problem
template <typename T>
void xyz2uv(const T* xyz, T* uv, T r){
    uv[0]   =   std::atan2(xyz[1], xyz[0]);
    uv[1]   =   std::asin(xyz[2]/r);    // may use atan2
}

template <typename T>
void uv2xyz(const T* uv, T* xyz, T r){
    xyz[0]  =   r*std::cos(uv[1])*std::cos(uv[0]);
    xyz[1]  =   r*std::cos(uv[1])*std::sin(uv[0]);
    xyz[2]  =   r*std::sin(uv[1]);
}

template <typename T>
std::string to_string(const T* a, int size){
    std::stringstream ss;
    for(int i=0; i<size; ++i) ss<<a[i]<<", ";
    return ss.str();
}
class LPOnSphere: public ceres::LocalParameterization{
public:
    bool Plus(const double* x, const double* dx, double* x_dx) const override{
        std::cout<<"Plus: \n";
        double uv[2];
        xyz2uv(x, uv, r);
        std::cout<<"["<<to_string(x, 3)<<"]("<<to_string(uv, 2)<<") + ";
        uv[0]   +=  dx[0];
        uv[1]   +=  dx[1];
        
        // cut, 
        if(uv[0]<-pi) uv[0] =   -pi;
        else if(uv[0]>=pi) uv[0]    =   pi;
        if(uv[1]<-pi/2) uv[1]   =   -pi/2;
        else if(uv[1]>pi/2) uv[1]   =   pi/2;

        uv2xyz(uv, x_dx, r);

        std::cout<<"("<<to_string(dx, 2)<<")"<<" = ["<<to_string(x_dx, 3)
            <<"]("<<to_string(uv, 2)<<")\n";


        return true;
    }

    bool ComputeJacobian(const double* x, double* j) const override{
        double l    =   std::sqrt(x[0]*x[0]+x[1]*x[1]);
        double sv   =   x[2]/r;
        double cv   =   l/r;
        double su   =   x[1]/l;
        double cu   =   x[0]/l;

        assert(j);
        j[0]    =   -cv*su;
        j[1]    =   -sv*cu;
        j[2]    =   cv*cu;
        j[3]    =   -sv*su;
        j[4]    =   0.;
        j[5]    =   cv;

        return true;
    }

    int GlobalSize() const override{ return 3; }
    int LocalSize() const override{ return 2; }

private:
    static constexpr double r   =   1.;
};

// reproject to sphere, 3->3
class LPToSphere: public ceres::LocalParameterization{
public:
    bool Plus(const double* x, const double* dx, double* x_dx) const override{
        x_dx[0] =   x[0]+dx[0];
        x_dx[1] =   x[1]+dx[1];
        x_dx[2] =   x[2]+dx[2];

        double l    =   std::sqrt(x_dx[0]*x_dx[0]+x_dx[1]*x_dx[1]+x_dx[2]*x_dx[2]);
        x_dx[0] *=  r/l;
        x_dx[1] *=  r/l;
        x_dx[2] *=  r/l;

        return true;
    }

    bool ComputeJacobian(const double* x, double* j) const override{
        j[0]    =   x[1]*x[1]+x[2]*x[2];
        j[1]    =   -x[0]*x[1];
        j[2]    =   -x[0]*x[2];
        j[3]    =   j[1];
        j[4]    =   x[0]*x[0]+x[2]*x[2];
        j[5]    =   -x[1]*x[2];
        j[6]    =   j[2];
        j[7]    =   j[5];
        j[8]    =   x[0]*x[0]+x[1]*x[1];

        for(int i=0; i<9; ++i) j[i] /=  r*r;

        return true;
    }    

    int GlobalSize() const override{ return 3; }
    int LocalSize() const override{ return 3; }

private:
    static constexpr double r   =   1.;    
};

// 
class CBPrint: public ceres::IterationCallback{
public:
    ceres::CallbackReturnType operator()(const ceres::IterationSummary& is) override{
        std::cout<<"called\n";

        return ceres::SOLVER_CONTINUE;
    }
};

int main(int argc, char** argv){
    google::ParseCommandLineFlags(&argc, &argv, true);

    double xyz[3]{0., -1., 0.}, pt[3]{2., 2., 2.};

    ceres::Problem problem;
    problem.AddResidualBlock(new CFDistance2(pt[0], pt[1], pt[2]), nullptr, xyz);

    if(FLAGS_on_sphere){
        auto lp =   new LPToSphere();
        problem.SetParameterization(xyz, lp);
    }

    ceres::Solver::Options options;
    {
        options.minimizer_progress_to_stdout    =   true;
        options.gradient_tolerance  =   1e-32;
        options.function_tolerance  =   1e-32;
        options.max_num_iterations  =   FLAGS_num_iters;
        // options.callbacks.push_back(new CBPrint());
        options.jacobi_scaling  =   false;
        // options.initial_trust_region_radius =   1e-2;
        options.use_nonmonotonic_steps  =   false;
    }
    ceres::Solver::Summary summary;
    ceres::Solve(options, &problem, &summary);
    // std::cout<<summary.FullReport()<<"\n";

    double dis  =   std::sqrt((xyz[0]-pt[0])*(xyz[0]-pt[0])+
        (xyz[1]-pt[1])*(xyz[1]-pt[1])+(xyz[2]-pt[2])*(xyz[2]-pt[2]));
    std::cout<<"\n==========\n("<<to_string(xyz, 3)<<")\t"
        <<"r: "<<std::sqrt(xyz[0]*xyz[0]+xyz[1]*xyz[1]+xyz[2]*xyz[2])<<"\t"
        <<"dis: "<<dis<<"\t"<<(dis-(std::sqrt(12.)-1))<<"\n";

    return 0;
}

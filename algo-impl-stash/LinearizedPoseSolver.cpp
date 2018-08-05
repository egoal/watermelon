#include "LinearizedPoseSolver.h"

#include <g2o/core/base_vertex.h>
#include <g2o/core/base_binary_edge.h>
#include <g2o/core/base_multi_edge.h>
#include <g2o/core/block_solver.h>
#include <g2o/core/linear_solver.h>
#include <g2o/solvers/eigen/linear_solver_eigen.h>
#include <g2o/solvers/pcg/linear_solver_pcg.h>
#include <g2o/core/optimization_algorithm_gauss_newton.h> 

#include "LogHelper.h"

#define LPS_LOG if(!isSilent) LOG_SAM_DEBUG<<"[LPS] "

namespace dm{
    
//* helper
Eigen::Matrix3d projectToSO3(const Eigen::Matrix3d& M){
    Eigen::JacobiSVD<Eigen::Matrix3d > svd(M, Eigen::ComputeFullU|Eigen::ComputeFullV);

    Eigen::Matrix3d U   =   svd.matrixU();
    Eigen::Matrix3d V   =   svd.matrixV();
    Eigen::Matrix3d S   =   Eigen::Matrix3d::Identity();
    S(2, 2) =   (U*V.transpose()).determinant();

    return static_cast<Eigen::Matrix3d >(U*S*V.transpose());
}

Eigen::Matrix3d asSkewSymmetricMatrix(const Eigen::Vector3d& v){
    Eigen::Matrix3d ssm =   Eigen::Matrix3d::Zero();
    ssm(0, 1)   =   -v(2);
    ssm(1, 0)   =   v(2);
    ssm(0, 2)   =   v(1);
    ssm(2, 0)   =   -v(1);
    ssm(1, 2)   =   -v(0);
    ssm(2, 1)   =   v(0);

    return ssm;
}

Eigen::Matrix3d mapToSO3(const Eigen::Vector3d& s){
    //! refer to g2o/se3quat.h
    double theta    =   s.norm();

    Eigen::Matrix3d Omega   =   asSkewSymmetricMatrix(s);

    if(theta<0.00001)
        return Eigen::Matrix3d::Identity()+Omega+Omega*Omega;
    else{
        return Eigen::Matrix3d::Identity()+ sin(theta)/theta*Omega
            +(1-cos(theta))/(theta*theta)*(Omega*Omega);
    }
}

Eigen::Matrix3d rightDistribute(const Eigen::Matrix3d& R, const Eigen::Vector3d& dr){
    return R*mapToSO3(dr);
}

namespace {
    //* hidden

    /* use in simple chordal */
    //* simple 3x3 matrix as R
    class VRelaxedRotation: public g2o::BaseVertex<9, Eigen::Matrix3d>{
    public:
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

        void setToOriginImpl() override{
            _estimate   =   Eigen::Matrix3d::Zero();
        }
        void oplusImpl(const double* update) override{
            // simple add
            Eigen::Matrix3d::ConstMapType dR(update);
            _estimate   +=  dR;
        }

        bool read(std::istream&) override{ return true; }
        bool write(std::ostream&) const override{ return true; }
    };

    //* R1 - R0 R01
    class EFNormRotation: public g2o::BaseBinaryEdge<9, Eigen::Matrix3d, 
        VRelaxedRotation, VRelaxedRotation>{
    public:
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

        void computeError() override{
            auto r0 =   static_cast<const VRelaxedRotation* >(_vertices[0]);
            auto r1 =   static_cast<const VRelaxedRotation* >(_vertices[1]);

            Eigen::Matrix3d r01(_measurement);
            auto dr =   static_cast<Eigen::Matrix3d>(r1->estimate()-r0->estimate()*r01);
            
            //todo: try reshape instead
            for(int i=0; i<9; ++i)
                _error(i, 0)    =   dr(i/3, i%3);

        }
        // void linearizeOplus() override{
        //     Eigen::Matrix3d r01(_measurement);
        //     _jacobianOplusXi    =   Eigen::Matrix<double, 9, 9>::Zero();
        //     _jacobianOplusXi.block<3, 3>(0, 0)  =   -r01.transpose();
        //     _jacobianOplusXi.block<3, 3>(3, 3)  =   -r01.transpose();
        //     _jacobianOplusXi.block<3, 3>(6, 6)  =   -r01.transpose();

        //     _jacobianOplusXj    =   Eigen::Matrix<double, 9, 9>::Identity();
        // }

        bool read(std::istream&) override{ return true; }
        bool write(std::ostream&) const override{ return true; }
    };

    /* use in translation optimization with corrected R */
    //* correction on R | simplt vector t
    //* format: dR in so3 | t in R3
    //? this could split as two vertex
    //todo: choose an elegant name
    using Vector6d  =   Eigen::Matrix<double, 6, 1>;
#define _dR(v6) v6.block<3, 1>(0, 0)
#define _t(v6) v6.block<3, 1>(3, 0)
    class VCorrectedRotationWithTranslation: public g2o::BaseVertex<6, Vector6d >{
    public:
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

        void setToOriginImpl() override{
            _estimate   =   Vector6d::Zero();
        }
        void oplusImpl(const double* update) override{
            //? actually, in gn this called only once, 
            //? in a patch mode, i could simple assign data instead
            Vector6d::ConstMapType dRt(update);
            _estimate   +=  dRt;
        }

        bool read(std::istream&) override{ return true; }
        bool write(std::ostream&) const override{ return true; }
    };

    //* R1.dR1 - R0.dR0 R01
    //* \simeq R1-R0.R01 + R1.s(dR1)-R0.s(dR0).R01
    // measurement as R01
    class EFNormCorrectedRotation: public g2o::BaseBinaryEdge<9, Eigen::Matrix3d, 
        VCorrectedRotationWithTranslation, VCorrectedRotationWithTranslation>{
    public:
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

        EFNormCorrectedRotation(const Eigen::Matrix3d& R0, const Eigen::Matrix3d& R1):
            g2o::BaseBinaryEdge<9, Eigen::Matrix3d, 
            VCorrectedRotationWithTranslation, VCorrectedRotationWithTranslation>(), 
            R0_(R0), R1_(R1){}

        //todo: hard coding to speed up
        void computeError() override{
            auto drt0   =   static_cast<const VCorrectedRotationWithTranslation* >(
                _vertices[0])->estimate();
            auto drt1   =   static_cast<const VCorrectedRotationWithTranslation* >(
                _vertices[1])->estimate();
            Eigen::Matrix3d R01(_measurement);
            Eigen::Matrix3d eR =   R1_-R0_*R01 +
                R1_*asSkewSymmetricMatrix(_dR(drt0)) - 
                R0_*asSkewSymmetricMatrix(_dR(drt1))*R01;

            for(int i=0; i<9; ++i)
                _error(i, 0)    =   eR(i/3, i%3);
        }
        // void linearizeOplus() override{
        //     Eigen::Matrix3d R01(_measurement);

        //     _jacobianOplusXi    =   Eigen::Matrix<double, 9, 6>::Zero();

        //     _jacobianOplusXj    =   Eigen::Matrix<double, 9, 6>::Zero();
        //     _jacobianOplusXj.block<3, 3>(0, 0)  =   asSkewSymmetricMatrix(R1_.row(0));
        //     _jacobianOplusXj.block<3, 3>(0, 3)  =   asSkewSymmetricMatrix(R1_.row(1));
        //     _jacobianOplusXj.block<3, 3>(0, 6)  =   asSkewSymmetricMatrix(R1_.row(2)); 
        // }

        bool read(std::istream&) override{ return true; }
        bool write(std::ostream&) const override{ return true; }

    private:
        Eigen::Matrix3d R0_, R1_;
    };

    //* t1 - t0 - R0.dR0 t01
    //* \simeq t1 - t0 - R0.t01 - R0.s(dR0).t01
    // measurement as t01
    //! if split, change this to BaseMultiEdge
    class ECorrectedTranslation: public g2o::BaseBinaryEdge<3, Eigen::Vector3d, 
        VCorrectedRotationWithTranslation, VCorrectedRotationWithTranslation>{
    public:
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

        ECorrectedTranslation(const Eigen::Matrix3d& R0): 
            g2o::BaseBinaryEdge<3, Eigen::Vector3d, 
            VCorrectedRotationWithTranslation, VCorrectedRotationWithTranslation>(), 
            R0_(R0){}

        void computeError() override{
            auto drt0   =   static_cast<const VCorrectedRotationWithTranslation* >(
                _vertices[0])->estimate();
            auto drt1   =   static_cast<const VCorrectedRotationWithTranslation* >(
                _vertices[1])->estimate();
            Eigen::Vector3d t01(_measurement);

            _error  =   _t(drt1)-_t(drt0) - R0_*t01 - 
                R0_*asSkewSymmetricMatrix(_dR(drt0))*t01;
            
        }
        // void linearizeOplus() override{}

        bool read(std::istream&) override{ return true; }
        bool write(std::ostream&) const override{ return true; }
    public:
        Eigen::Matrix3d R0_;
    };

#undef _dR
#undef _t

    /* used in simple recover t */
    class VTranslation: public g2o::BaseVertex<3, Eigen::Vector3d>{
    public:
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

        void setToOriginImpl() override{
            _estimate   =   Eigen::Vector3d::Zero();
        }
        void oplusImpl(const double* update) override{
            //? actually, in gn this called only once, 
            //? in a patch mode, i could simple assign data instead
            Eigen::Vector3d::ConstMapType dt(update);
            _estimate   +=  dt;
        }

        bool read(std::istream&) override{ return true; }
        bool write(std::ostream&) const override{ return true; }
    };

    //* t1 - t0 - R0 t01
    class ETranslation: public g2o::BaseBinaryEdge<3, Eigen::Vector3d, 
        VTranslation, VTranslation>{
    public:
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

        ETranslation(const Eigen::Matrix3d& R0): 
            g2o::BaseBinaryEdge<3, Eigen::Vector3d, VTranslation, VTranslation>(), 
            R0_(R0){}

        void computeError() override{
            auto t0 =   static_cast<const VTranslation* >(_vertices[0])->estimate();
            auto t1 =   static_cast<const VTranslation* >(_vertices[1])->estimate();
            Eigen::Vector3d t01(_measurement);

            _error  =   t1-t0 - R0_*t01;
        }
        // void linearizedOplus() override{}

        bool read(std::istream&) override{ return true; }
        bool write(std::ostream&) const override{ return true; }

    private:
        Eigen::Matrix3d R0_;
    };
}

////////////////////////////////////////////////////////////////////////////////

#define TID(id) umapID2Index_[id]
#define ID(tid) vecVertices_[tid].id

#define RBLOCK(M) M.block<3, 3>(0, 0)
#define tBLOCK(M) M.block<3, 1>(0, 3)

LinearizedPoseSolver::LinearizedPoseSolver(){}

LinearizedPoseSolver::~LinearizedPoseSolver(){}

void LinearizedPoseSolver::addVertex(IDType id, const TransformType& M, bool active){
    assert(!_contains(id) && "duplicate vertex");

    vecVertices_.push_back(Vertice(id, M, active));
}
void LinearizedPoseSolver::addEstimation(IDType idi, IDType idj, 
    const MeasurementType& Mij, double weightR, double weightt){
    assert(_contains(idi) && _contains(idj) && "isolate edge");

    vecMeasurements_.push_back(Measurement(idi, idj, Mij, weightt, weightR));
}

bool LinearizedPoseSolver::solve(){
    if(!checkInputs()){
        LPS_LOG<<"unsurpported inputs";
        return false;
    }

    if(!initializeEstimation()){
        LPS_LOG<<"initialization failed";
        return false;
    }

    if(!estimateRotation()){
        LPS_LOG<<"rotation estimation failed";
        return false;
    }

#if 0
    if(!estimateTranslation()){
        LPS_LOG<<"translation estimation failed";
        return false;
    }
#else
    if(!estimatePose()){
        LPS_LOG<<"pose estimation failed";
        return false;
    }
#endif

    if(!terminateEstimation()){
        LPS_LOG<<"termination failed";
        return false;
    }

    return true;
}

// gain
const LinearizedPoseSolver::TransformType& LinearizedPoseSolver::gain(IDType id) const{
    assert(_contains(id) && "bad request");

    return std::find_if(vecVertices_.begin(), vecVertices_.end(), 
        [&id](const Vertice& v){ return v.id == id; })->M;
}
LinearizedPoseSolver::TransformType& LinearizedPoseSolver::gain(IDType id){
    assert(_contains(id) && "bad request");

    return std::find_if(vecVertices_.begin(), vecVertices_.end(), 
        [&id](const Vertice& v){ return v.id == id; })->M;
}

// private
bool LinearizedPoseSolver::checkInputs(){
    //todo: check data, check connective 
    return true;
}

bool LinearizedPoseSolver::initializeEstimation(){
    // initialization here
    //* map id to tight ids, which is actually the index
    {
        umapID2Index_.clear();
        for(IDType i=0; i<vecVertices_.size(); ++i)
            umapID2Index_[vecVertices_[i].id]   =   i;
    }

    //* stash pivot
    if(pivotID!=NO_PIVOT){
        LPS_LOG<<"pivot enabled: "<<pivotID;
        pivotM  =   _findVertice(pivotID)->M;
    }

    //* reset pose, used in test
    for(auto& v: vecVertices_){
        if(v.isActive && v.id!=pivotID)
            v.M =   TransformType::Identity();
            // RBLOCK(v.M) =   RotationType::Identity();
    }

    return true;
}

bool LinearizedPoseSolver::estimateRotation(){
    LPS_LOG<<__FUNCTION__<<" begin";
    //* invoke optimizer
    g2o::SparseOptimizer optimizer;
    {
        using BS9x9 =   g2o::BlockSolver<g2o::BlockSolverTraits<9, 9> >;
        auto ptrLS  =   new g2o::LinearSolverEigen<BS9x9::PoseMatrixType>();
        auto ptrBS  =   new BS9x9(ptrLS);
        auto ptrSolver  =   new g2o::OptimizationAlgorithmGaussNewton(ptrBS);
        optimizer.setAlgorithm(ptrSolver);
    }

    //* generate vertices
    for(const auto& v: vecVertices_){
        auto vr =   new VRelaxedRotation();
        vr->setEstimate(RBLOCK(v.M));
        vr->setId(TID(v.id));
        vr->setFixed(!v.isActive);

        optimizer.addVertex(vr);
    }

    //* make connection
    std::cerr<<"connecting graph...\n";
    for(const auto& m: vecMeasurements_){
        auto e  =   new EFNormRotation();
        e->setVertex(0, dynamic_cast<g2o::OptimizableGraph::Vertex* >(
            optimizer.vertex(TID(m.idi))));
        e->setVertex(1, dynamic_cast<g2o::OptimizableGraph::Vertex* >(
            optimizer.vertex(TID(m.idj))));
        e->setMeasurement(RBLOCK(m.Mij));
        e->setInformation(Eigen::Matrix<double, 9, 9>::Identity()*m.wr);

        // e->computeError();
        // std::cerr<<e->chi2()<<" "<<(Eigen::Matrix3d::Identity()-RBLOCK(m.Mij)).squaredNorm()<<"\n";

        optimizer.addEdge(e);
    }

    //* launch
    optimizer.setVerbose(!isSilent);
    optimizer.initializeOptimization();
    optimizer.optimize(2);  // actually, once is enough

    //* install updates
    for(IDType tid=0; tid<vecVertices_.size(); ++tid){
        auto v  =   static_cast<VRelaxedRotation* >(optimizer.vertex(tid));
        RBLOCK(vecVertices_[tid].M) =   projectToSO3(v->estimate());
    }

    return true;
}

bool LinearizedPoseSolver::estimateTranslation(){
    LPS_LOG<<__FUNCTION__<<" begin";
    //* invoke optimizer
    g2o::SparseOptimizer optimizer;
    {
        //todo: try check the specified block size, may speed up
        auto ptrLS  =   new g2o::LinearSolverEigen<g2o::BlockSolverX::PoseMatrixType>();
        // auto ptrLS  =   new g2o::LinearSolverPCG<g2o::BlockSolverX::PoseMatrixType>();
        auto ptrBS  =   new g2o::BlockSolverX(ptrLS);
        auto ptrSolver  =   new g2o::OptimizationAlgorithmGaussNewton(ptrBS);
        optimizer.setAlgorithm(ptrSolver);
    }

    //* generate vertices
    for(const auto& v: vecVertices_){
        auto vt =   new VTranslation();
        vt->setEstimate(tBLOCK(v.M));
        vt->setId(TID(v.id));
        vt->setFixed(!v.isActive);

        optimizer.addVertex(vt);
    }

    //* make connection
    for(const auto& m: vecMeasurements_){
        auto ivi    =   _findVertice(m.idi);
        auto ivj    =   _findVertice(m.idj);

        auto e  =   new ETranslation(RBLOCK(ivi->M));
        e->setVertex(0, dynamic_cast<g2o::OptimizableGraph::Vertex* >(
            optimizer.vertex(TID(m.idi))));
        e->setVertex(1, dynamic_cast<g2o::OptimizableGraph::Vertex* >(
            optimizer.vertex(TID(m.idj))));
        e->setMeasurement(tBLOCK(m.Mij));
        e->setInformation(Eigen::Matrix3d::Identity()*m.wt);

        optimizer.addEdge(e);
    }

    //* launch
    optimizer.setVerbose(true);
    optimizer.initializeOptimization();
    optimizer.optimize(2);  // actually, once is enough

    //* install updates
    for(IDType tid=0; tid<vecVertices_.size(); ++tid){
        auto t  =   static_cast<VTranslation* >(optimizer.vertex(tid))->estimate();

        auto& vertice   =   vecVertices_[tid];
        tBLOCK(vertice.M)   =   t;
    }

    return true;
}

bool LinearizedPoseSolver::estimatePose(){
    LPS_LOG<<__FUNCTION__<<" begin";
    //* invoke optimizer
    g2o::SparseOptimizer optimizer;
    {
        //todo: try check the specified block size, may speed up
        auto ptrLS  =   new g2o::LinearSolverEigen<g2o::BlockSolverX::PoseMatrixType>();
        // auto ptrLS  =   new g2o::LinearSolverPCG<g2o::BlockSolverX::PoseMatrixType>();
        auto ptrBS  =   new g2o::BlockSolverX(ptrLS);
        auto ptrSolver  =   new g2o::OptimizationAlgorithmGaussNewton(ptrBS);
        optimizer.setAlgorithm(ptrSolver);
    }

    //* generate vertices
    for(const auto& v: vecVertices_){
        auto vr =   new VCorrectedRotationWithTranslation();
        // zero dR, t
        Vector6d es =   Vector6d::Zero();
        es.block<3, 1>(3, 0)    =   tBLOCK(v.M);
        vr->setEstimate(es);
        // vr->setToOrigin(); // its ok to use this in gn
        vr->setId(TID(v.id));
        vr->setFixed(!v.isActive);

        optimizer.addVertex(vr);
    }

    //* make connection
    for(const auto& m: vecMeasurements_){
        auto ivi    =   _findVertice(m.idi);
        auto ivj    =   _findVertice(m.idj);

        //todo: as in estimationRotation, this could be tidy with a macro
        // two type of edges
        auto er =   new EFNormCorrectedRotation(RBLOCK(ivi->M), RBLOCK(ivj->M));
        er->setVertex(0, dynamic_cast<g2o::OptimizableGraph::Vertex* >(
            optimizer.vertex(TID(m.idi))));
        er->setVertex(1, dynamic_cast<g2o::OptimizableGraph::Vertex* >(
            optimizer.vertex(TID(m.idj))));
        er->setMeasurement(RBLOCK(m.Mij));
        er->setInformation(Eigen::Matrix<double, 9, 9>::Identity()*m.wr);

        optimizer.addEdge(er);

        auto et =   new ECorrectedTranslation(RBLOCK(ivi->M));
        et->setVertex(0, dynamic_cast<g2o::OptimizableGraph::Vertex* >(
            optimizer.vertex(TID(m.idi))));
        et->setVertex(1, dynamic_cast<g2o::OptimizableGraph::Vertex* >(
            optimizer.vertex(TID(m.idj))));
        et->setMeasurement(tBLOCK(m.Mij));
        et->setInformation(Eigen::Matrix<double, 3, 3>::Identity()*m.wt);

        optimizer.addEdge(et);
    }

    //* launch
    optimizer.setVerbose(true);
    optimizer.initializeOptimization();
    optimizer.optimize(2);  // actually, once is enough

    //* install updates
    // std::ofstream fout("v6.out");
    for(IDType tid=0; tid<vecVertices_.size(); ++tid){
        auto v6 =   static_cast<VCorrectedRotationWithTranslation* >(
            optimizer.vertex(tid))->estimate();

        // for(int r=0; r<6; ++r)
        //     fout<<v6(r, 0)<<" ";
        // fout<<"\n";

        auto& vertice   =   vecVertices_[tid];
        RBLOCK(vertice.M)   =   rightDistribute(RBLOCK(vertice.M), v6.block<3, 1>(0, 0));
        tBLOCK(vertice.M)   =   v6.block<3, 1>(3, 0);
    }

    return true;
}

bool LinearizedPoseSolver::terminateEstimation(){
    // termination here
    //* rigid transform
    if(pivotID!=NO_PIVOT){
        TransformType dM    =   _findVertice(pivotID)->M.inverse()*pivotM;
        for(auto& v: vecVertices_)
            v.M =   v.M*dM;
    }

    //* check so3 
    // std::ofstream fout("so3e.out");
    // for(const auto& v: vecVertices_){
    //     double inve =   (RotationType::Identity()-RBLOCK(v.M)*RBLOCK(v.M).transpose()).norm();
    //     double dete =   RBLOCK(v.M).determinant()-1.;

    //     fout<<inve<<" "<<dete<<"\n";
    // }

    //* check jacobi
    // std::ofstream fout("jaco.out");
    // for(const auto& m: vecMeasurements_){
    //     fout<<m.Mij(0, 3)<<" "<<m.Mij(1, 3)<<" "<<m.Mij(2, 3)<<"\n";
    // }

    return true;
}

// assists

// end
#undef TID
#undef ID

#undef RBLOCK
#undef tBLOCK

}

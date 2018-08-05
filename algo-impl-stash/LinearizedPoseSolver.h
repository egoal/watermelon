#pragma once

#include <Eigen/Dense>

#include <unordered_map>
#include <algorithm>

namespace dm{

/**
 * @brief 
 * ! all transforms here is RC
 * * do not require tight IDs
 * ! not supported Scale now
 * ! float number type: double
 */

class LinearizedPoseSolver{
public:
    // alias
    using MeasurementType   =   Eigen::Matrix4d;
    using TransformType =   Eigen::Matrix4d;
    using RotationType  =   Eigen::Matrix3d;
    using IDType    =   std::uint32_t;

    static constexpr IDType NO_PIVOT   =   -1;

    LinearizedPoseSolver();
    // ban copy
    LinearizedPoseSolver(const LinearizedPoseSolver& )  =   delete;
    LinearizedPoseSolver& operator=(const LinearizedPoseSolver& )   =   delete;
    ~LinearizedPoseSolver();

    // prepare
    void addVertex(IDType id, const TransformType& M, bool active=true);
    void addEstimation(IDType idi, IDType idj, const MeasurementType& Mij, 
        double weightR=1., double weightt=1.);

    // configurations
    void keepSilent(bool silent){ isSilent=silent; }
    /**
     * when all vertices is active, the pivot hold the information of absolute coordinates, 
     * according to which the graph is rearranged after optimization
     * ! you may not use this if any of vertice is fixed
     */
    void setPivot(IDType id){
        assert(_contains(id) && "undefined pivot");
        pivotID =   id;
    }

    // do
    bool solve();

    // gain
    const TransformType& gain(IDType id) const;
    TransformType& gain(IDType id);

private:
    struct Vertice{
        Vertice(IDType _id, TransformType _M, bool active):
            id(_id), M(_M), isActive(active){}

        IDType id;
        TransformType M;
        bool isActive;
    };
    struct Measurement{
        Measurement(IDType i, IDType j, MeasurementType mij, 
            double _wt=1., double _wr=1., double _ws=1.):
            idi(i), idj(j), Mij(mij), wt(_wt), wr(_wr), ws(_ws) {}

        IDType idi, idj;
        MeasurementType Mij;
        double wt, wr, ws;  // weight
    };

    // core data
    //todo: may use set instead
    std::vector<Vertice > vecVertices_;
    std::vector<Measurement > vecMeasurements_; // allow duplicate id

    std::unordered_map<IDType, IDType > umapID2Index_;  // id -> tighted id(index)

    // process
    bool checkInputs();
    bool initializeEstimation();
    bool estimateRotation();
    bool estimateTranslation();
    bool estimatePose();
    bool terminateEstimation();

    // assists
    inline bool _contains(IDType id) const{
        return vecVertices_.end() != _findVertice(id);
    }
    inline decltype(vecVertices_.cbegin()) _findVertice(IDType id) const{
        return std::find_if(vecVertices_.cbegin(), vecVertices_.cend(), 
            [&id](const Vertice& v){ return v.id == id; });
    }
    
    // configurations
    bool isSilent{false};
    IDType pivotID{NO_PIVOT};
    TransformType pivotM;
};

}


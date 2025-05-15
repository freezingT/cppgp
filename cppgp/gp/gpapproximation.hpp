#pragma once

#include <memory>

#include <Eigen/Eigen>

#include <cppgp/kernels/gpkernel.hpp>

namespace gp {

class GPApproximation {
public:
    GPApproximation(){};
    virtual void getParameters(Eigen::VectorXd& params) const=0;
    virtual void setParameters(const Eigen::VectorXd& params)=0;
    virtual size_t nParameters() const=0;
    void updateKernelPrecomputations(const std::shared_ptr<kernel::GPKernel>& kernel, const Eigen::MatrixXd& obsX, const Eigen::MatrixXd& obsYnormalized);

    void KinvScalarProduct(Eigen::VectorXd& XTKinvX, const Eigen::MatrixXd& Xin) const;
    void alphaProduct(Eigen::MatrixXd& prod, const Eigen::MatrixXd& lfactor) const;

    virtual double computeNegativeLogMarginalLikelihood(const Eigen::MatrixXd& obsYnormalized)=0;

    bool setNoise(const double noise);
    double getNoise() const;
    void setNoiseFixed(bool isNoiseFixed);
    bool isNoiseFixed() const;
    double getLogDetK();
protected:
    Eigen::LLT<Eigen::MatrixXd> llt;
    Eigen::MatrixXd K;    // [nData x nData]
    double logDetK;
    Eigen::VectorXd innerProducts; // [nY]
    Eigen::MatrixXd alpha; // [nData x nY]
    bool isInverseKComputed() const;
private:
    double noise;
    bool fixedNoise;
    bool isInverseK; // true, if the inverse of K has been successfully computed, else false

    virtual void updateK(const std::shared_ptr<kernel::GPKernel>& kernel, const Eigen::MatrixXd& obsX)=0;
    virtual void updateAD(const Eigen::MatrixXd& obsYNormalized)=0;
    virtual void updateAlpha(const Eigen::MatrixXd& obsYNormalized)=0;

    void computeInverse();
    void computeLogDetK();
};

} // namespace GP

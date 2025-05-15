#pragma once

#include <memory>

#include <Eigen/Eigen>

#include <cppgp/kernels/gpkernel.hpp>
#include <cppgp/gp/gpapproximation.hpp>

namespace gp {


class GaussianProcess {
public:
    GaussianProcess();
    GaussianProcess(const GaussianProcess & m);
    GaussianProcess(const Eigen::MatrixXd& X, const Eigen::MatrixXd& y);
    GaussianProcess(const kernel::GPKernel& kernel, const Eigen::MatrixXd& X, const Eigen::MatrixXd& y);
    GaussianProcess(const kernel::GPKernel& kernel);
    virtual ~GaussianProcess();
    virtual GaussianProcess* copy() const {return(new GaussianProcess(*this));};

    // Getter/Setter
    void setObservation(const Eigen::MatrixXd& X, const Eigen::MatrixXd& y);
    void getObservation(Eigen::MatrixXd& X, Eigen::MatrixXd& y) const;
    virtual void setParameters(const Eigen::VectorXd& params);
    virtual void getParameters(Eigen::VectorXd& params) const;
    virtual size_t nParameters() const;

    void setKernel(const kernel::GPKernel& kernel);
    const std::shared_ptr<const kernel::GPKernel> getKernel();

    // evaluate and optimize
    void posteriorMeanVar(Eigen::MatrixXd& mu, Eigen::MatrixXd& varSigma, const Eigen::MatrixXd& Xin) const;
    void posteriorMean(Eigen::MatrixXd& mu, const Eigen::MatrixXd& Xin) const;
    double computeNegativeLogMarginalLikelihood();

private:
    void computeNormalizedObservation();
    void updateKernelPrecomputations();

    void rescaleMuInplace(Eigen::MatrixXd& mu) const;
    void rescaleMuSigmaInplace(Eigen::MatrixXd& mu, Eigen::MatrixXd& sigma) const;
protected:

    // store observation data
    Eigen::MatrixXd obsX;  // [nData x nX]
    Eigen::MatrixXd obsY;  // [nData x nY]

    std::shared_ptr<kernel::GPKernel> kernel;
    std::shared_ptr<GPApproximation> approx;

    // normalize input observation
    Eigen::RowVectorXd bias;  // [nY]
    Eigen::RowVectorXd scale; // [nY]
    Eigen::MatrixXd obsYnormalized; // [nData x nY]

}; // class GaussianProcess



} // namespace GP

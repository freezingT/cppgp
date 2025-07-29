#pragma once

#include <memory>

#include <Eigen/Eigen>

#include <cppgp/util/prototype.hpp>
#include <cppgp/gp/gpdata.hpp>
#include <cppgp/kernels/gpkernel.hpp>

namespace gp {


class GaussianProcess : public util::Prototype {
public:
    GaussianProcess();
    GaussianProcess(const GaussianProcess & m);
    GaussianProcess(const std::shared_ptr<GPData>& data);
    GaussianProcess(const std::shared_ptr<kernel::GPKernel>& kernel);
    GaussianProcess(const std::shared_ptr<kernel::CovarianceFunction>& covfun);

    GaussianProcess(const std::shared_ptr<GPData>& data, const std::shared_ptr<kernel::GPKernel>& kernel);
    GaussianProcess(const std::shared_ptr<GPData>& data, const std::shared_ptr<kernel::CovarianceFunction>& covfun);

    virtual ~GaussianProcess();

    virtual std::shared_ptr<util::Prototype> copy() const override;// {return(new GaussianProcess(*this));};

    // Getter/Setter
    void setObservation(const std::shared_ptr<GPData>& obsData);
    std::shared_ptr<GPData> getObservation() const;

    void setKernel(const std::shared_ptr<kernel::GPKernel>& kernel);
    const std::shared_ptr<const kernel::GPKernel> getKernel();

    virtual void setParameters(const Eigen::VectorXd& params);
    virtual void getParameters(Eigen::VectorXd& params) const;
    virtual size_t nParameters() const;


    // evaluate and optimize
    void posteriorMeanVar(Eigen::MatrixXd& mu, Eigen::MatrixXd& varSigma, const Eigen::MatrixXd& Xin) const;
    void posteriorMean(Eigen::MatrixXd& mu, const Eigen::MatrixXd& Xin) const;
    double computeNegativeLogMarginalLikelihood();

protected:
    class GaussianProcess_Impl;
    std::unique_ptr<GaussianProcess_Impl> _gp_impl;


}; // class GaussianProcess



} // namespace GP

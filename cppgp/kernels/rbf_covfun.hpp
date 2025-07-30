#pragma once

#include <cppgp/kernels/covfun.hpp>

namespace gp::kernel
{

class RBFCovFun : public CovarianceFunction {
public:
    RBFCovFun();
    RBFCovFun(const double inverseWidth, const double variance);
    RBFCovFun(const Eigen::VectorXd& params);
    virtual ~RBFCovFun();
    virtual std::shared_ptr<CovarianceFunction> copy() const override;

protected:
    virtual void covariancefunction(Eigen::MatrixXd& K, const Eigen::MatrixXd& X1, const Eigen::MatrixXd& X2) const override;
    virtual void covariancefunction(Eigen::MatrixXd& K, const Eigen::MatrixXd& X) const override;
    virtual void covariancefunctionDiag(Eigen::VectorXd& K, const Eigen::MatrixXd& X) const override;
private:
    void kerncompute(Eigen::MatrixXd& k, const Eigen::MatrixXd& n2) const;
};

} // namespace gp::kernel

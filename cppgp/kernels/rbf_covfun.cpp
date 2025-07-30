#include "rbf_covfun.hpp"

#include <cppgp/math/distance.hpp>

gp::kernel::RBFCovFun::RBFCovFun()
{
    this->parameters = Eigen::VectorXd(2);
    this->parameters(0) = 1.0;
    this->parameters(1) = 1.0;
}

gp::kernel::RBFCovFun::RBFCovFun(const double inverseWidth, const double variance)
{
    this->parameters = Eigen::VectorXd(2);
    this->parameters(0) = inverseWidth;
    this->parameters(1) = variance;
}

gp::kernel::RBFCovFun::RBFCovFun(const Eigen::VectorXd &params)
{
    assert(params.size() == 2);
    this->parameters = params;
}

gp::kernel::RBFCovFun::~RBFCovFun()
{}

std::shared_ptr<gp::kernel::CovarianceFunction> gp::kernel::RBFCovFun::copy() const
{
    CovarianceFunction* cfun = new RBFCovFun(this->getParameters());
    return std::shared_ptr<CovarianceFunction>(cfun);
}

void gp::kernel::RBFCovFun::covariancefunction(Eigen::MatrixXd &K, const Eigen::MatrixXd &X1, const Eigen::MatrixXd &X2) const
{
    Eigen::MatrixXd n2;
    math::dist2(n2, X1, X2);
    //math::dist2(n2, (X1.array().rowwise()*this->inputScales.array()).matrix(),
    //      (X2.array().rowwise()*this->inputScales.array()).matrix());
    this->kerncompute(K, n2);
}

void gp::kernel::RBFCovFun::covariancefunction(Eigen::MatrixXd &K, const Eigen::MatrixXd &X) const
{
    //const Eigen::MatrixXd xsc = (X.array().rowwise()*this->inputScales.array()).matrix();
    Eigen::MatrixXd dist;
    math::dist2(dist, X, X);
    //math::dist2(dist, xsc, xsc);
    this->kerncompute(K, dist);
}

void gp::kernel::RBFCovFun::covariancefunctionDiag(Eigen::VectorXd& K, const Eigen::MatrixXd& X) const
{
    const double variance = this->parameters(1);
    K = Eigen::VectorXd::Ones(X.rows())*variance;
}

void gp::kernel::RBFCovFun::kerncompute(Eigen::MatrixXd& k, const Eigen::MatrixXd& n2) const {
    const double inverseWidth = this->parameters(0);
    const double variance = this->parameters(1);

    const double wi2 = (0.5 * inverseWidth);
    k = variance * (-n2*wi2).array().exp();
}

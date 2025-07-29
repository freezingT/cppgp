#include <cppgp/kernels/covfun.hpp>
#include "covfun.hpp"


gp::kernel::CovarianceFunction::CovarianceFunction():
    parameters(0)
{}

gp::kernel::CovarianceFunction::CovarianceFunction(const Eigen::VectorXd &params):
    parameters(params)
{}

gp::kernel::CovarianceFunction::~CovarianceFunction()
{}

void gp::kernel::CovarianceFunction::K(Eigen::MatrixXd &K, const Eigen::MatrixXd &X)
{
    this->covariancefunction(K, X, X);
}

void gp::kernel::CovarianceFunction::K(Eigen::MatrixXd &K, const Eigen::MatrixXd &X1, const Eigen::MatrixXd &X2)
{
    this->covariancefunction(K, X1, X2);
}

//void gp::kernel::CovarianceFunction::diagK(Eigen::VectorXd &K, const Eigen::MatrixXd &X)
//{
//
//}

void gp::kernel::CovarianceFunction::dK_dP()
{
    //TODO
}

void gp::kernel::CovarianceFunction::dK_dX()
{
    //TODO
}

void gp::kernel::CovarianceFunction::ddK_dXdX()
{
    //TODO
}

void gp::kernel::CovarianceFunction::dddK_dXdXdP()
{
    //TODO
}

void gp::kernel::CovarianceFunction::setParameters(const Eigen::VectorXd& params, const unsigned int index)
{
    const unsigned int n = this->nParameters();
    this->parameters = params(Eigen::seq(index, index+n-1));
}

void gp::kernel::CovarianceFunction::getParameters(Eigen::VectorXd& params, const unsigned int index) const
{
    const unsigned int n = this->nParameters();
    if(index == 0 && params.size() == 0){
        params = Eigen::VectorXd(n);
        params = this->parameters;
    }
    else {
        params(Eigen::seq(index, index+n-1)) = this->parameters;
    }
}

unsigned int gp::kernel::CovarianceFunction::nParameters() const
{
    return this->parameters.size();
}

Eigen::VectorXd gp::kernel::CovarianceFunction::getParameters() const
{
    return this->parameters;
}

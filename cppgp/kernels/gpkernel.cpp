/*
* This file is part of the cppgp package.
* Copyright (c) 2025 kaltertee
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the 'Software'), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
*/

#include <cppgp/kernels/gpkernel.hpp>


gp::kernel::GPKernel::GPKernel(const std::shared_ptr<gp::kernel::CovarianceFunction> &covfun):
    covfun(covfun), data(nullptr)
{}


gp::kernel::GPKernel::GPKernel(const std::shared_ptr<gp::kernel::CovarianceFunction> covfun, const double noise):
    covfun(covfun), noise(noise)
{}


gp::kernel::GPKernel::GPKernel(const GPKernel &gpkernel):
    covfun(gpkernel.covfun->copy()), data(gpkernel.data) //copy ref to data without copy
{}


gp::kernel::GPKernel::~GPKernel()
{}


std::shared_ptr<util::Prototype> gp::kernel::GPKernel::copy() const
{
    return std::make_shared<gp::kernel::GPKernel>(*this);
}


void gp::kernel::GPKernel::registerData(const std::shared_ptr<gp::GPData> gpdata)
{
    if(this->data != nullptr){
        this->data->unsubscribe(this);
    }
    this->data = gpdata;
    this->data->subscribe(this, std::bind(&gp::kernel::GPKernel::changedData_trigger, this));
}


void gp::kernel::GPKernel::computeCov(Eigen::MatrixXd& K) const
{
    Eigen::MatrixXd X;
    data->getX(X);
    covfun->K(K, X);
}


void gp::kernel::GPKernel::computeNoisedCov(Eigen::MatrixXd &K) const
{
    Eigen::MatrixXd X;
    data->getX(X);
    int n = X.rows();
    covfun->K(K, X);
    K = K + this->noise*Eigen::MatrixXd(n, n);
}


void gp::kernel::GPKernel::computeCrossCov(Eigen::MatrixXd& K, const Eigen::MatrixXd& X2) const
{
    Eigen::MatrixXd X1;
    data->getX(X1);
    covfun->K(K, X1, X2);
}


void gp::kernel::GPKernel::computeCovDiag(Eigen::VectorXd& K) const
{
    Eigen::MatrixXd X;
    data->getX(X);
    this->covfun->diagK(K, X);
}


void gp::kernel::GPKernel::getAlpha(Eigen::VectorXd& alpha) const
{
    alpha = this->alpha;
}


/*
void gp::kernel::GPKernel::getNoisedInvCov(Eigen::MatrixXd& ICov) const
{
    this->computeNoisedCov(ICov, );
}
*/


void gp::kernel::GPKernel::getNoisedInvCov(Eigen::MatrixXd& ICov, const Eigen::MatrixXd &B) const
{
    ICov = this->noisedCovDecomp.solve(B);
}


double gp::kernel::GPKernel::computeNoisedLogDetCov() const
{
    return this->noisedCovDecomp.vectorD().array().log().sum();
}


void gp::kernel::GPKernel::setParameters(const Eigen::VectorXd& params, const unsigned int index)
{
    this->noise = params(index);
    this->covfun->setParameters(params, index+1);
    changedData_trigger();
}


void gp::kernel::GPKernel::getParameters(Eigen::VectorXd& params, const unsigned int index) const
{
    params(index) = this->noise;
    this->covfun->getParameters(params, index+1);
}


unsigned int gp::kernel::GPKernel::nParameters() const
{
    return 1 + this->covfun->nParameters();
}


std::shared_ptr<gp::GPData> gp::kernel::GPKernel::getData() const
{
    return this->data;
}


int gp::kernel::GPKernel::getN() const
{
    return this->data->getN();
}


void gp::kernel::GPKernel::precomputeAlpha()
{
    auto data = this->data->getNormalizedData();
    Eigen::MatrixXd K;
    this->computeNoisedCov(K);
    noisedCovDecomp = K.ldlt();
    alpha = noisedCovDecomp.solve(std::get<1>(data));
}


void gp::kernel::GPKernel::changedData_trigger()
{
    this->precomputeAlpha();
}

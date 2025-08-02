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
    covfun(covfun), data(nullptr), is_alpha_computed(false)
{}


gp::kernel::GPKernel::GPKernel(const std::shared_ptr<gp::kernel::CovarianceFunction> covfun, const double noise):
    covfun(covfun), noise(noise), is_alpha_computed(false)
{}


gp::kernel::GPKernel::GPKernel(const GPKernel &gpkernel):
    covfun(std::dynamic_pointer_cast<CovarianceFunction>(gpkernel.covfun->copy())),
    data(gpkernel.data),
    is_alpha_computed(false)
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
    if(gpdata != nullptr){
        this->data->subscribe(this, std::bind(&gp::kernel::GPKernel::changedData_trigger, this));
        changedData_trigger();
    }
    else {
        is_alpha_computed = false;
    }
}


void gp::kernel::GPKernel::computeCov(Eigen::MatrixXd& K) const
{
    if(this->data == nullptr){
        K = Eigen::MatrixXd(0, 0);
        return;
    }
    Eigen::MatrixXd X;
    data->getX(X);
    covfun->K(K, X);
}


void gp::kernel::GPKernel::computeNoisedCov(Eigen::MatrixXd &K) const
{
    if(this->data == nullptr){
        K = Eigen::MatrixXd(0, 0);
        return;
    }
    Eigen::MatrixXd X;
    data->getX(X);
    int n = X.rows();
    covfun->K(K, X);
    K = K + this->noise*Eigen::MatrixXd::Identity(n, n);
}


void gp::kernel::GPKernel::computeCrossCov(Eigen::MatrixXd& K, const Eigen::MatrixXd& X2) const
{
    if(this->data == nullptr){
        K = Eigen::MatrixXd(0, 0);
        return;
    }
    Eigen::MatrixXd X1;
    data->getX(X1);
    covfun->K(K, X1, X2);
}


void gp::kernel::GPKernel::computeCovDiag(Eigen::VectorXd& K) const
{
    if(this->data == nullptr){
        K = Eigen::VectorXd(0);
        return;
    }
    Eigen::MatrixXd X;
    data->getX(X);
    this->covfun->diagK(K, X);
}

void gp::kernel::GPKernel::testing()
{

}

void gp::kernel::GPKernel::getAlpha(Eigen::MatrixXd& alpha) const
{
    if(!is_alpha_computed){
        precomputeAlpha();
    }
    alpha = this->alpha;
}


void gp::kernel::GPKernel::setNoise(const double noise)
{
    if(noise < 0){
        this->noise =  -noise;
    }
    else {
        this->noise =  noise;
    }
}


double gp::kernel::GPKernel::getNoise() const
{
    return this->noise;
}


/*
void gp::kernel::GPKernel::getNoisedInvCov(Eigen::MatrixXd& ICov) const
{
    this->computeNoisedCov(ICov, );
}
*/


void gp::kernel::GPKernel::getNoisedInvCov(Eigen::MatrixXd& ICov, const Eigen::MatrixXd &B) const
{
    if(!is_alpha_computed){
        precomputeAlpha();
    }
    ICov = this->noisedCovDecomp.solve(B);
}


double gp::kernel::GPKernel::computeNoisedLogDetCov() const
{
    if(!is_alpha_computed){
        precomputeAlpha();
    }
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

std::shared_ptr<gp::kernel::CovarianceFunction> gp::kernel::GPKernel::getCovarianceFunction() const
{
    return this->covfun;
}

int gp::kernel::GPKernel::getN() const
{
    if(this->data == nullptr){
        return 0;
    }
    return this->data->getN();
}


void gp::kernel::GPKernel::precomputeAlpha() const
{
    if(this->data == nullptr){
        return;
    }
    auto data = this->data->getNormalizedData();
    Eigen::MatrixXd K;
    this->computeNoisedCov(K);
    noisedCovDecomp = K.ldlt();
    auto tmp = std::get<1>(data);
    alpha = noisedCovDecomp.solve(tmp);
    is_alpha_computed = true;
}


void gp::kernel::GPKernel::changedData_trigger()
{
    if(this->data != nullptr)
    {
        this->precomputeAlpha();
    }
}

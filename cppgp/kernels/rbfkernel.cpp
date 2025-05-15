
#include <cppgp/kernels/rbfkernel.hpp>
#include <cppgp/util/exceptions.hpp>
#include <cppgp/math/distance.hpp>

using namespace gp::kernel;


RBFKernel::RBFKernel(): GPKernel(), variance(0), inverseWidth(0), inputScales(Eigen::VectorXd(0))
{}

RBFKernel::RBFKernel(const Eigen::VectorXd& params) : RBFKernel(){
    if(params.size() == 0) {
        // do nothing
    }
    else if(params.size() >= 2) {
        this->variance = params(0);
        this->inverseWidth = params(1);
        this->inputScales = Eigen::VectorXd(params.size()-2);
        for(int i = 2; i < params.size(); ++i){
            this->inputScales(i-2) = params(i);
        }
    }
    else {
        util::exceptions::throwException<util::exceptions::InconsistentInputError>("");
    }
}

RBFKernel::RBFKernel(const RBFKernel& m): GPKernel(m), variance(m.variance), inverseWidth(m.inverseWidth), inputScales(m.inputScales)
{}

RBFKernel::~RBFKernel()
{}

std::shared_ptr<GPKernel> RBFKernel::copy() const{
    return std::shared_ptr<GPKernel>(new RBFKernel(*this));
}

void RBFKernel::computeCov(Eigen::MatrixXd& K, const Eigen::MatrixXd& X) const {
    const Eigen::MatrixXd xsc = (X.array().rowwise()*this->inputScales.array()).matrix();
    Eigen::MatrixXd dist;
    math::dist2(dist, xsc, xsc);
    this->kerncompute(K, dist);
}

void RBFKernel::computeCrossCov(Eigen::MatrixXd& K, const Eigen::MatrixXd& X1, const Eigen::MatrixXd& X2) const {
    Eigen::MatrixXd n2;
    math::dist2(n2, (X1.array().rowwise()*this->inputScales.array()).matrix(),
          (X2.array().rowwise()*this->inputScales.array()).matrix());
    this->kerncompute(K, n2);
}

void RBFKernel::computeCovDiag(Eigen::VectorXd& diagK, const Eigen::MatrixXd& X) const {
    diagK = this->variance * Eigen::MatrixXd::Ones(X.rows(), 1);
}


// Getter/Setter
void RBFKernel::setParameters(const Eigen::VectorXd& params, const int index) {
    _setParameters(params); //TODO consider the second input parameter
}

void RBFKernel::getParameters(Eigen::VectorXd& params) const {
    params = Eigen::VectorXd(this->inputScales.size()+2);
    params(0) = variance;
    params(1) = inverseWidth;
    params(Eigen::seq(2, Eigen::last)) = inputScales;
}

void RBFKernel::getParameters(Eigen::VectorXd& params, const int index) const {
    params(index+0) = variance;
    params(index+1) = inverseWidth;
    params(Eigen::seq(index+2, index+2+inputScales.size())) = inputScales;
}

void RBFKernel::_setParameters(const Eigen::VectorXd& params){
    variance = params(0);
    inverseWidth = params(1);
    inputScales = params(Eigen::seq(2, Eigen::last));
}

void RBFKernel::kerncompute(Eigen::MatrixXd& k, const Eigen::MatrixXd& n2) const {
    const double wi2 = (0.5 * this->inverseWidth);
    k = this->variance * (-n2*wi2).array().exp();
}

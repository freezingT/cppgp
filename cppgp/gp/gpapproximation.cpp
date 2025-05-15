#include <cppgp/gp/gpapproximation.hpp>
#include <cppgp/util/exceptions.hpp>

using namespace gp;

void GPApproximation::computeInverse(){
    int maxTries = (this->fixedNoise) ? 1 : 20;

    Eigen::VectorXd diag;
    bool success = false;
    this->isInverseK = true;
    for(int i = 0; i < maxTries; ++i){
        auto K2 = this->K;
        if(i > 0){
            this->noise *= 10;
        }
        diag = this->noise * Eigen::VectorXd::Ones(this->K.rows());
        K2 += diag.asDiagonal().toDenseMatrix();

        this->llt.compute(K2);
        if(llt.info() == Eigen::Success){
            success = true;
            break;
        }
    }
    if(!success){
        util::exceptions::throwException<util::exceptions::Error>("Failed to invert the kernel matrix.");
    }
    this->isInverseK = true;
}

void GPApproximation::computeLogDetK(){
    if(!this->isInverseK){
        this->computeInverse();
    }
    this->logDetK = 2*this->llt.matrixL().determinant();
}

void GPApproximation::updateKernelPrecomputations(const std::shared_ptr<kernel::GPKernel>& kernel, const Eigen::MatrixXd& obsX, const Eigen::MatrixXd& obsYnormalized) {
    this->updateK(kernel, obsX);
    this->computeInverse();
    this->computeLogDetK();

    this->updateAD(obsYnormalized);
    this->updateAlpha(obsYnormalized);
}


void GPApproximation::alphaProduct(Eigen::MatrixXd& prod, const Eigen::MatrixXd& lfactor) const {
    prod = lfactor*this->alpha;
}


void GPApproximation::KinvScalarProduct(Eigen::VectorXd& XTKinvX, const Eigen::MatrixXd& X) const {
    XTKinvX = (X.array()*(this->llt.solve(X).array())).colwise().sum();
}

bool GPApproximation::isInverseKComputed() const {
    return this->isInverseK;
}

bool GPApproximation::setNoise(const double noise) {
    this->noise = std::abs(noise);
}

double GPApproximation::getNoise() const {
    return this->noise;
}

void GPApproximation::setNoiseFixed(bool isNoiseFixed) {
    this->fixedNoise = isNoiseFixed;
}

bool GPApproximation::isNoiseFixed() const {
    return this->fixedNoise;
}

double GPApproximation::getLogDetK() {
    return this->logDetK;
}

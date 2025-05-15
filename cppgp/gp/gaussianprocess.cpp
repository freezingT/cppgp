
#include <cppgp/gp/gaussianprocess.hpp>
#include <cppgp/util/exceptions.hpp>

using namespace gp;

const Eigen::VectorXd nullvector = Eigen::VectorXd(0);
const Eigen::MatrixXd nullmatrix = Eigen::MatrixXd(0, 0);

inline bool isNullmatrix(const Eigen::MatrixXd& M){
    return M.size() == 0;
}

GaussianProcess::GaussianProcess() : obsX(nullmatrix),obsY(nullmatrix) {
    kernel = std::shared_ptr<kernel::GPKernel>(nullptr);
}

GaussianProcess::GaussianProcess(const GaussianProcess & m) : GaussianProcess(){

}

GaussianProcess::GaussianProcess(const Eigen::MatrixXd& X, const Eigen::MatrixXd& y) {
    this->setObservation(X, y);
}

GaussianProcess::GaussianProcess(const kernel::GPKernel& kernel, const Eigen::MatrixXd& X, const Eigen::MatrixXd& y){
    this->setObservation(X, y);
    this->setKernel(kernel);
}

GaussianProcess::GaussianProcess(const kernel::GPKernel& kernel) : GaussianProcess(){
    this->setKernel(kernel);
}

GaussianProcess::~GaussianProcess()
{}

void GaussianProcess::setObservation(const Eigen::MatrixXd& X, const Eigen::MatrixXd& y){
    if(X.rows() != y.rows()){
        util::exceptions::throwException<util::exceptions::InconsistentInputError>("The number of samples is not consistent.");
    }
    this->obsX = X;
    this->obsY = y;
    this->computeNormalizedObservation();
    this->updateKernelPrecomputations();
}

void GaussianProcess::getObservation(Eigen::MatrixXd& X, Eigen::MatrixXd& y) const {
    X = this->obsX;
    y = this->obsY;
}


void GaussianProcess::setKernel(const kernel::GPKernel& kernel){
    this->kernel = std::shared_ptr<kernel::GPKernel>(kernel.copy());
    this->updateKernelPrecomputations();
}

const std::shared_ptr<const kernel::GPKernel> GaussianProcess::getKernel(){
    return this->kernel;
}

void GaussianProcess::computeNormalizedObservation() {
    this->bias = this->obsY.colwise().mean();
    this->scale = Eigen::VectorXd::Ones(this->obsY.cols());
    this->obsYnormalized = (this->obsY - this->bias).array().rowwise()/this->scale.array();
}


void GaussianProcess::setParameters(const Eigen::VectorXd& params) {
    auto nk = this->kernel->nParameters();
    auto na = this->approx->nParameters();
    this->kernel->setParameters(params(Eigen::seq(0, nk)));
    this->approx->setParameters(params(Eigen::seq(nk, na)));
    this->updateKernelPrecomputations();
}

size_t GaussianProcess::nParameters() const {
    return this->approx->nParameters() + this->kernel->nParameters();
}

void GaussianProcess::getParameters(Eigen::VectorXd& params) const {
    Eigen::VectorXd aparam;
    Eigen::VectorXd kparam;
    this->approx->getParameters(aparam);
    this->kernel->getParameters(kparam);
    auto n = aparam.size() + kparam.size();
    params.resize(n);
    params << kparam, aparam;
}

void GaussianProcess::updateKernelPrecomputations(){
    if(this->kernel != nullptr && !isNullmatrix(this->obsX)){
        this->approx->updateKernelPrecomputations(this-> kernel, this->obsX, this->obsYnormalized);
    }
}

void GaussianProcess::rescaleMuInplace(Eigen::MatrixXd& mu) const{
    mu = (mu.array().rowwise()*this->scale.array()).rowwise()+this->bias.array();
}

void GaussianProcess::rescaleMuSigmaInplace(Eigen::MatrixXd& mu, Eigen::MatrixXd& sigma) const{
    mu = (mu.array().rowwise()*this->scale.array()).rowwise()+this->bias.array();
    sigma = sigma.array().rowwise()*(this->scale.array()*this->scale.array());
}

void GaussianProcess::posteriorMeanVar(Eigen::MatrixXd& mu, Eigen::MatrixXd& varSigma, const Eigen::MatrixXd& Xin) const {
    mu.resize(Xin.rows(), this->obsY.cols());
    varSigma.resize(Xin.rows(), this->obsY.cols());

    Eigen::MatrixXd kXStar;
    this->kernel->computeCrossCov(kXStar, this->obsX, Xin);
    this->approx->alphaProduct(mu, kXStar.transpose());

    Eigen::VectorXd diagK;
    this->kernel->computeCovDiag(diagK, Xin);
    Eigen::VectorXd varsig;
    this->approx->KinvScalarProduct(varsig, kXStar.transpose());
    varsig = diagK - varsig;
    varSigma = varsig.replicate(1, Xin.cols());

    this->rescaleMuInplace(mu);
    this->rescaleMuSigmaInplace(mu, varSigma);
}

void GaussianProcess::posteriorMean(Eigen::MatrixXd& mu, const Eigen::MatrixXd& Xin) const {
    mu.resize(Xin.rows(), this->obsY.cols());

    Eigen::MatrixXd kXStar;
    this->kernel->computeCrossCov(kXStar, this->obsX, Xin);
    this->approx->alphaProduct(mu, kXStar.transpose());

    this->rescaleMuInplace(mu);
}

double GaussianProcess::computeNegativeLogMarginalLikelihood() {
    return this->approx->computeNegativeLogMarginalLikelihood(this->obsYnormalized);
}

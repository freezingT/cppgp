
#include <cppgp/gp/gaussianprocess.hpp>
#include <cppgp/util/exceptions.hpp>

using namespace gp;


const Eigen::VectorXd nullvector = Eigen::VectorXd(0);
const Eigen::MatrixXd nullmatrix = Eigen::MatrixXd(0, 0);

inline bool isNullmatrix(const Eigen::MatrixXd& M){
    return M.size() == 0;
}

/**
 * Implementation class of GaussianProcesses.
 */
struct GaussianProcess::GaussianProcess_Impl {


    // store observation data
    std::shared_ptr<GPData> obsData;
    std::shared_ptr<kernel::GPKernel> kernel;
    //std::shared_ptr<GPApproximation> approx;

    //Eigen::LLT<Eigen::MatrixXd> llt;
    //bool is_logDetK_computed;
    //double logDetK;

    /*
    *************** Methods ***************
    */

    GaussianProcess_Impl(
        const std::shared_ptr<GPData>& obsData,
        const std::shared_ptr<kernel::GPKernel>& kernel
        ) :
        obsData(obsData), kernel(kernel)
    {
    }

    void dataChange_trigger()
    {
        kernel->registerData(obsData);
    }


    void rescaleMuInplace(Eigen::MatrixXd& mu) const
    {
        Eigen::RowVectorXd scale, bias;
        obsData->getScale(scale);
        obsData->getBias(bias);
        mu = (mu.array().rowwise()*scale.array()).rowwise() + bias.array(); //TODO check dimensions
    }


    void rescaleMuSigmaInplace(Eigen::MatrixXd& mu, Eigen::MatrixXd& sigma) const
    {
        Eigen::RowVectorXd scale, bias;
        obsData->getScale(scale);
        obsData->getBias(bias);
        mu = (mu.array().rowwise()*scale.array()).rowwise() + bias.array(); //TODO check dimensions
        sigma = sigma.array().rowwise()*(scale.array()*scale.array());
    }

    /**
     * Computations taken from former gpapproximation.


    void computeLogDetK(){
        if(!this->isInverseK){
            this->computeInverse();
        }
        this->logDetK = 2*this->llt.matrixL().determinant();
        this->is_logDetK_computed = true;
    }

    double getLogDetK() {
        if(!is_logDetK_computed){
            computeLogDetK();
        }
        return this->logDetK;
    }

    double computeNegativeLogMarginalLikelihood() {
        const Eigen::MatrixXd& obsYnormalized = std::get<1>(obsData->getNormalizedData());
        Eigen::VectorXd vectorprod;
        this->KinvScalarProduct(vectorprod, obsYnormalized.transpose());
        double nlml = -0.5*vectorprod.sum();
        nlml -= 0.5*obsYnormalized.cols()*this->getLogDetK();
        return -nlml;
        }
    */
};


GaussianProcess::GaussianProcess() :
    _gp_impl(std::make_unique<GaussianProcess_Impl>(
        std::shared_ptr<GPData>(nullptr),
        std::shared_ptr<kernel::GPKernel>(nullptr)))
{}


GaussianProcess::GaussianProcess(const GaussianProcess & m) : GaussianProcess()
{
    //TODO
}


GaussianProcess::GaussianProcess(const std::shared_ptr<GPData>& data) :
    _gp_impl(std::make_unique<GaussianProcess_Impl>(
        data,
        std::shared_ptr<kernel::GPKernel>(nullptr)))
{}


GaussianProcess::GaussianProcess(const std::shared_ptr<kernel::GPKernel>& kernel) :
    _gp_impl(std::make_unique<GaussianProcess_Impl>(
        std::shared_ptr<GPData>(nullptr),
        kernel))
{}


GaussianProcess::GaussianProcess(const std::shared_ptr<kernel::CovarianceFunction>& covfun) :
    _gp_impl(std::make_unique<GaussianProcess_Impl>(
        std::shared_ptr<GPData>(nullptr),
        std::make_shared<kernel::GPKernel>(covfun)))
{}

GaussianProcess::GaussianProcess(const std::shared_ptr<GPData>& data, const std::shared_ptr<kernel::GPKernel>& kernel) :
    _gp_impl(std::make_unique<GaussianProcess_Impl>(
        data,
        kernel))
{}


GaussianProcess::GaussianProcess(const std::shared_ptr<GPData>& data, const std::shared_ptr<kernel::CovarianceFunction>& covfun) :
    _gp_impl(std::make_unique<GaussianProcess_Impl>(
        data,
        std::make_shared<kernel::GPKernel>(covfun)))
{}


GaussianProcess::~GaussianProcess()
{
}


std::shared_ptr<gp::util::Prototype> GaussianProcess::copy() const
{
    return std::make_shared<GaussianProcess>(*this);
}


void GaussianProcess::setObservation(const std::shared_ptr<GPData>& obsData){
    auto old_data = this->_gp_impl->obsData;
    this->_gp_impl->obsData = obsData;
    //TODO: connect to event system to detect data changes
    try {
        this->_gp_impl->dataChange_trigger();
    }
    catch(...){ //TODO: Replace with appropriate exceptions ...?
        this->_gp_impl->obsData = old_data;
    }
}


std::shared_ptr<GPData> GaussianProcess::getObservation() const {
    return this->_gp_impl->obsData;
}


void GaussianProcess::setKernel(const std::shared_ptr<kernel::GPKernel>& kernel){
    this->_gp_impl->kernel = kernel;
    /**
     * Copy or not?
     * - Should the user be able to provide also a GPKernel or only CovFunctions and GPKernels
     * are only internally used?
     * - Event system for kernel to track changes?
     */
    _gp_impl->kernel->registerData(_gp_impl->obsData);
}

const std::shared_ptr<const kernel::GPKernel> GaussianProcess::getKernel(){
    return _gp_impl->kernel;
}


void GaussianProcess::setParameters(const Eigen::VectorXd& params) {
    auto nk = _gp_impl->kernel->nParameters();
    //auto na = _gp_impl->approx->nParameters();
    _gp_impl->kernel->setParameters(params(Eigen::seq(0, nk)));
    //_gp_impl->approx->setParameters(params(Eigen::seq(nk, na)));
}


size_t GaussianProcess::nParameters() const {
    return this->_gp_impl->kernel->nParameters();
}


void GaussianProcess::getParameters(Eigen::VectorXd& params) const {
    size_t n = 0;
    //Eigen::VectorXd aparam;
    //_gp_impl->approx->getParameters(aparam);
    // n += aparam.size();
    Eigen::VectorXd kparam;
    _gp_impl->kernel->getParameters(kparam);
    n += kparam.size();

    params.resize(n);
    params << kparam;//, aparam;
}


void GaussianProcess::posteriorMeanVar(Eigen::MatrixXd& mu, Eigen::MatrixXd& varSigma, const Eigen::MatrixXd& Xin) const
{
    Eigen::MatrixXd obsY;
    _gp_impl->obsData->getYNormalized(obsY);

    mu.resize(Xin.rows(), obsY.cols());
    varSigma.resize(Xin.rows(), obsY.cols());

    Eigen::MatrixXd kXStar;
    _gp_impl->kernel->computeCrossCov(kXStar, Xin);
    this->approx->alphaProduct(mu, kXStar.transpose());

    Eigen::VectorXd diagK;
    _gp_impl->kernel->computeCovDiag(diagK);
    Eigen::VectorXd varsig;
    this->approx->KinvScalarProduct(varsig, kXStar.transpose());
    varsig = diagK - varsig;
    varSigma = varsig.replicate(1, Xin.cols());

    _gp_impl->rescaleMuInplace(mu);
    _gp_impl->rescaleMuSigmaInplace(mu, varSigma);
}


void GaussianProcess::posteriorMean(Eigen::MatrixXd& mu, const Eigen::MatrixXd& Xin) const
{
    Eigen::MatrixXd obsY;
    _gp_impl->obsData->getYNormalized(obsY);

    mu.resize(Xin.rows(), obsY.cols());

    Eigen::MatrixXd kXStar;
    _gp_impl->kernel->computeCrossCov(kXStar, Xin);
    _gp_impl->approx->alphaProduct(mu, kXStar.transpose());

    _gp_impl->rescaleMuInplace(mu);
}


double GaussianProcess::computeNegativeLogMarginalLikelihood() {
    Eigen::MatrixXd obsYNormalized;
    _gp_impl->obsData->getYNormalized(obsYNormalized);
    return 0.0; //TODO _gp_impl->computeNegativeLogMarginalLikelihood(obsYNormalized);
}


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

    Eigen::LLT<Eigen::MatrixXd> llt;
    Eigen::MatrixXd K;    // [nData x nData]
    bool is_inverseK_computed; // true, if the inverse of K has been successfully computed, else false
    double inverseNoise;
    bool is_noise_fixed;

    Eigen::MatrixXd alpha; // [nData x nY]
    bool is_alpha_computed;

    bool is_logDetK_computed;
    double logDetK;


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
        updateKernelComputations();
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
     */

    void updateKernelComputations()
    {
        this->kernel->computeCov(this->K);
        this->computeInverse();
        this->updateAlpha();
        this->computeLogDetK();
    }

    void updateAlpha()
    {
        Eigen::MatrixXd obsYNormalized;
        this->obsData->getYNormalized(obsYNormalized);
        this->alpha = this->llt.solve(obsYNormalized.transpose()).transpose();// this->Kinv*obsYNormalized;
    }

    void alphaProduct(Eigen::MatrixXd& prod, const Eigen::MatrixXd& lfactor) const {
        prod = lfactor*this->alpha;
    }

    void KinvScalarProduct(Eigen::VectorXd& XTKinvX, const Eigen::MatrixXd& X) const {
        XTKinvX = (X.array()*(this->llt.solve(X).array())).colwise().sum();
    }

     void computeInverse() {
        int maxTries = (is_noise_fixed) ? 1 : 20;

        Eigen::VectorXd diag;
        bool success = false;
        for(int i = 0; i < maxTries; ++i){
            auto K2 = this->K;
            if(i > 0){
                inverseNoise *= 10;
            }
            diag = inverseNoise * Eigen::VectorXd::Ones(this->K.rows());
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
        this->is_inverseK_computed = true;
    }

    void computeLogDetK(){
        if(!is_inverseK_computed){
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


std::shared_ptr<util::Prototype> GaussianProcess::copy() const
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
    _gp_impl->updateKernelComputations();
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
    _gp_impl->alphaProduct(mu, kXStar.transpose());

    Eigen::VectorXd diagK;
    _gp_impl->kernel->computeCovDiag(diagK);
    Eigen::VectorXd varsig;
    _gp_impl->KinvScalarProduct(varsig, kXStar.transpose());
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
    _gp_impl->alphaProduct(mu, kXStar.transpose());

    _gp_impl->rescaleMuInplace(mu);
}


double GaussianProcess::computeNegativeLogMarginalLikelihood() {
    if(this->getKernel() == nullptr){
        return std::numeric_limits<double>::quiet_NaN();
    }
    return -this->getKernel()->computeNoisedLogDetCov();
}

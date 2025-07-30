#pragma once

#include <memory>
#include <Eigen/Eigen>

#include <cppgp/kernels/covfun.hpp>
#include <cppgp/gp/gpdata.hpp>
#include <cppgp/util/observer.hpp>
#include <cppgp/util/prototype.hpp>

namespace gp::kernel {

class GPKernel : public util::IObserver, public util::Prototype {
    /**
     * @brief Abstract kernel class.
     *
     */
    public:
    /**
     * @brief Create new GPKernel object.
     */
    GPKernel(const std::shared_ptr<gp::kernel::CovarianceFunction>& covfun);

    /**
     * @brief Create a new GPKernel object using the given parameters.
     * @param Parameters to initialize the GP kernel.
     */
    GPKernel(const std::shared_ptr<gp::kernel::CovarianceFunction> covfun, const double noise);

    /**
     * @brief Copy constructor to create a new GPKernel object.
     * @param Existing GPKernel that is used to initialize the parameters.
     */
    GPKernel(const GPKernel&);

    virtual ~GPKernel();

    /**
     * @brief Create a deep copy of the GPKernel object.
     * @return The deep copy.
     */
    virtual std::shared_ptr<util::Prototype> copy() const override;

    void registerData(const std::shared_ptr<gp::GPData> gpdata);

    void computeCov(Eigen::MatrixXd& K) const;
    void computeNoisedCov(Eigen::MatrixXd& K) const;
    void computeCrossCov(Eigen::MatrixXd& K, const Eigen::MatrixXd& X2) const;
    void computeCovDiag(Eigen::VectorXd& K) const;
    void getAlpha(Eigen::VectorXd& alpha) const;
    //void getNoisedInvCov(Eigen::MatrixXd& ICov) const;
    void getNoisedInvCov(Eigen::MatrixXd& ICov, const Eigen::MatrixXd& B) const;
    double computeNoisedLogDetCov() const;

    // Getter/Setter
    void setParameters(const Eigen::VectorXd& params, const unsigned int index=0);
    void getParameters(Eigen::VectorXd& params, const unsigned int index=0) const;
    unsigned int nParameters() const;

private:
    void precomputeAlpha();
    void changedData_trigger();

    std::shared_ptr<gp::kernel::CovarianceFunction> covfun;
    std::shared_ptr<gp::GPData> data;
    Eigen::VectorXd alpha;
    Eigen::LDLT<Eigen::MatrixXd> noisedCovDecomp;
    double noise;
};

} // namespace gp::kernel

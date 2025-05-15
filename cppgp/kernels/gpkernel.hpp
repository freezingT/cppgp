#pragma once

#include <memory>
#include <Eigen/Eigen>

namespace gp::kernel {

class GPKernel{
    /**
     * @brief Abstract kernel class.
     *
     */
    public:
    /**
     * @brief Create new GPKernel object.
     */
    GPKernel(){};

    /**
     * @brief Create a new GPKernel object using the given parameters.
     * @param Parameters to initialize the GP kernel.
     */
    GPKernel(const Eigen::VectorXd&){};

    /**
     * @brief Copy constructor to create a new GPKernel object.
     * @param Existing GPKernel that is used to initialize the parameters.
     */
    GPKernel(const GPKernel&){};
    virtual ~GPKernel(){};

    /**
     * @brief Create a deep copy of the GPKernel object.
     * @param Existing GPKernel that is used to initialize the parameters.
     */
    virtual std::shared_ptr<GPKernel> copy() const = 0;

    virtual void computeCov(Eigen::MatrixXd& K, const Eigen::MatrixXd& X) const = 0;
    virtual void computeCrossCov(Eigen::MatrixXd& K, const Eigen::MatrixXd& X1, const Eigen::MatrixXd& X2) const = 0;
    virtual void computeCovDiag(Eigen::VectorXd& K, const Eigen::MatrixXd& X) const = 0;

    // Getter/Setter
    virtual void setParameters(const Eigen::VectorXd&, const int=0){};
    virtual void getParameters(Eigen::VectorXd& params) const {params = Eigen::VectorXd(0);};
    virtual void getParameters(Eigen::VectorXd&, const int) const {};
    virtual size_t nParameters() const {return 0;};
};

} // namespace gp::kernel

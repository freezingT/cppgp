#pragma once

#include <memory>

#include <autodiff/forward/real/eigen.hpp>

namespace gp::kernel {

class CovarianceFunction {
public:
    CovarianceFunction();
    CovarianceFunction(const Eigen::VectorXd& params);
    virtual ~CovarianceFunction();
    virtual std::shared_ptr<CovarianceFunction> copy() const = 0;

    void K(Eigen::MatrixXd& K, const Eigen::MatrixXd& X);
    void K(Eigen::MatrixXd& K, const Eigen::MatrixXd& X1, const Eigen::MatrixXd& X2);
    void diagK(Eigen::VectorXd& K, const Eigen::MatrixXd& X);
    void dK_dP();
    void dK_dX();
    void ddK_dXdX();
    void dddK_dXdXdP();


    // Getter/Setter

    /**
     * \brief Set the parameters for the CovarianceFunction object.
     * Get nParameters() values from the given vector params, starting at index index,
     * and set them as parameter values for the CovarianceFunction object. It
     * is assumed that params has at least index+nParameters() entries.
     *
     * \param params The parameter vector
     * \param index The index from which the parameters are taken from params.
     */
    void setParameters(const Eigen::VectorXd& params, const unsigned int index=0);

    /**
     * \brief Get the parameters for the CovarianceFunction object.
     * Store the parameters in the given vector params, starting at index index in params.
     * It is assumed that params has at least length index+nParameters(). However: If index=0 holds,
     * the given vector params has size zero, then it is created.
     *
     * \param params The parameter vector in which the parameters are stored.
     * \param index The index from which the parameters are written in to params.
     */
    void getParameters(Eigen::VectorXd& params, const unsigned int index=0) const;

    /**
     * \brief Get number of parameters.
     * Returns the size of the parameter vector of the CovarianceFunction object.ADJ_OFFSET_SINGLESHOT
     *
     * \return the number of parameters of this object.
     */
    unsigned int nParameters() const;

    /**
     * \brief Returns a copy of the parameter vector.
     * Returns a copy of the parameter vector of the covarianceFunction object.
     *
     * \return a copy of the parameter vector
     *
     */
    Eigen::VectorXd getParameters() const;

protected:
    virtual void covariancefunction(Eigen::MatrixXd& K, const Eigen::MatrixXd& X1, const Eigen::MatrixXd& X2) const = 0;
    virtual void covariancefunction(Eigen::MatrixXd& K, const Eigen::MatrixXd& X) const = 0;
    virtual void covariancefunctionDiag(Eigen::VectorXd& K, const Eigen::MatrixXd& X) const = 0;

    Eigen::VectorXd parameters;
};


} // namespace gp::kernel

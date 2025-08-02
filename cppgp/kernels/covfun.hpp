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

#pragma once

#include <memory>
#include <autodiff/forward/real/eigen.hpp>

#include <cppgp/util/prototype.hpp>

namespace gp::kernel {

/**
 * The CovarianceFunction class is an abstract superclass for all CovarianceFunction classes
 * that implement kernel functions. These kernel functions constitute a kernel class.
 *
 * The class publicly inherits from Prototype to allow the creation of deep copies.
 */
class CovarianceFunction : public util::Prototype {
public:
    /**
     * Default constructor.
     */
    CovarianceFunction();

    /**
     * Construct using a parameter vector.
     *
     * @param params A vector to initialize the CovarianceFunction's parameters.
     */
    CovarianceFunction(const Eigen::VectorXd& params);

    /**
     * Deconstructor.
     */
    virtual ~CovarianceFunction();

    /**
     * Create a deep copy of the CovarianceFunction object
     *
     * @returns A pointer that contains a deep copy of the object.
     */
    virtual std::shared_ptr<util::Prototype> copy() const override = 0;

    /**
     * Compute the covariance matrix (Gram matrix) for this covariance function
     * using the given input data.
     *
     * @param K Returns the computed covariance matrix, size [n, n]
     * @param X The input data to compute the covariance matrix, size [n, k]
     */
    void K(Eigen::MatrixXd& K, const Eigen::MatrixXd& X);

    /**
     * Compute the cross covariance (Gram matrix) for this covariance function
     * using the given input data.
     *
     * @param K Returns the computed covariance matrix, size [n1, n2]
     * @param X1 Input data to compute the covariance matrix, size [n1, k]
     * @param X2 Input data to compute the covariance matrix, size [n2, k]
     */
    void K(Eigen::MatrixXd& K, const Eigen::MatrixXd& X1, const Eigen::MatrixXd& X2);

    /**
     * Compute the entries on the diagonal of the covariance matrix.
     *
     * @param K The diagonal entries as a vector, size [n]
     * @param X The input data to compute the covariance matrix, size [n, k]
     */
    void diagK(Eigen::VectorXd& K, const Eigen::MatrixXd& X);

    /**
     * Compute the derivative of the covariance matrix with respect to the parameter vector.
     *
     */
    void dK_dP();

    /**
     * Compute the derivative of the covariance matrix with respect to the input data.
     */
    void dK_dX();

    /**
     * Compute the second derivative of the covariance matrix with respect to the parameter vector.
     */
    void ddK_dXdX();

    /**
     *
     */
    void dddK_dXdXdP();


    /*
    ********* getter and setter *********
    */

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

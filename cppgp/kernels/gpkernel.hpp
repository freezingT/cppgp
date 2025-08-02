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
#include <Eigen/Eigen>

#include <cppgp/kernels/covfun.hpp>
#include <cppgp/gp/gpdata.hpp>
#include <cppgp/util/observer.hpp>
#include <cppgp/util/prototype.hpp>

namespace gp::kernel {


/**
     * Kernel class for Gaussian processes based on a given covariance function.
     * Receives notifications from the GPData class as soon as the data changes.
     */
class GPKernel : public util::IObserver, public util::Prototype {

    public:
    /**
     * Create a new GPKernel object.
     *
     * @param covfun The covariance function that defines the behavior of the new kernel object.
     */
    GPKernel(const std::shared_ptr<gp::kernel::CovarianceFunction>& covfun);

    /**
     * Create a new GPKernel object using the given noise parameter.
     *
     * @param covfun The covariance function that defines the behavior of the new kernel object.
     * @param noise The noise value sigma of the kernel.
     */
    GPKernel(const std::shared_ptr<gp::kernel::CovarianceFunction> covfun, const double noise);

    /**
     * Copy constructor to create a new GPKernel object.
     * A deep copy of the internal covariance function is done,
     * but only a shallow copy of the internal data.
     *
     * @param gpkernel GPKernel that is copied.
     */
    GPKernel(const GPKernel& gpkernel);

    virtual ~GPKernel();

    /**
     * Create a deep copy of the GPKernel object.
     * An exception is the shared_pointer to the data: In this case only a shallow copy is created to avoid copying the data.
     *
     * @return The deep copy.
     */
    virtual std::shared_ptr<util::Prototype> copy() const override;

    /**
     * Add observation data to the kernel object.
     *
     * @param pgdata The new data.
     */
    void registerData(const std::shared_ptr<gp::GPData> gpdata);

    /**
     * Compute the covariance matrix using the covariance function
     * based on the registered data. Mathematically, it is the Gram matrix
     * with entries \f$ K_{ij} = f_{cov}(x_i, x_j) \f$ for the covariance function \f$ f_{cov} \f$.
     * The Gram matrix corresponding to a covariance function is
     * - squared
     * - symmetric
     * - positive semi-definite
     *
     * [Rasmussen and Williams, p. 80].
     *
     * This method ignores the noise parameter.
     *
     * @param K Returns the covariance matrix, size [getN(), getN()].
     */
    void computeCov(Eigen::MatrixXd& K) const;

    /**
     * Compute the covariance matrix using the covariance function
     * based on the registered data.
     * For more details @see computeCov.
     *
     * This method uses the noise parameter \f$\sigma\f$ and computes \f$(K + \sigma I)\f$.
     *
     * @param K Returns the covariance matrix, size [getN(), getN()].
     */
    void computeNoisedCov(Eigen::MatrixXd& K) const;

    /**
     * Compute the cross covariance matrix using the covariance function
     * based on the registered data and the given data.
     *
     * For more details @see computeCov.
     *
     * This method ignores the noise parameter.
     *
     * @param K Returns the covariance matrix, size [getN(), getN()].
     * @param X2 The data that is used together with the registered GPData to compute the cross covariance. Size [k, getData()->getDimX()], where k is arbitrary.
     */
    void computeCrossCov(Eigen::MatrixXd& K, const Eigen::MatrixXd& X2) const;

    /**
     * Compute the diagonal of the covariance matrix.
     *
     * @param K Returns the diagonal of the covariance matrix as a vector, size [getN()].
     */
    void computeCovDiag(Eigen::VectorXd& K) const;

    /**
     * Compute the product of the inverse covariance matrix and the target observation vector.
     * In formulas:
     * \f[
     * \alpha := (K + \sigma I)^{-1}\bar{y}_{obs}
     * \f]
     * The covariance vector is noised, i.e. the result of #computeNoisedCov.
     * The normalized target observation vector is used.
     *
     * @param alpha Returns the (pre-)computed alpha value, size [getN()].
     */
    void getAlpha(Eigen::MatrixXd& alpha) const;

    //void getNoisedInvCov(Eigen::MatrixXd& ICov) const;

    /**
     * Set the kernel noise to the the absolute value of the given noise.
     *
     * @param noise The new noise value.
     */
    void setNoise(const double noise);

    /**
     * Get the current kernel noise
     *
     * @return The kernel noise.
     */
    double getNoise() const;

    /**
     * Compute the product of the inverse covariance matrix and a given matrix B.
     * Mathematically, it returns the solution of the following system of equations:
     * \f[
     * (K + \sigma I) X = B
     * \f]
     * The covariance vector is noised, i.e. the result of #computeNoisedCov.
     *
     * @param X Returns the computed product
     * @param B The RHS of the above system of equations. Size [getN(), k], where k is arbitrary.
     */
    void getNoisedInvCov(Eigen::MatrixXd& X, const Eigen::MatrixXd& B) const;

    /**
     * Compute the logarithm of the determinant of the noised covariance matrix.
     * \f[
     * \log\left(\det\left(K + \sigma I\right)\right)
     * \f]
     *
     * @return The logarithm of the determinant of the noised covariance matrix.
     */
    double computeNoisedLogDetCov() const;

    void testing();

    /*
    ********* getter and setter *********
    */

    /**
     * Set the parameters of this GPKernel object as a parameter vector.
     * The parameters are taken from the params vector starting at the given index.
     *
     * @param params The vector containing the new parameters for this object.
     * @param index The index at which to start taking parameters from the given vector.
     */
    void setParameters(const Eigen::VectorXd& params, const unsigned int index=0);

    /**
     * Get the parameters of this GPKernel object as a parameter vector.
     * The parameters are written into the params vector starting at the given index.
     * The size of the provided parameter vector must be at least index+nParameters().
     *
     * @param params Returns the new parameters for this object.
     * @param index The index at which to start taking parameters from the given vector.
     */
    void getParameters(Eigen::VectorXd& params, const unsigned int index=0) const;

    /**
     * Get the number of parameters of this object.
     *
     * @return The number of parameters.
     */
    unsigned int nParameters() const;

    /**
     * Get a pointer to the currently registered GPData object.
     *
     * @return The currently registered GPData object.
     */
    std::shared_ptr<GPData> getData() const;

    /**
     * Get a pointer to the internal covariance function.
     *
     * @return The internal covariance function.
     */
    std::shared_ptr<kernel::CovarianceFunction> getCovarianceFunction() const;

    /**
     * Get the number of data points in the registered GPData object.
     * This number determines the size of the covariane matrix.
     *
     * @return The number of data points n and the size of the quadratic covariance matrix.
     */
    int getN() const;

private:
    void precomputeAlpha() const;
    void changedData_trigger();

    std::shared_ptr<gp::kernel::CovarianceFunction> covfun;
    std::shared_ptr<gp::GPData> data;
    mutable Eigen::MatrixXd alpha;
    mutable Eigen::LDLT<Eigen::MatrixXd> noisedCovDecomp;
    mutable bool is_alpha_computed;
    double noise;
};

} // namespace gp::kernel

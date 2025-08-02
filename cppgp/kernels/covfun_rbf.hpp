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

#include <cppgp/kernels/covfun.hpp>

namespace gp::kernel
{

/**
 * Radial basis function class.
 *
 * This class has two parameters:\par
 * - [0]: inverse width,  default = 1.0\par
 * - [1]: variance,       default = 1.0
 *
*/
class RBFCovFun : public CovarianceFunction {
public:

    /**
     * Default constructor
     */
    RBFCovFun();

    /**
     * Constructor taking the two parameters
     *
     * @param inverseWidth The inverse width
     * @param variance The variance
     */
    RBFCovFun(const double inverseWidth, const double variance);

    /**
     * Constructor taking the two parameters as parameter vector
     */
    RBFCovFun(const Eigen::VectorXd& params);

    /**
     * Deconstructor
     */
    virtual ~RBFCovFun();

    /**
     * Create a deep copy of the object.
     *
     * @return The deep copy of the object.
     */
    virtual std::shared_ptr<util::Prototype> copy() const override;

protected:
    virtual void covariancefunction(Eigen::MatrixXd& K, const Eigen::MatrixXd& X1, const Eigen::MatrixXd& X2) const override;
    virtual void covariancefunction(Eigen::MatrixXd& K, const Eigen::MatrixXd& X) const override;
    virtual void covariancefunctionDiag(Eigen::VectorXd& K, const Eigen::MatrixXd& X) const override;
private:
    void kerncompute(Eigen::MatrixXd& k, const Eigen::MatrixXd& n2) const;
};

} // namespace gp::kernel

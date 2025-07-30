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

#include <string>
#include <tuple>
#include <memory>

#include <Eigen/Eigen>

#include <cppgp/util/observer.hpp>

namespace gp {

/**
 * Container to manage observation and input data for a Gaussian Process.
 *
 * Holds first and second order observation data used in Gaussian processes.
 * Allows adding new data and signals to all subscribers that the data has changed.
 * Provides access to its data.
 * Provides general information on the stored data (dimensions, number etc).
 *
 * Open TODOs:
 * - Implement second order observation data.
 */
class GPData : public util::ISubject {
public:

    /**
     * Create a new instance of the GPData class.
     *
     * @param dimX The dimension of the observed input observation data.
     * @param dimX The dimension of the observed output/target observation data.
     */
    GPData(const unsigned int dimX, const unsigned int dimY);

    /*
    ********* add multiple data points *********
    */

    /**
     * Add n new data points.
     * The input and target observation data must both be one-dimensional.
     *
     * @param X The input observation data, size [n]
     * @param Y The target observation data, size [n]
     */
    void addData(const Eigen::VectorXd& X, const Eigen::VectorXd& Y);

    /**
     * Add n new data points.
     * The target observation data must be one-dimensional.
     * The dimension of the input data is dimx.
     *
     * @param X The input observation data, size [n, dimx]
     * @param Y The target observation data, size [n]
     */
    void addData(const Eigen::MatrixXd& X, const Eigen::VectorXd& Y);

    /**
     * Add n new data points.
     * The target observation data must be one-dimensional.
     * The dimension of the target data is dimy.
     *
     * @param X The input observation data, size [n]
     * @param Y The target observation data, size [n, dimy]
     */
    void addData(const Eigen::VectorXd& X, const Eigen::MatrixXd& Y);

    /**
     * Add n new data points.
     * The dimension of the input data is dimx.
     * The dimension of the target data is dimy.
     *
     * @param X x-coordinates of the observation data, size [n, dimx]
     * @param X y-coordinates of the observation data, size [n, dimy]
     */
    void addData(const Eigen::MatrixXd& X, const Eigen::MatrixXd& Y);


    /*
    ********* add single data point *********
    */

    /**
     * Add a single new data point.
     * The input and target observation data must be one-dimensional.
     *
     * @param x The input observation datum
     * @param y The target observation datum
     */
    void addDatum(const double x, const double y);

    /**
     * Add a single new data point.
     * The target observation data must be one-dimensional.
     * The input data has dimension dimx.
     *
     * @param x The input observation datum, size [dimx]
     * @param y The target observation datum
     */
    void addDatum(const Eigen::VectorXd& x, const double y);

    /**
     * Add a single new data point.
     * The input observation data must be one-dimensional.
     * The target data has dimension dimy.
     *
     * @param x The input observation datum
     * @param y The target observation datum, size [dimy]
     */
    void addDatum(const double x, const Eigen::VectorXd& y);

    /**
     * Add a single new data point.
     * The input data has dimension dimx.
     * The target data has dimension dimy.
     *
     * @param x The input observation datum, size [dimx]
     * @param y The target observation datum, size [dimy]
     */
    void addDatum(const Eigen::VectorXd& x, const Eigen::VectorXd& y);

    /*
    ********* get information *********
    */

    /**
     * Get the number of data points in this container.
     *
     * @return The number of data points.
     */
    unsigned int getN() const;

    /**
     * Check if this container contains second order data points.
     *
     * @return True if this container holds second order data points.
     */
    bool hasDerivativeData() const;

    /**
     * Get the dimension of the input observation data.
     *
     * @return The dimension of the input observation data.
     */
    unsigned int getDimX() const;

    /**
     * Get the dimension of the target observation data.
     *
     * @return The dimension of the target observation data.
     */
    unsigned int getDimY() const;

    /*
    ********* retrieve data *********
    *
    * TODO: what is the best way to return the data?
    * As copy, reference or shared pointer?
    * Currently, the tuple returns a reference and the getX/getY-methods create a copy.
    */

    /**
     * Return all observation data stored in this container.
     * Currently, the returned tuple containes constant references to the stored data,
     * these references are invalidated as soon as this container is destroyed.
     *
     * @return tuple containing references to the input and target observation data
     */
    std::tuple<const Eigen::MatrixXd&, const Eigen::MatrixXd&> getData() const;

    /**
     * Return all observation data stored in this container.
     * The target data is normalized.
     * Currently, the returned tuple containes constant references to the stored data,
     * these references are invalidated as soon as this container is destroyed.
     *
     * @return References to the input and normalized target observation data
     */
    std::tuple<const Eigen::MatrixXd&, const Eigen::MatrixXd&> getNormalizedData() const;

    /**
     * Get a copy of the input observation data.
     *
     * @param X Return the input data, size [getN(), getDimX()]
     */
    void getX(Eigen::MatrixXd& X) const;

    /**
     * Get a copy of the target observation data.
     *
     * @param X Return the target data, size [getN(), getDimY()]
     */
    void getY(Eigen::MatrixXd& Y) const;

    /**
     * Get a copy of the normalized target observation data.
     * The target data is shifted by getBias() and scaled by getScale().
     *
     * @param X Return the normalized target data, size [getN(), getDimY()]
     */
    void getYNormalized(Eigen::MatrixXd& yNormalized) const;

    /**
     * Get the bias of the target data for each dimension.
     *
     * @param bias Returns the mean of the target data, size [1, getDimY()]
     */
    void getBias(Eigen::RowVectorXd& bias) const;

    /**
     * Get the scale factor of the target data for each dimension.
     * Currently, this method provides online ones as factors.
     * //TODO: make this a parameter that can be changed by the user or that is scaled automatically.
     *
     * @param bias Returns the scale of the target data, size [1, getDimY()]
     */
    void getScale(Eigen::RowVectorXd& scale) const;

private:
    Eigen::MatrixXd X; // [n, dimx]
    Eigen::MatrixXd Y; // [n, dimy]

    void updatedData_trigger(); /// This method is called when the data has changed. It resets normalization and notifies all listeners.

    // Normalized data
    mutable bool computed_normalized[3]; // flag for lazy computation ([bias, scale, yNorm])
    mutable Eigen::RowVectorXd bias;  // [nY],  lazy computation!
    mutable Eigen::RowVectorXd scale; // [nY],  lazy computation!
    mutable Eigen::MatrixXd obsYnormalized; // [nData x nY],  lazy computation!

    void computeBias() const;
    void computeScale() const;
    void computeYNorm() const;
    void resetNormalization() const;
};

}

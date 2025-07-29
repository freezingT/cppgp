#pragma once

#include <string>
#include <tuple>
#include <memory>

#include <Eigen/Eigen>

#include <cppgp/util/observer.hpp>

namespace gp {

class GPData : public gp::util::ISubject {
public:
    GPData(const unsigned int dimX, const unsigned int dimY);

    // add multiple data points
    void addData(const Eigen::VectorXd& X, const Eigen::VectorXd& Y);
    void addData(const Eigen::MatrixXd& X, const Eigen::VectorXd& Y);
    void addData(const Eigen::VectorXd& X, const Eigen::MatrixXd& Y);
    void addData(const Eigen::MatrixXd& X, const Eigen::MatrixXd& Y);

    // add single data point
    void addDatum(const double x, const double y);
    void addDatum(const Eigen::VectorXd& x, const double y);
    void addDatum(const double x, const Eigen::VectorXd& y);
    void addDatum(const Eigen::VectorXd& x, const Eigen::VectorXd& y);

    // get information
    unsigned int getN() const;
    bool hasDerivativeData() const;
    unsigned int getDimX() const;
    unsigned int getDimY() const;

    /**
     * retrieve data
     */

    //std::tuple<std::shared_ptr<const Eigen::MatrixXd>, std::shared_ptr<const Eigen::MatrixXd>> getData() const;
    std::tuple<const Eigen::MatrixXd&, const Eigen::MatrixXd&> getData() const;
    std::tuple<const Eigen::MatrixXd&, const Eigen::MatrixXd&> getNormalizedData() const;
    /**
     * TODO what is the best way to return the data?
     * As copy, reference or shared pointer?
     * Currently, the tuple returns a reference and the getX/getY-methods create a copy.
     */
    void getX(Eigen::MatrixXd& X) const;
    void getY(Eigen::MatrixXd& Y) const;

    void getBias(Eigen::RowVectorXd& bias) const; //TODO implement!
    void getScale(Eigen::RowVectorXd& scale) const;
    void getYNormalized(Eigen::MatrixXd& yNormalized) const;

private:
    Eigen::MatrixXd X;
    Eigen::MatrixXd Y;

    void updatedData_trigger();

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

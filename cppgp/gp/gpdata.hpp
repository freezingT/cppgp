#pragma once

#include <string>
#include <tuple>
#include <memory>

#include <Eigen/Eigen>


namespace gp {

class GPData {
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

    // retrieve data
    //std::tuple<std::shared_ptr<const Eigen::MatrixXd>, std::shared_ptr<const Eigen::MatrixXd>> getData() const;
    std::tuple<const Eigen::MatrixXd&, const Eigen::MatrixXd&> getData() const;

private:
    Eigen::MatrixXd X;
    Eigen::MatrixXd Y;
};

}

#pragma once

#include <Eigen/Eigen>

namespace math {

    /**
     * @brief Calculates squared distance between two sets of points.
     */
    void dist2(Eigen::MatrixXd& dist, const Eigen::MatrixXd& x1, const Eigen::MatrixXd& x2);
}

#include <cppgp/math/distance.hpp>
#include <cppgp/util/exceptions.hpp>

void math::dist2(Eigen::MatrixXd& dist, const Eigen::MatrixXd& x1, const Eigen::MatrixXd& x2){
    auto n1 = x1.rows();
    auto dim1 = x1.cols();
    auto n2 = x2.rows();
    auto dim2 = x2.cols();
    if(dim1 != dim2){
        util::exceptions::throwException<util::exceptions::InconsistentInputError>("Data dimension does not match dimension of centres");
    }

    dist =
              x1.array().square().rowwise().sum().rowwise().replicate(n2).matrix()
            + x2.array().square().rowwise().sum().rowwise().replicate(n1).transpose().matrix()
            - 2.0 * x1 * x2.transpose();

    dist = dist.cwiseMax(0.0);
}

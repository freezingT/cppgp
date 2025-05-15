#pragma once

#include <cppgp/kernels/gpkernel.hpp>

namespace gp::kernel {

    class RBFKernel : public GPKernel  {
    public:
        RBFKernel();
        RBFKernel(const Eigen::VectorXd& params);
        RBFKernel(const RBFKernel & m);
        virtual ~RBFKernel() override;
        virtual std::shared_ptr<GPKernel> copy() const override;

        virtual void computeCov(Eigen::MatrixXd& K, const Eigen::MatrixXd& X) const override;
        virtual void computeCrossCov(Eigen::MatrixXd& K, const Eigen::MatrixXd& X1, const Eigen::MatrixXd& X2) const override;
        virtual void computeCovDiag(Eigen::VectorXd& K, const Eigen::MatrixXd& X) const override;

        // Getter/Setter
        virtual void setParameters(const Eigen::VectorXd& params, const int index=0)override;
        virtual void getParameters(Eigen::VectorXd& params) const override;
        virtual void getParameters(Eigen::VectorXd& params, const int index) const override;

    private:
        double variance;
        double inverseWidth;
        Eigen::RowVectorXd inputScales;
        void _setParameters(const Eigen::VectorXd& params);
        void kerncompute(Eigen::MatrixXd& k, const Eigen::MatrixXd& n2) const;
    };

} // namespace GP

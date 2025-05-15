#pragma once

#include <Eigen/Eigen>

#include "gpapproximation.h"

namespace GP{
    
    class FTC : public GPApproximation {
    public:
        FTC(){};
        
    private:
        virtual void updateK(const std::shared_ptr<GPKernel>& kernel, const Eigen::MatrixXd& obsX) override{
            kernel->computeCov(this->K, obsX);
        }

        virtual void updateAD(const Eigen::MatrixXd& obsYNormalized) override {
            size_t n = obsYNormalized.cols();
            this->innerProducts.resize(n);
            for(int i = 0; i < n; ++i){
                this->innerProducts(i) = obsYNormalized.col(i).transpose()*this->llt.solve(obsYNormalized.col(i));
            }            
        }

        virtual void updateAlpha(const Eigen::MatrixXd& obsYNormalized) override {
            this->alpha = this->llt.solve(obsYNormalized.transpose()).transpose();// this->Kinv*obsYNormalized;
        }

        virtual double computeNegativeLogMarginalLikelihood(const Eigen::MatrixXd& obsYnormalized) override {
            Eigen::VectorXd vectorprod;
            this->KinvScalarProduct(vectorprod, obsYnormalized.transpose());
            double nlml = -0.5*vectorprod.sum();
            nlml -= 0.5*obsYnormalized.cols()*this->getLogDetK();
            return -nlml;            
        }
    };

} // GP
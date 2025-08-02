#include <cppgp/kernels/gpkernel.hpp>
#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace gp;

class DummyCovfun1 : public kernel::CovarianceFunction
{
public:
    DummyCovfun1() {this->parameters = Eigen::VectorXd::Ones(4);}
    virtual ~DummyCovfun1(){}
    virtual std::shared_ptr<util::Prototype> copy() const override {return std::make_shared<DummyCovfun1>(*this);}
protected:
    virtual void covariancefunction(Eigen::MatrixXd& K, const Eigen::MatrixXd& X1, const Eigen::MatrixXd& X2) const
    {
        K = Eigen::MatrixXd::Identity(X1.rows(), X2.rows());
    }

    virtual void covariancefunction(Eigen::MatrixXd& K, const Eigen::MatrixXd& X) const
    {
        K = Eigen::MatrixXd::Identity(X.rows(), X.rows());
    }

    virtual void covariancefunctionDiag(Eigen::VectorXd& K, const Eigen::MatrixXd& X) const
    {
        K = Eigen::VectorXd::Ones(X.rows());
    }
};

class DummyCovfun2 : public kernel::CovarianceFunction
{
public:
    virtual ~DummyCovfun2(){}
    virtual std::shared_ptr<util::Prototype> copy() const override {return std::make_shared<DummyCovfun2>(*this);}
protected:
    virtual void covariancefunction(Eigen::MatrixXd& K, const Eigen::MatrixXd& X1, const Eigen::MatrixXd& X2) const
    {
        K = Eigen::MatrixXd(X1.rows(), X2.rows());
        for(int i = 0; i < X1.rows(); ++i)
        {
            for(int j = 0; j < X2.rows(); ++j)
            {
                K(i, j) = (X1.row(i).array()*X2.row(j).array()).sum();
            }
        }
    }

    virtual void covariancefunction(Eigen::MatrixXd& K, const Eigen::MatrixXd& X) const
    {
        covariancefunction(K, X, X);
    }

    virtual void covariancefunctionDiag(Eigen::VectorXd& K, const Eigen::MatrixXd& X) const
    {
        K = (X.array()*X.array()).rowwise().sum();
    }
};

std::tuple<Eigen::MatrixXd, Eigen::MatrixXd> fill_random_data(std::shared_ptr<GPData>& data, int n)
{
    Eigen::MatrixXd X = Eigen::MatrixXd::Random(n, data->getDimX());
    Eigen::MatrixXd Y = Eigen::MatrixXd::Random(n, data->getDimY());

    data->addData(X, Y);
    return std::make_tuple(X, Y);
}

std::shared_ptr<GPData> get_test_data()
{
    std::shared_ptr<GPData> gpdata = std::make_shared<GPData>(2, 3);

    Eigen::MatrixXd X(5, 2);
    Eigen::MatrixXd Y(5, 3);
    X <<     0.5,  1.0,
            -0.5,  0.5,
             1.0,  2.0,
            -1.5, -2.0,
             1.5, -0.5;

    Y <<    -0.5,  0.5,  2.0,
            -1.5, -2.0,  1.0,
            -0.5,  0.5,  0.5,
             1.5, -0.5, -2.0,
             1.0,  2.0, -1.0;

    gpdata->addData(X, Y);
    return gpdata;
}

std::shared_ptr<GPData> get_test_data_2()
{
    std::shared_ptr<GPData> gpdata = std::make_shared<GPData>(1, 3);

    Eigen::MatrixXd X(2, 1);
    Eigen::MatrixXd Y(2, 3);
    X <<
        0.5,
        1.0;

    Y <<
        -0.5,  0.5,  2.0,
        -0.5, -2.0,  1.0;

    gpdata->addData(X, Y);
    return gpdata;
}

TEST(kernels_gpkernel, create_getCovFun_gpkernel_1){
    auto dcov = std::make_shared<DummyCovfun1>();
    kernel::GPKernel gpk(dcov);

    EXPECT_EQ(gpk.getCovarianceFunction(), dcov);
}

TEST(kernels_gpkernel, create_getCovFun_gpkernel_2){
    auto dcov = std::make_shared<DummyCovfun1>();
    double noise = 0.2;
    kernel::GPKernel gpk(dcov, noise);

    EXPECT_EQ(gpk.getCovarianceFunction(), dcov);
}

TEST(kernels_gpkernel, create_getCovFun_gpkernel_3){
    auto dcov = std::make_shared<DummyCovfun1>();
    kernel::GPKernel gpk(dcov);
    auto gpdat = std::make_shared<GPData>(3, 5);
    gpk.registerData(gpdat);

    kernel::GPKernel gpk2(gpk);

    EXPECT_FALSE(gpk.getCovarianceFunction().get() == gpk2.getCovarianceFunction().get());
    EXPECT_EQ(gpk.getData().get(), gpk2.getData().get());
}

TEST(kernels_gpkernel, copy_getCovFun){
    auto dcov = std::make_shared<DummyCovfun1>();
    std::shared_ptr<kernel::GPKernel> gpk = make_shared<kernel::GPKernel>(dcov);
    auto gpdat = std::make_shared<GPData>(3, 5);
    gpk->registerData(gpdat);
    std::shared_ptr<kernel::GPKernel> gpk2 = dynamic_pointer_cast<kernel::GPKernel>(gpk->copy());

    EXPECT_FALSE(gpk->getCovarianceFunction().get() == gpk2->getCovarianceFunction().get());
    EXPECT_EQ(gpk->getData().get(), gpk2->getData().get());
}

TEST(kernels_gpkernel, register_data_1){
    auto dcov = std::make_shared<DummyCovfun1>();
    auto gpdat = std::make_shared<GPData>(3, 5);
    kernel::GPKernel gpk(dcov);
    gpk.registerData(gpdat);

    EXPECT_EQ(gpk.getData().get(), gpdat.get());
}

TEST(kernels_gpkernel, register_data_getN_2){
    // check if new data can be added later

    auto dcov = std::make_shared<DummyCovfun1>();
    auto gpdat = std::make_shared<GPData>(3, 5);
    fill_random_data(gpdat, 4);

    kernel::GPKernel gpk(dcov);
    gpk.registerData(gpdat);
    fill_random_data(gpdat, 7);

    EXPECT_EQ(gpk.getData(), gpdat);
    EXPECT_EQ(gpk.getData()->getN(), 11);
}

TEST(kernels_gpkernel, n_parameters){
    // check if new data can be added later

    auto dcov = std::make_shared<DummyCovfun1>();
    auto gpdat = std::make_shared<GPData>(3, 5);
    fill_random_data(gpdat, 4);

    kernel::GPKernel gpk(dcov);

    EXPECT_EQ(gpk.nParameters(), 5);
}

TEST(kernels_gpkernel, get_set_parameters1){
    // check if new data can be added later

    auto dcov = std::make_shared<DummyCovfun1>();
    auto gpdat = std::make_shared<GPData>(3, 5);
    fill_random_data(gpdat, 4);

    kernel::GPKernel gpk(dcov);
    gpk.registerData(gpdat);

    Eigen::VectorXd param(7);
    param << 1, 2, 3, 4, 5, 6, 7;
    gpk.setParameters(param);

    Eigen::VectorXd resparam(10);
    gpk.getParameters(resparam);

    EXPECT_EQ(resparam(0), 1);
    EXPECT_EQ(resparam(1), 2);
    EXPECT_EQ(resparam(2), 3);
    EXPECT_EQ(resparam(3), 4);
    EXPECT_EQ(resparam(4), 5);
}

TEST(kernels_gpkernel, get_set_parameters2){
    // check if new data can be added later

    auto dcov = std::make_shared<DummyCovfun1>();
    auto gpdat = std::make_shared<GPData>(3, 5);
    fill_random_data(gpdat, 4);

    kernel::GPKernel gpk(dcov);

    Eigen::VectorXd param(7);
    param << 1, 2, 3, 4, 5, 6, 7;
    gpk.setParameters(param, 2);

    Eigen::VectorXd resparam(12);
    gpk.getParameters(resparam, 5);

    EXPECT_EQ(resparam(5), 3);
    EXPECT_EQ(resparam(6), 4);
    EXPECT_EQ(resparam(7), 5);
    EXPECT_EQ(resparam(8), 6);
    EXPECT_EQ(resparam(9), 7);
}

TEST(kernels_gpkernel, computeCov_1){
    auto dcov = std::make_shared<DummyCovfun1>();
    kernel::GPKernel gpk(dcov);

    Eigen::MatrixXd K;
    Eigen::MatrixXd Kres = Eigen::MatrixXd(0, 0);
    gpk.computeCov(K);

    EXPECT_EQ(K, Kres);
}

TEST(kernels_gpkernel, computeCov_2){
    auto dcov = std::make_shared<DummyCovfun2>();
    kernel::GPKernel gpk(dcov);
    auto gpdat = get_test_data();
    gpk.registerData(gpdat);

    Eigen::MatrixXd K;
    Eigen::VectorXd Kdiag;
    Eigen::MatrixXd Kres = Eigen::MatrixXd(5, 5);

    Kres << // computed by hand with pen and paper
     1.25,  0.25,  2.50, -2.75,  0.25,
     0.25,  0.50,  0.50, -0.25, -1.00,
     2.50,  0.50,  5.00, -5.50,  0.50,
    -2.75, -0.25, -5.50,  6.25, -1.25,
     0.25, -1.00,  0.50, -1.25,  2.50;

    gpk.computeCov(K);
    gpk.computeCovDiag(Kdiag);

    EXPECT_TRUE(K.isApprox(Kres));
    EXPECT_TRUE(Kdiag.isApprox(Kres.diagonal()));
}

TEST(kernels_gpkernel, computeCov_3){
    auto dcov = std::make_shared<DummyCovfun2>();
    kernel::GPKernel gpk(dcov);
    auto gpdat = get_test_data();
    gpk.registerData(gpdat);
    Eigen::VectorXd param(gpk.nParameters());
    double sigma = 0.4;
    param(0) = sigma;
    gpk.setParameters(param);

    Eigen::MatrixXd K;
    Eigen::MatrixXd Kres = Eigen::MatrixXd(5, 5);

    Kres <<
     1.25,  0.25,  2.50, -2.75,  0.25,
     0.25,  0.50,  0.50, -0.25, -1.00,
     2.50,  0.50,  5.00, -5.50,  0.50,
    -2.75, -0.25, -5.50,  6.25, -1.25,
     0.25, -1.00,  0.50, -1.25,  2.50;

    gpk.computeNoisedCov(K);

    EXPECT_TRUE(K.isApprox(Kres + Eigen::MatrixXd::Identity(Kres.rows(), Kres.cols())*sigma));
    EXPECT_EQ(K, Kres + Eigen::MatrixXd::Identity(Kres.rows(), Kres.cols())*sigma);
}

TEST(kernels_gpkernel, computeInvCov){
    auto dcov = std::make_shared<DummyCovfun2>();
    kernel::GPKernel gpk(dcov);
    auto gpdat = get_test_data_2();
    gpk.registerData(gpdat);
    Eigen::VectorXd param(gpk.nParameters());
    double sigma = 0.25;
    param(0) = sigma;
    gpk.setParameters(param);

    Eigen::MatrixXd invK, K, alpha;
    Eigen::MatrixXd invKres(2, 2);
    Eigen::MatrixXd alphaRes(2, 3);

    invKres << // computed by hand with pen and paper
         5/4.0, -1/2.0,
        -1/2.0,  1/2.0;
    invKres *= 8/3.0;

    gpk.computeNoisedCov(K);
    gpk.getNoisedInvCov(invK, Eigen::MatrixXd::Identity(2,2));
    gpk.getAlpha(alpha);

    EXPECT_TRUE(invK.isApprox(invKres));
    EXPECT_TRUE(std::get<1>(gpdat->getNormalizedData()).isApprox(K*alpha)); // check that K*alpha = yNormalized
}


TEST(kernels_gpkernel, computeLog){
    auto dcov = std::make_shared<DummyCovfun2>();
    kernel::GPKernel gpk(dcov);
    auto gpdat = get_test_data_2();
    gpk.registerData(gpdat);
    Eigen::VectorXd param(gpk.nParameters());
    double sigma = 0.25;
    param(0) = sigma;
    gpk.setParameters(param);

    double detRes = 3/8.0;
    double det = gpk.computeNoisedLogDetCov();

    EXPECT_THAT(det, testing::DoubleEq(std::log(detRes)));
}

TEST(kernels_gpkernel, get_set_noise){
    auto dcov = std::make_shared<DummyCovfun1>();
    kernel::GPKernel gpk(dcov);
    //auto gpdat = get_test_data_2();
    //gpk.registerData(gpdat);

    double sigma = 0.333;
    gpk.setNoise(sigma);

    EXPECT_EQ(gpk.getNoise(), sigma);

    Eigen::VectorXd param(gpk.nParameters());
    sigma = 1.5;
    param(0) = sigma;
    gpk.setParameters(param);

    EXPECT_EQ(gpk.getNoise(), sigma);

    sigma = -2.0;
    gpk.setNoise(sigma);

    EXPECT_EQ(gpk.getNoise(), -sigma);
}

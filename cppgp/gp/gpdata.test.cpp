
#include <cppgp/gp/gpdata.hpp>

#include <iostream>

#include <gtest/gtest.h>

using namespace gp;

TEST(create_gpdata, create_gpdata_dimensions){
    GPData gpdat1(1, 2);
    GPData gpdat2(3, 5);

    EXPECT_EQ(gpdat1.getDimX(), 1);
    EXPECT_EQ(gpdat1.getDimY(), 2);

    EXPECT_EQ(gpdat2.getDimX(), 3);
    EXPECT_EQ(gpdat2.getDimY(), 5);
}

TEST(add_gpdata_getN, add_gpdata_getN_dim1){
    GPData gpdat(1, 1);

    gpdat.addDatum(1.0, 2.0);
    EXPECT_EQ(gpdat.getN(), 1);

    gpdat.addDatum(0.0, 1.0);
    EXPECT_EQ(gpdat.getN(), 2);

    Eigen::VectorXd x(5), y(5);
    x << 1.5, 2.5, 3.5, 4.5, 5.5;
    y << 1.5, 2.5, 3.5, 4.5, 5.5;
    gpdat.addData(x, y);
    EXPECT_EQ(gpdat.getN(), 7);
}

TEST(add_gpdata_getN, add_gpdata_getN_dimN){
    GPData gpdat(2, 3);

    Eigen::VectorXd x1(2), y1(3);
    x1 << 0.0, 0.0;
    y1 << 0.0, 1.0, 0.5;

    gpdat.addDatum(x1, y1);
    EXPECT_EQ(gpdat.getN(), 1);


    Eigen::MatrixXd x2(5,2), y2(5,3);
    x2 << 1.5, 2.5,
        0.5, 2.5,
        0.5, 3.5,
        1.5, 0.5,
        1.5, 1.5;
    y2 << -1.5, 2.5, 2.5,
        0.5, -2.5, 0.5,
        0.5, 3.5, 1.5,
        -1.5, 0.5, 3.5,
        1.5, 1.5, 2.5;
    gpdat.addData(x2, y2);
    EXPECT_EQ(gpdat.getN(), 6);

    x1(1) = 3.0;
    y1(0) = 1.0;
    gpdat.addDatum(x1, y1);
    EXPECT_EQ(gpdat.getN(), 7);
}


TEST(add_gpdata_data, add_gpdata_1to1){
    //Eigen::MatrixXd resX, resY;
    Eigen::Matrix<double, 1, 1> expX1, expY1;
    GPData gpdat(1, 1);

    gpdat.addDatum(1.0, 2.0);
    auto res = gpdat.getData();
    Eigen::Matrix<double, 1, 1> expX2, expY2;
    expX2 << 1.0;
    expY2 << 2.0;
    EXPECT_EQ(expX2, std::get<0>(res));
    EXPECT_EQ(expY2, std::get<1>(res));

    gpdat.addDatum(0.0, 1.0);
    auto res2 = gpdat.getData();
    Eigen::Matrix<double, 2, 1> expX3, expY3;
    expX3 << 1.0, 0.0;
    expY3 << 2.0, 1.0;
    EXPECT_EQ(expX3, std::get<0>(res2));
    EXPECT_EQ(expY3, std::get<1>(res2));

    Eigen::VectorXd x(5), y(5);
    x << 1.5, 2.5, 3.5, 4.5, 5.5;
    y << 1.5, 2.5, 3.5, 4.5, 5.5;
    gpdat.addData(x, y);
    auto res3 = gpdat.getData();

    Eigen::Matrix<double, 7, 1> expX4, expY4;
    expX4 << 1.0, 0.0, 1.5, 2.5, 3.5, 4.5, 5.5;
    expY4 << 2.0, 1.0, 1.5, 2.5, 3.5, 4.5, 5.5;
    EXPECT_EQ(expX4, std::get<0>(res3));
    EXPECT_EQ(expY4, std::get<1>(res3));
}

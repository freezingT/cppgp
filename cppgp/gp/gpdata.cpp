#include <cppgp/gp/gpdata.hpp>




gp::GPData::GPData(const unsigned int dimX, const unsigned int dimY) :
    X(0, dimX), Y(0, dimY){}


void gp::GPData::addData(const Eigen::VectorXd& X, const Eigen::VectorXd& Y){
    int n = this->getN();
    int dimx = this->getDimX();
    int dimy = this->getDimY();

    assert(dimx == 1);
    assert(dimy == 1);
    assert(X.size() == Y.size());

    Eigen::MatrixXd newX(n+X.size(), dimx);
    newX << this->X, X;
    Eigen::MatrixXd newY(n+X.size(), dimy);
    newY << this->Y, Y;

    this->X = newX;
    this->Y = newY;
}


void gp::GPData::addData(const Eigen::MatrixXd& X, const Eigen::VectorXd& Y){
    int n = this->getN();
    int dimx = this->getDimX();
    int dimy = this->getDimY();

    assert(X.cols() == dimx);
    assert(dimy == 1);
    assert(X.rows() == Y.size());

    Eigen::MatrixXd newX(n+X.rows(), dimx);
    newX << this->X, X;
    Eigen::MatrixXd newY(n+X.size(), dimy);
    newY << this->Y, Y;

    this->X = newX;
    this->Y = newY;
}



void gp::GPData::addData(const Eigen::VectorXd& X, const Eigen::MatrixXd& Y){
    int n = this->getN();
    int dimx = this->getDimX();
    int dimy = this->getDimY();

    assert(dimx == 1);
    assert(Y.cols() == dimy);
    assert(X.size() == Y.rows());

    Eigen::MatrixXd newX(n+X.size(), dimx);
    newX << this->X, X;
    Eigen::MatrixXd newY(n+X.rows(), dimy);
    newY << this->Y, Y;

    this->X = newX;
    this->Y = newY;
}



void gp::GPData::addData(const Eigen::MatrixXd& X, const Eigen::MatrixXd& Y){
    int n = this->getN();
    int dimx = this->getDimX();
    int dimy = this->getDimY();

    assert(X.cols() == dimx);
    assert(Y.cols() == dimy);
    assert(X.rows() == Y.rows());

    Eigen::MatrixXd newX(n+X.rows(), dimx);
    newX << this->X, X;
    Eigen::MatrixXd newY(n+X.rows(), dimy);
    newY << this->Y, Y;

    this->X = newX;
    this->Y = newY;
}



void gp::GPData::addDatum(const double x, const double y){
    Eigen::Vector<double, 1> Xnew;
    Xnew << x;
    Eigen::Vector<double, 1> Ynew;
    Ynew << y;
    this->addDatum(Xnew, Ynew);
}



void gp::GPData::addDatum(const Eigen::VectorXd& x, const double y){
    Eigen::Vector<double, 1> Ynew;
    Ynew << y;
    this->addDatum(x, Ynew);
}



void gp::GPData::addDatum(const double x, const Eigen::VectorXd& y){
    Eigen::Vector<double, 1> Xnew;
    Xnew << x;
    this->addDatum(Xnew, y);
}



void gp::GPData::addDatum(const Eigen::VectorXd& x, const Eigen::VectorXd& y){
    int n = this->getN();
    int dimx = this->getDimX();
    int dimy = this->getDimY();

    assert(x.rows() == dimx);
    assert(y.rows() == dimy);

    Eigen::MatrixXd newX(n+1, dimx);
    newX << this->X, x.transpose();
    Eigen::MatrixXd newY(n+1, dimy);
    newY << this->Y, y.transpose();

    this->X = newX;
    this->Y = newY;
}



unsigned int gp::GPData::getN() const{
    return this->X.rows();
}



bool gp::GPData::hasDerivativeData() const {
    return false; // NOT YET IMPLEMENTED.
}



unsigned int gp::GPData::getDimX() const {
    return this->X.cols();
}



unsigned int gp::GPData::getDimY() const {
    return this->Y.cols();
}


//std::tuple<std::shared_ptr<const Eigen::MatrixXd>, std::shared_ptr<const Eigen::MatrixXd>> gp::GPData::getData() const{
std::tuple<const Eigen::MatrixXd&, const Eigen::MatrixXd&> gp::GPData::getData() const{
    const Eigen::MatrixXd& xptr = this->X;
    const Eigen::MatrixXd& yptr = this->Y;
    auto data = std::tie(xptr, yptr);
    return data;
}

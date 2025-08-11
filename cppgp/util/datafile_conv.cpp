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

#include <cppgp/util/datafile_conv.hpp>

#include <map>
#include <Eigen/Core>

namespace util::data {

bool sEigenMatrix(std::ostream& stream, const DFVariable& variable) {
    std::shared_ptr<Eigen::MatrixXd> ptr = retrieve<Eigen::MatrixXd>(variable, "EigenMatrixXd");
    stream
        //<< "EigenMatrixXd "
        << ptr->rows() << " "
        << ptr->cols() << " ";
    for(long i = 0; i < ptr->size(); ++i){
        stream << *(ptr->data()+i) << " ";
    }
    return true;
}

bool uEigenMatrix(std::istream& stream, DFVariable& variable) {
    int nr, nc;
    stream >> nr;
    stream >> nc;
    std::shared_ptr<Eigen::MatrixXd> ptr = std::make_shared<Eigen::MatrixXd>(nr, nc);
    for(long i = 0; i < ptr->size(); ++i){
        stream >> *(ptr->data()+i);
    }
    variable = DFVariable("EigenMatrixXd", std::static_pointer_cast<void>(ptr));
    return true;
}

typedef std::function<bool (std::ostream &, const DFVariable &)> SFun; // serialize function
typedef std::function<bool (std::istream &, DFVariable &)> UFun; // unserialize function

class VariableHandlerRegister
{
public:
    VariableHandlerRegister(const VariableHandlerRegister& vhr) = delete;
    VariableHandlerRegister(const VariableHandlerRegister&& vhr) = delete;

    static std::shared_ptr<VariableHandlerRegister> get();

    bool streamData(std::ostream& stream, const DFVariable& variable);
    bool streamData(std::istream& stream, DFVariable& variable);
    bool registerDataType(const std::string& typeID,
                          const std::function<bool(std::ostream&, const DFVariable&)>& serialize,
                          const std::function<bool(std::istream&, DFVariable&)> unserialize);

    std::vector<std::string> getRegisteredDataTypes();
private:
    VariableHandlerRegister();

    static std::shared_ptr<VariableHandlerRegister> m_instance;

    std::map<std::string, SFun> serializeFunctions;
    std::map<std::string, UFun> unserializeFunctions;
};

std::shared_ptr<VariableHandlerRegister> VariableHandlerRegister::m_instance = nullptr;


std::shared_ptr<VariableHandlerRegister> VariableHandlerRegister::get()
{
    if(m_instance == nullptr){
        m_instance = std::shared_ptr<VariableHandlerRegister>(new VariableHandlerRegister());
    }
    return m_instance;
}

bool VariableHandlerRegister::streamData(std::ostream &stream, const DFVariable &variable)
{
    return serializeFunctions.at(variable.typeIdentifier())(stream, variable);
}

bool VariableHandlerRegister::streamData(std::istream &stream, DFVariable &variable)
{
    return unserializeFunctions.at(variable.typeIdentifier())(stream, variable);
}

bool VariableHandlerRegister::registerDataType(const std::string &typeID, const std::function<bool (std::ostream &, const DFVariable &)> &serialize, const std::function<bool (std::istream &, DFVariable &)> unserialize)
{
    if(serializeFunctions.contains(typeID) || serialize == nullptr || unserialize == nullptr){
        return false;
    }
    serializeFunctions.insert(std::pair(typeID, serialize));
    unserializeFunctions.insert(std::pair(typeID, unserialize));
    return true;
}

std::vector<std::string> VariableHandlerRegister::getRegisteredDataTypes()
{
    std::vector<std::string> keys;
    for(const auto &c : serializeFunctions){
        keys.push_back(std::get<0>(c));
    }
    return keys;
}

VariableHandlerRegister::VariableHandlerRegister()
{
    registerDataType("EigenMatrixXd", sEigenMatrix, uEigenMatrix);
    //TODO do this for more data types!!!
}


bool VariableHandler::streamData(std::ostream &stream, const DFVariable &variable)
{
    return VariableHandlerRegister::get()->streamData(stream, variable);
}

bool VariableHandler::streamData(std::istream &stream, DFVariable &variable)
{
    return VariableHandlerRegister::get()->streamData(stream, variable);
}

bool VariableHandler::registerDataType(const std::string &typeID, const std::function<bool (std::ostream &, const DFVariable &)> &serialize, const std::function<bool (std::istream &, DFVariable &)> unserialize)
{
    return VariableHandlerRegister::get()->registerDataType(typeID, serialize, unserialize);
}

std::vector<std::string> VariableHandler::getRegisteredDataTypes()
{
    return VariableHandlerRegister::get()->getRegisteredDataTypes();
}

} // namespace util::data

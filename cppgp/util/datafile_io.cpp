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

#include <cppgp/util/datafile_io.hpp>
#include <cppgp/util/datafile_conv.hpp>

#include <assert.h>
#include <iomanip>
#include <limits>
#include <fstream>

using namespace util::data;

typedef std::iostream::pos_type PosType;

int computeNumberOfDigitsInPosType(){
    int ndigits = 0;
    return std::numeric_limits<unsigned long long>::digits10 + 1;
    // the following is nice but not working :-(
    if(std::is_same<PosType, long long>::value)
    {
        ndigits = std::numeric_limits<long long>::digits10 + 1;
    }
    else if (std::is_same<PosType, long>::value) {
        ndigits = std::numeric_limits<long>::digits10 + 1;
    }
    else if(std::is_same<PosType, int>::value){
        ndigits = std::numeric_limits<int>::digits10 + 1;
    }
    else if(std::is_same<PosType, unsigned int>::value){
        ndigits = std::numeric_limits<unsigned int>::digits10 + 1;
    }
    else if(std::is_same<PosType, unsigned long>::value){
        ndigits = std::numeric_limits<unsigned long>::digits10 + 1;
    }
    else if(std::is_same<PosType, unsigned long long>::value){
        ndigits = std::numeric_limits<unsigned long long>::digits10 + 1;
    }
    else {
        throw std::runtime_error("Cannot determine the type of std::ifstream::off_type. Programmer, do something!");
    }
    return ndigits;
}

const int c_postypeLength = computeNumberOfDigitsInPosType();
const int c_numVariableLength = 10;
const int c_varnameLength = 16;


class ITextDataFileIO::DatafileHandler
{
public:
    DatafileHandler(const std::string& filename, int entriesPerSection=0);
    ~DatafileHandler();

    void writeHeaderSection(std::ostream& stream, const std::string& versionNumber, const int numEntries);
    void writeVersion(std::ostream& stream, const std::string& versionNumber);
    //void writeVarsTotal(std::ostream& stream, const unsigned int number=0);
    bool writeVartableSection(std::ostream& stream, const int numEntries);
    void writeNewVariable(std::ostream& stream, const std::string& varname, const DFVariable& variable);

    std::string readVersion(std::istream& stream);
    void readHeaderSection(std::istream& stream);

private:
    std::fstream file;
    const std::string filename;          //! the name of the file on which the DatafileHandler object operates
    int entriesPerSection;               //! how many entries there are per default in the vartable sections

    std::vector<PosType> m_pos_varname;  //! positions of the variable names in the vartable sections
    std::vector<PosType> m_pos_varpos;   //! positions of the variable data position in the vartable sections
    std::vector<PosType> m_pos_variable; //! positions of the data positions of the variables in the file
};

#define WRITE_POS_IF_ZERO(STREAM, VAR) if(!VAR) { VAR = STREAM.tellp(); }


ITextDataFileIO::DatafileHandler::DatafileHandler(const std::string &filename, int entriesPerSection)
{
    //TODO
    file.open(filename);
    // remove 'stream' argument in the methods in this class?
}

ITextDataFileIO::DatafileHandler::~DatafileHandler()
{
    file.close();
}

void ITextDataFileIO::DatafileHandler::writeHeaderSection(std::ostream &stream, const std::string &versionNumber, const int numEntries)
{
    writeVersion(stream, versionNumber);
    writeVartableSection(stream, numEntries);
}

void ITextDataFileIO::DatafileHandler::writeVersion(std::ostream &stream, const std::string &versionNumber)
{
    stream << versionNumber << " ";
}

// void ITextDataFileIO::DatafileHandler::writeVarsTotal(std::ostream &stream, const unsigned int number)
// {
//     WRITE_POS_IF_ZERO(stream, m_pos_num_vars)
//     stream << std::setfill('0')
//            << std::setw(c_numVariableLength)
//            << number
//            << " ";
// }

inline void writeNumEntriesInVartable(std::ostream &stream, const int numEntries){
    stream << std::setfill('0')
        << std::setw(c_numVariableLength)
        << numEntries
        << " ";
}

bool ITextDataFileIO::DatafileHandler::writeVartableSection(std::ostream &stream, int numEntries)
{
    if(numEntries <= 0){
        numEntries = entriesPerSection;
    }
    if(stream.fail()) return false; // necessary to check frequently?

    writeNumEntriesInVartable(stream, numEntries);
    if(stream.fail()) return false;

    for(int i = 0; i < numEntries; ++i){
        stream << std::string(16, ' ')
               << " "
               << std::string(19, '0')
               << " ";
    }

    stream << "^ ";
    return true;
}

void ITextDataFileIO::DatafileHandler::writeNewVariable(std::ostream& stream, const std::string& varname, const DFVariable& variable){
    size_t n = m_pos_variable.size();
    if(n >= m_pos_varpos.size()){ // check if there is no free slot in an existing vartable section
        writeVartableSection(stream, std::min(entriesPerSection, 10)); // make 10 also a member variable?
    }

    variable.rawDataPtr();
    m_pos_variable.push_back(stream.tellp());
    // if all positions required to create a new variable entry

    // write the data in the file
    util::data::VariableHandler::streamData(stream, variable);

    stream << "^ ";
}


ITextDataFileIO::ITextDataFileIO(const std::string &filename, const std::string &versionNumber, const int nHead)
{

}

ITextDataFileIO::ITextDataFileIO(const std::string &filename)
{

}

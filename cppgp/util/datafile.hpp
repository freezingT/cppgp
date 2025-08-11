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

#pragma once

#include <string>
#include <vector>
#include <functional>
#include <memory>

/**
 * TODO:
 * Implement a loader class for data.
 *
 * Objectives:
 *
 * - Save data to a new file.
 * - Append data to an existing file.
 * - Load all data from a file.
 * - Load a specific variable from a file by providing the name of this variable.
 * - Load the following information from the saved data in the file:
 *   -> variable name
 *   -> data type
 *   -> size in bytes and number of entries/elements.
 * - Save files can be in text or binary.
 * - The list of supported data types that can be stored in the data file is easy
 *   to extend.
 * - Data (like for strings, matrices and vectors) can be streamed from the file,
 *   such that e.g., Eigen::Matrices can be easily initialized.
 * - If all
 *
 *
 * == Implementation Ideas ==
 * - Use a header section where all variables in this file are listed by name.
 *   For each variable, the address (seekg/p, tellg/p) is stored such that on retrieval,
 *   it is possible to directly jump to the location in the file, ignoring potentially
 *   large amounts of data before.
 *      -> Should this header section also contain the information about the data type or
 *         the size of the objects? Probably not
 * - The very first entry in the save file is a version number such that the format
 *   of the save file might be changed later if necessary.
 * - After the version number comes the first header section. The first entry in a header
 *   section is the number of entry slots N in this section. The header section may
 *   contain unused entry slots.
 * - The next N values in the header section are [variable_name, data_location_in_file]
 * - The size of each entry [variable_name, data_location_in_file] must be
 *   the same for all variables and all data types. Implication:
 *   -> The length of valid variable names must be limited.
 * - The (N+1)th value in the header section is used if more than N variables are stored
 *   in this save file. In this case, the (N+1)th entry points to the location in the
 *   save file where the header is continued.
 * - Use end markers after each header and after each variable. This simplifies recovery
 *   of the variables/data if something is broken.
 *
 * Maybe it is better to support some built-in data types and provide a class for custom
 * data types. The built-in data types might be
 * - string
 * - Eigen::Matrix
 * - Eigen::Vector
 * - double
 * - int
 * - ...?
 */



namespace util::data {






} // namespace util::data



 /*

/**
 * Abstract class
 * Allow to use additional data types in the data file.
 *
 * WIP!
 * Who to design this class properly?
 * /
template<class Type>
class IDataFileIOTypeHandler
{
public:
    IDataFileIOTypeHandler();
    virtual ~IDataFileIOTypeHandler();

    /**
     * Allows the following use:
     * datafileobj.append(IDataFileIOTypeHandlerCustomClass << mycustomclassobj)
     * /
    //static IDataFileIOTypeHandler operator<<(const Type obj)=0;

    /**
     * Get the identifier by which the IDataFileIO class recognizes that the found
     * data type can be parsed by this handler.
     * /
    //static std::string getTypeIdentifier()=0;
    //virtual std::string getTypeIdentifier() const=0;

    virtual int getSize() const=0;
    virtual unsigned long getNumber() const;

    /**
     * Performs the serialization
     * /
    virtual std::string stringRepresentation() const=0;
};







/**
 * Implement the read and write operations on a binary save file.
 * /
class IBinaryDataFileIO : public IDataFileIO
{

};

/**
 * Interact with data save files.
 * Read data and save or append data.
 * /
 class DataFile
 {
public:

    enum class TYPE {BINARY, TEXT};

    struct VariableInfo
    {
        std::string variableName;
        std::string typeIdentifier;
        unsigned long byteSize;
        unsigned long length;
    };

    DataFile(const std::string& filename);

    static Variable readVariable(const std::string& variableName);
    static Workspace readData(const std::vector<std::string>& variableNames);
    static Workspace readData();
    static Workspace saveData(const TYPE = TYPE::TEXT);
    static void appendData();

    static std::vector<std::string> getVariableNames(const std::string& filename);
    static std::vector<VariableInfo> getVariableInformation(const std::string& filename, const std::string& varname="");

    std::string getFilename() const;
private:
    static const std::string VERSION_NUMBER;
    static const int POSTYPE_LENGTH;


    std::string filename;


 };


} // namespace util::data

*/

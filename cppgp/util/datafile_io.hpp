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

#include <cppgp/util/workspace.hpp>

#include <iostream>
#include <memory>

namespace util::data {

/**
  * Abstract class.
  * Implement the read and write operations on the actual file.
  */
class IDataFileIO
{

};

/**
 * Implement the read and write operations on a text save file.
 */
class ITextDataFileIO : public IDataFileIO
{
public:
    ITextDataFileIO()=delete;

    static std::unique_ptr<ITextDataFileIO> open(const std::string& filename);
    static std::unique_ptr<ITextDataFileIO> create(const std::string& filename, const std::string& versionNumber, const int nHead);


    void writeDFVariable(std::ostream& stream, const DFVariable& variable) const;

    void readDFVariable(std::istream& stream, DFVariable& variable) const;

private:
    class DatafileHandler;
    std::unique_ptr<DatafileHandler> dfh;

    ITextDataFileIO(const std::string& filename, const std::string& versionNumber, const int nHead);
    ITextDataFileIO(const std::string& filename);
};


} // namespace util::data

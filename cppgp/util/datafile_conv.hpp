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
#ifndef DATAFILE_CONV_H
#define DATAFILE_CONV_H

#include <memory.h>
#include <functional>
#include <cppgp/util/workspace.hpp>

namespace util::data {


    template<class T>
    std::shared_ptr<T> retrieve(const DFVariable& variable, const std::string& typeID="")
    {
        if(typeID.size() == 0 || variable.isType(typeID)){
            return std::static_pointer_cast<T>(variable.rawDataPtr());
        }
        return std::shared_ptr<T>(nullptr);
    }

    namespace VariableHandler {

        bool streamData(std::ostream& stream, const DFVariable& variable);
        bool streamData(std::istream& stream, DFVariable& variable);

        bool registerDataType(const std::string& typeID,
                              const std::function<bool(std::ostream&, const DFVariable&)>& serialize,
                              const std::function<bool(std::istream&, DFVariable&)> unserialize);

        std::vector<std::string> getRegisteredDataTypes();
    };

} // namespace util::data

#endif // DATAFILE_CONV_H

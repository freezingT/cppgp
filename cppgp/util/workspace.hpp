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
#include <memory>
#include <vector>

namespace util::data {


/**
 * @brief Container for a variable loaded from a DataFile.
 */
class DFVariable
{
public:
    DFVariable(const std::string& type, const std::shared_ptr<void>& data);

    /**
     * @brief Check if the data in this DFVariable is of the given type.
     * @param typeIdentifier Data type identifier.
     * @return True if the data is of the given type, else false.
     */
    bool isType(const std::string &typeIdentifier) const;

    /**
     * @brief Get the typeIdentifier string for the stored variable.
     * @return The typeIdentifier string for the stored variable.
     */
    std::string typeIdentifier() const;

    /**
     * @brief Get a pointer to the data stored in this variable.
     * @return The shared pointer to the stored data.
     */
    std::shared_ptr<void> rawDataPtr() const;

private:
    std::shared_ptr<void> m_data;
    std::string m_typeIdentifier;
};


/**
 * @brief Container that provides access to all variables loaded from a data file.
 */
class Workspace
{
public:
    Workspace(const std::vector<std::string>& varnames,
              const std::vector<DFVariable>& variables);
    ~Workspace();

    /**
     * Add a new named variable to the workspace.
     * The name of the new variable must not exist already in the workspace to be added.
     * @param varname The name of the new variable
     * @param variable The new variable
     * @return True if the new variable has been added, else false.
     */
    bool addEntry(const std::string& varname, const util::data::DFVariable& variable);

    DFVariable operator[](const std::string& variableName) const;
    DFVariable operator[](const size_t id) const;


    DFVariable pop(const std::string& variableName);
    DFVariable pop(const size_t id=0);

    size_t length() const;
    const std::vector<std::string>& getVariableNames() const;

private:
    class WS_Impl;
    std::unique_ptr<WS_Impl> m_wsImpl;
};



} // namespace util::data

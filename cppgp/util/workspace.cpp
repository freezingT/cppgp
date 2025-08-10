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




#include <cppgp/util/workspace.hpp>


util::data::DFVariable::DFVariable(const std::string &type, const std::shared_ptr<void> &data)
    : m_data(data)
    , m_typeIdentifier(type)
{}

bool util::data::DFVariable::isType(const std::string &typeIdentifier) const
{
    return m_typeIdentifier.compare(typeIdentifier) == 0;
}

std::string util::data::DFVariable::typeIdentifier() const
{
    return m_typeIdentifier;
}

std::shared_ptr<void> util::data::DFVariable::rawDataPtr() const
{
    return m_data;
}

class util::data::Workspace::WS_Impl
{
public:
    WS_Impl(const std::vector<std::string>& varnames,
            const std::vector<DFVariable>& variables)
        : m_varnames(varnames), m_vars(variables)
    {
        if(m_varnames.size() != m_vars.size()){
            throw std::runtime_error("The number of variables and names given must be the same.");
        }
    }

    bool addEntry(const std::string& varname, const util::data::DFVariable& variable){
        if(std::find(m_varnames.begin(), m_varnames.end(), varname) != m_varnames.end()){
            return false;
        }
        m_varnames.push_back(varname);
        m_vars.push_back(variable);
        return true;
    }

    auto begin() const {
        return m_vars.begin();
    }
    auto end() const {
        return m_vars.end();
    }

    size_t length() const {
        return m_vars.size();
    }

    const std::vector<std::string>& varnames() const{
        return m_varnames;
    }

    inline size_t varname2id(const std::string& varname){
        return std::find(m_varnames.begin(), m_varnames.end(), varname) - m_varnames.begin();
    }

    DFVariable get(const size_t id){
        return m_vars.at(id);
    }

    DFVariable pop(const size_t id){
        DFVariable result(m_vars.at(id));
        m_vars.erase(m_vars.begin()+id);
        m_varnames.erase(m_varnames.begin()+id);
        return result;
    }

private:
    std::vector<std::string> m_varnames; // alternative: use a map std::string -> id
    std::vector<util::data::DFVariable> m_vars;
};


util::data::Workspace::Workspace(const std::vector<std::string> &varnames, const std::vector<DFVariable> &variables)
    : m_wsImpl(std::make_unique<WS_Impl>(varnames, variables))
{}

util::data::Workspace::~Workspace(){}

bool util::data::Workspace::addEntry(const std::string& varname, const util::data::DFVariable& variable){
    return m_wsImpl->addEntry(varname, variable);
}

util::data::DFVariable util::data::Workspace::operator[](const std::string& variableName) const {
    return m_wsImpl->get(m_wsImpl->varname2id(variableName));
}

util::data::DFVariable util::data::Workspace::operator[](const size_t id) const {
    return m_wsImpl->get(id);
}

std::vector<util::data::DFVariable>::const_iterator util::data::Workspace::begin() const
{
    return m_wsImpl->begin();
}

std::vector<util::data::DFVariable>::const_iterator util::data::Workspace::end() const
{
    return m_wsImpl->end();
}

util::data::DFVariable util::data::Workspace::pop(const std::string &variableName)
{
    return m_wsImpl->pop(m_wsImpl->varname2id(variableName));
}

util::data::DFVariable util::data::Workspace::pop(const size_t id)
{
    return m_wsImpl->pop(id);
}

size_t util::data::Workspace::length() const
{
    return m_wsImpl->length();
}

const std::vector<std::string> &util::data::Workspace::getVariableNames() const
{
    return m_wsImpl->varnames();
}

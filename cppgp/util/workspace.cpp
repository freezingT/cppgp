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

#include <map>

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
    {
        if(varnames.size() != variables.size()){
            throw std::runtime_error("The number of variables and names given must be the same.");
        }
        for(size_t i = 0; i < varnames.size(); ++i){
            auto res = m_varmap.insert(std::pair(varnames[i], variables[i]));
            if(std::get<1>(res)){
                m_varnames.push_back(varnames[i]);
            }
        }
    }

    bool addEntry(const std::string& varname, const util::data::DFVariable& variable){
        auto ret = m_varmap.insert(std::pair(varname, variable));
        bool succ = std::get<1>(ret);
        if(succ){
            m_varnames.push_back(varname);
        }
        return succ;
    }

    size_t length() const {
        return m_varmap.size();
    }

    const std::vector<std::string>& varnames() const{

        return m_varnames;
    }


    DFVariable get(const std::string& varname){
        return m_varmap.at(varname);
    }

    DFVariable get(const int id){
        return m_varmap.at(id2varname(id));
    }

    DFVariable pop(const std::string varname){
        DFVariable result(m_varmap.at(varname));
        eraseName(varname);
        m_varmap.erase(varname);
        return result;
    }

    DFVariable pop(const size_t id){
        return pop(id2varname(id));
    }

private:
    std::map<std::string, util::data::DFVariable> m_varmap;
    std::vector<std::string> m_varnames; // alternative: use a map std::string -> id
    //std::vector<util::data::DFVariable> m_vars;

    inline std::string id2varname(const int id){
        return m_varnames.at(id);
    }

    inline void eraseName(const std::string& name){
        auto it = std::find(m_varnames.begin(), m_varnames.end(), name);
        m_varnames.erase(it);
    }
};


util::data::Workspace::Workspace(const std::vector<std::string> &varnames, const std::vector<DFVariable> &variables)
    : m_wsImpl(std::make_unique<WS_Impl>(varnames, variables))
{}

util::data::Workspace::~Workspace(){}

bool util::data::Workspace::addEntry(const std::string& varname, const util::data::DFVariable& variable){
    return m_wsImpl->addEntry(varname, variable);
}

util::data::DFVariable util::data::Workspace::operator[](const std::string& variableName) const {
    return m_wsImpl->get(variableName);
}

util::data::DFVariable util::data::Workspace::operator[](const size_t id) const {
    return m_wsImpl->get(id);
}

util::data::DFVariable util::data::Workspace::pop(const std::string &variableName)
{
    return m_wsImpl->pop(variableName);
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

#include <cppgp/util/workspace.hpp>
#include <cppgp/util/datafile_conv.hpp>

#include <Eigen/Dense>
#include <gtest/gtest.h>
#include <gmock/gmock.h>


TEST(util_workspace, variable)
{
    auto var1 = std::make_shared<double>(2.0);
    std::string t_var1 = "double";
    auto var2 = std::make_shared<Eigen::MatrixXd>(Eigen::MatrixXd::Identity(2, 3));
    std::string t_var2 = "EMatrix";

    util::data::DFVariable v1(t_var1, std::static_pointer_cast<void>(var1));

    EXPECT_EQ(v1.typeIdentifier(), t_var1);
    EXPECT_TRUE(v1.isType(t_var1));
    EXPECT_FALSE(v1.isType(t_var2));
    EXPECT_EQ(v1.rawDataPtr(), var1);
}

TEST(util_workspace, retrieve)
{
    auto var1 = std::make_shared<double>(2.0);
    std::string t_var1 = "double";
    auto id = Eigen::MatrixXd::Identity(2, 3);
    auto var2 = std::make_shared<Eigen::MatrixXd>(id);
    std::string t_var2 = "EMatrix";

    util::data::DFVariable v1(t_var1, std::static_pointer_cast<void>(var1));
    util::data::DFVariable v2(t_var2, std::static_pointer_cast<void>(var2));

    EXPECT_EQ(*util::data::retrieve<double>(v1, t_var1), 2.0);
    EXPECT_EQ(*util::data::retrieve<double>(v1), 2.0);
    EXPECT_EQ(*util::data::retrieve<Eigen::MatrixXd>(v2, t_var2), id);
    EXPECT_EQ(*util::data::retrieve<Eigen::MatrixXd>(v2), id);
}

void createWorkspaceData(std::vector<std::string>& names, std::vector<util::data::DFVariable>& vars){
    names.clear();
    vars.clear();
    names.push_back("var1");
    names.push_back("v2");
    names.push_back("newvariable3");
    vars.push_back(util::data::DFVariable("double", std::make_shared<double>(5.9)));
    vars.push_back(util::data::DFVariable("uint", std::make_shared<unsigned int>(5)));
    vars.push_back(util::data::DFVariable("string", std::make_shared<std::string>("variablecontent")));
}

TEST(util_workspace, workspace_create){
    std::vector<std::string> names;
    std::vector<util::data::DFVariable> vars;
    createWorkspaceData(names, vars);
    util::data::Workspace ws(names, vars);

    auto res = ws.getVariableNames();
    for(size_t i = 0; i < res.size(); ++i){
        EXPECT_EQ(names[i], res[i]);
    }
}

TEST(util_workspace, workspace_length){
    std::vector<std::string> names;
    std::vector<util::data::DFVariable> vars;
    createWorkspaceData(names, vars);

    util::data::Workspace ws(names, vars);

    EXPECT_EQ(ws.length(), 3);
}

TEST(util_workspace, workspace_add){
    std::vector<std::string> names;
    std::vector<util::data::DFVariable> vars;
    createWorkspaceData(names, vars);

    util::data::Workspace ws(names, vars);
    ws.addEntry("newvar", util::data::DFVariable("string", std::make_shared<std::string>("dat2")));
    auto resvnames = ws.getVariableNames();

    ASSERT_EQ(ws.length(), 4);
    EXPECT_EQ(resvnames[0], names[0]);
    EXPECT_EQ(resvnames[1], names[1]);
    EXPECT_EQ(resvnames[2], names[2]);
    EXPECT_EQ(resvnames[3], "newvar");
}

TEST(util_workspace, workspace_getVarnames){
    std::vector<std::string> names;
    std::vector<util::data::DFVariable> vars;
    createWorkspaceData(names, vars);
    util::data::Workspace ws(names, vars);

    auto varnames = ws.getVariableNames();

    ASSERT_EQ(names.size(), varnames.size());
    EXPECT_EQ(varnames[0], names[0]);
    EXPECT_EQ(varnames[1], names[1]);
    EXPECT_EQ(varnames[2], names[2]);
}

inline bool isEqualDFVar(const util::data::DFVariable& v1, const util::data::DFVariable& v2){
    return v1.isType(v2.typeIdentifier()) && v1.rawDataPtr() == v2.rawDataPtr();
}

TEST(util_workspace, workspace_access){
    std::vector<std::string> names;
    std::vector<util::data::DFVariable> vars;
    createWorkspaceData(names, vars);
    util::data::Workspace ws(names, vars);

    auto var1 = ws[names[0]];
    auto var2 = ws[names[1]];
    auto var3 = ws[names[2]];

    auto var4 = ws[0];
    auto var5 = ws[1];
    auto var6 = ws[2];

    EXPECT_TRUE(isEqualDFVar(var1, vars[0]));
    EXPECT_TRUE(isEqualDFVar(var2, vars[1]));
    EXPECT_TRUE(isEqualDFVar(var3, vars[2]));
    EXPECT_TRUE(isEqualDFVar(var4, vars[0]));
    EXPECT_TRUE(isEqualDFVar(var5, vars[1]));
    EXPECT_TRUE(isEqualDFVar(var6, vars[2]));
    EXPECT_THROW(ws[4], std::out_of_range);
    EXPECT_THROW(ws["fantasy"], std::out_of_range);
}

TEST(util_workspace, workspace_popName){
    std::vector<std::string> names;
    std::vector<util::data::DFVariable> vars;
    createWorkspaceData(names, vars);
    util::data::Workspace ws(names, vars);

    auto res = ws.pop(names[0]);
    ASSERT_EQ(ws.length(), 2);
    EXPECT_TRUE(isEqualDFVar(res, vars[0]));
    EXPECT_EQ(ws.getVariableNames()[0], names[1]);
    EXPECT_EQ(ws.getVariableNames()[1], names[2]);
    EXPECT_TRUE(isEqualDFVar(ws[0], vars[1]));
    EXPECT_TRUE(isEqualDFVar(ws[1], vars[2]));

    res = ws.pop(names[2]);
    ASSERT_EQ(ws.length(), 1);
    EXPECT_TRUE(isEqualDFVar(res, vars[2]));
    EXPECT_EQ(ws.getVariableNames()[0], names[1]);
    EXPECT_TRUE(isEqualDFVar(ws[0], vars[1]));
}

TEST(util_workspace, workspace_popID){
    std::vector<std::string> names;
    std::vector<util::data::DFVariable> vars;
    createWorkspaceData(names, vars);
    util::data::Workspace ws(names, vars);

    auto res = ws.pop(0);
    ASSERT_EQ(ws.length(), 2);
    EXPECT_TRUE(isEqualDFVar(res, vars[0]));
    EXPECT_EQ(ws.getVariableNames()[0], names[1]);
    EXPECT_EQ(ws.getVariableNames()[1], names[2]);
    EXPECT_TRUE(isEqualDFVar(ws[0], vars[1]));
    EXPECT_TRUE(isEqualDFVar(ws[1], vars[2]));

    res = ws.pop(1);
    ASSERT_EQ(ws.length(), 1);
    EXPECT_TRUE(isEqualDFVar(res, vars[2]));
    EXPECT_EQ(ws.getVariableNames()[0], names[1]);
    EXPECT_TRUE(isEqualDFVar(ws[0], vars[1]));
}

TEST(util_workspace, workspace_empty){
    std::vector<std::string> names;
    std::vector<util::data::DFVariable> vars;
    createWorkspaceData(names, vars);

    std::vector<std::string> in1;
    std::vector<util::data::DFVariable> in2;
    util::data::Workspace ws(in1, in2);

    EXPECT_EQ(ws.length(), 0);
    ws.addEntry(names[0], vars[0]);
    EXPECT_EQ(ws.length(), 1);
    ws.pop();
    EXPECT_EQ(ws.length(), 0);
    EXPECT_EQ(ws.getVariableNames(), std::vector<std::string>(0));
    EXPECT_THROW(ws[0], std::out_of_range);
}


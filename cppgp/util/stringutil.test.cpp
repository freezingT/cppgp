#include <cppgp/util/stringutil.hpp>

#include <gtest/gtest.h>


TEST(tokenize, tokenize_string){
    std::string s = "Tokenize a string in C++";
    std::vector<std::string> T;

    util::string::tokenize(T, s);

    EXPECT_EQ(T.size(), 5);
    EXPECT_EQ(T[0], "Tokenize");
    EXPECT_EQ(T[1], "a");
    EXPECT_EQ(T[2], "string");
    EXPECT_EQ(T[3], "in");
    EXPECT_EQ(T[4], "C++");
}

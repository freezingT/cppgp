#include <cppgp/util/stringutil.hpp>

#include <regex>

void util::string::tokenize(
        std::vector<std::string>& tokens,
        const std::string& str,
        const std::string& regex){

    std::regex reg(regex);

    std::sregex_token_iterator iter(str.begin(), str.end(), reg, -1);
    std::sregex_token_iterator end;

    tokens = std::vector<std::string>(iter, end);
}

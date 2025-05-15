#pragma once

#include <string>
#include <vector>

namespace util::string {

    /**
     * \brief Tokenize a string.
     *
     * Tokenize a given string and store the results in a std::vector. A regular
     * expression can be given to tokenize on other symbols than spaces.
     *
     * \param tokens The vector in which the tokens are stored.
     * \param str The string to be tokenized.
     * \param regex Regular expression that describes where to tokenize. Default is spaces.
     */
    void tokenize(std::vector<std::string>& tokens, const std::string& str, const std::string& regex="\\s+");

}

#pragma once

#include <vector>
#include <string>

namespace util::io {

    /**
     * \brief Read data from a text file.
     *
     * Read data from a text file  and store it in nested std::vectors.
     * The outer std::vector stores std::vectors of doubles that contain the
     * rows in the text file. The data in the rows is separated by spaces.
     *
     *
     * \param M Container in which the data gets written. All previous data gets cleared.
     * \param filename The filename from which the data is read.
     */
    void read_textfile(std::vector<std::vector<double>>& M, const std::string& filename);

    /**
     *
     */
    std::string get_project_path();
}

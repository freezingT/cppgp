#pragma once

#include <Eigen/Eigen>
#include <vector>
#include <string>


namespace util::io {


    /**
     * \brief Get the absolute path to the current project folder as string.
     * Get the string that represents the absolute path to the current project folder.
     *
     * \return The string containing the absolute path.
     */
    std::string get_project_path();


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
     * \brief Load matrix data from a textfile.
     *
     * Read data from a text file and store it in an Eigen::MatrixXd.
     * Each line in a textfile is a row in the matrix, the entries in
     * each line are separated by spaces.
     *
     * \param M matrix in which the data gets written. The matrix
     * is resized and previous data is cleared.
     * \param filename The filename from which the data is read.
     */
    void load_matrix_from_textfile(Eigen::MatrixXd& M, const std::string& rel_path);

}

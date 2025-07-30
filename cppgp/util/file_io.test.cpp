#include <cppgp/util/file_io.hpp>
#include <vector>
#include <string>

#include <gtest/gtest.h>
#include <fstream>


TEST(read_series_data, Read_some_data_from_text_file){

    std::vector<std::vector<double>> M;
    std::string projpath = util::io::get_project_path();
    std::string filename = projpath+"/data/testing/matrix_01.txt";

    util::io::read_textfile(M, filename);

    EXPECT_EQ(M.size(), 3);
    EXPECT_EQ(M[0].size(), 6);

    for(int row = 1; row <= 3; ++row){
        for(int column = 1; column <= 5; ++column){
            EXPECT_EQ(M[row-1][column-1], row*10+column);
        }
    }
}

TEST(read_matrix, Read_matrix_from_textfile){
    Eigen::MatrixXd M;
    std::string rel_path = "/data/testing/matrix_01.txt";
    util::io::load_matrix_from_textfile(M, rel_path);

    for(int row = 1; row <= 3; ++row){
        for(int column = 1; column <= 5; ++column){
            EXPECT_EQ(M(row-1, column-1), row*10+column);
        }
    }
}

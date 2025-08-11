#include <cppgp/util/datafile.hpp>
#include <cppgp/util/datafile_conv.hpp>

#include <Eigen/Dense>
#include <gtest/gtest.h>
#include <gmock/gmock.h>


TEST(util_datafile, tmp){

}


/**
 *
 *  Ref    description                           data type       length[chars]
 * ----------------------------------------------------------------------------
 * Header section - general information                                       7
 *  [1]    version number                        string            6
 *
 * Header section - variable table 1                                  11+N*37+2
 *  [2]    number of variables in vartable       int              10
 *  [3]    variable name, max 16 chars           string           16
 *  [4]    position in file where data starts    long long int    19
 *  [5]    section end symbol                    string            1
 *
 * Data section 1
 *  [6]    data type identifier                  string           ?
 *  [7]    number of elements                    int              ?
 *  [8]    serialization data                    string           ?
 *
 *
 *  |[1]-| |--[2]---| |-----[3]------| |-------[4]-------| ...                                                                       [5]|-[6]-| [7]|-----------[8]----------| [5]
 * "00.1.0 0000000003 var1xxxxxxxxxxxx 0000000000000000131 var2xxxxxxxxxxxx 0000000000000000000 var3xxxxxxxxxxxx 0000000000000000000 ^ E:Vector 7 1.0 2.0 3.0 4.0 5.0 6.0 7.0 ^ ...";
 */

std::string headerstr = "00.1.0 0000000003 var1             0000000000000000000 var1             0000000000000000000 var1             0000000000000000000 ^ ";


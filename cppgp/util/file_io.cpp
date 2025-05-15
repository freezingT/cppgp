#include <cppgp/util/file_io.hpp>

#include <cppgp/util/exceptions.hpp>
#include <cppgp/util/stringutil.hpp>

#include <iostream>
#include <fstream>
#include <algorithm>
#include <filesystem>
#include <regex>


double string_to_double(const std::string& s){
    double v = std::stod(s);
    return v;
};

void util::io::read_textfile(std::vector<std::vector<double>>& M, const std::string& filename){
    M.clear();
    std::ifstream ifs;
    std::string line;
    ifs.open(filename, std::ios::in);
    if(ifs){
        while(!ifs.eof()){
            std::getline(ifs, line);
            std::vector<std::string> strvec;
            std::vector<double> dvec;
            util::string::tokenize(strvec, line);
            dvec.resize(strvec.size());
            std::transform(strvec.begin(), strvec.end(), dvec.begin(), string_to_double);
            M.push_back(dvec);
        }
    }
    else {
        util::exceptions::throwException<util::exceptions::FileReaderError>(filename);
    }
}

std::string get_foldername(const std::filesystem::path& path){
    auto p2 = path.parent_path();
    std::string sp1 = path.string();
    sp1 = std::regex_replace(sp1, std::regex(p2.string()+"/"), "");
    return sp1;
}

std::string util::io::get_project_path(){
    namespace fs = std::filesystem;
    fs::path cpath = fs::current_path();
    std::string pathname = "";

    while(true){ // will end at some point in the root directory
        std::string foldername = get_foldername(cpath);
        if(!foldername.compare("cppgp") && fs::exists(cpath/fs::path("build"))){
            pathname = cpath.string();
            break;
        }
        else {
            if(cpath.has_parent_path() && cpath != cpath.root_path()){
                cpath = cpath.parent_path();
            }
            else {
                break; // reached root directory, stop searching
            }
        }
    }
    return pathname;
}

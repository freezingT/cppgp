
target_sources(libgp PRIVATE
    datafile_conv.hpp
    datafile_conv.cpp
    datafile.hpp
    datafile.cpp
    datafile_io.hpp
    datafile_io.cpp
    exceptions.hpp
    file_io.hpp
    file_io.cpp
    stringutil.hpp
    stringutil.cpp
    observer.hpp
    observer.cpp
    prototype.hpp
    prototype.cpp
    workspace.hpp
    workspace.cpp
)

create_test(test_exception exceptions.test.cpp)
create_test(test_file_io file_io.test.cpp)
create_test(test_stringutil stringutil.test.cpp)
create_test(test_observer observer.test.cpp)
create_test(test_datafile datafile.test.cpp)
create_test(test_datafile_io datafile_io.test.cpp)
create_test(test_workspace workspace.test.cpp)

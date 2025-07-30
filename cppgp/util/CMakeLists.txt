
target_sources(libgp PRIVATE
    exceptions.hpp
    file_io.hpp
    file_io.cpp
    stringutil.hpp
    stringutil.cpp
    observer.hpp
    observer.cpp
    prototype.hpp
    prototype.cpp
)

create_test(test_exception exceptions.test.cpp)
create_test(test_file_io file_io.test.cpp)
create_test(test_stringutil stringutil.test.cpp)
create_test(test_observer observer.test.cpp)


target_sources(libgp PRIVATE
    gaussianprocess.hpp
    gaussianprocess.cpp
    gpdata.hpp
    gpdata.cpp
)

create_test(test_gpdata gpdata.test.cpp)

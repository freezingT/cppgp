
target_sources(libgp PRIVATE
    distance.cpp
    distance.hpp
)


create_test(dist2_test distance.test.cpp)
target_link_libraries(dist2_test Eigen3::Eigen)

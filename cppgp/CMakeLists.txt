

add_library(libgp SHARED)
#target_link_libraries(libgp PRIVATE Eigen3::Eigen AutoDiff::AutoDiff)
target_link_libraries(libgp PRIVATE Eigen3::Eigen)

set (gtest_disable_pthreads on)

add_subdirectory(math)
add_subdirectory(util)
add_subdirectory(gp)
add_subdirectory(kernels)
add_subdirectory(examples)
add_subdirectory(experimentation)

#link_libraries(gp libutil)

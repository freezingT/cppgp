

add_library(libgp SHARED)
target_link_libraries(libgp PRIVATE Eigen3::Eigen AutoDiff::AutoDiff)

add_subdirectory(math)
add_subdirectory(gp)
add_subdirectory(kernels)
add_subdirectory(util)
add_subdirectory(experimentation)

#link_libraries(gp libutil)

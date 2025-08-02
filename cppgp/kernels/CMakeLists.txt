
set(COVARIANCE_FUNCTIONS
    covfun_rbf.hpp
    covfun_rbf.cpp
)

target_sources(libgp PRIVATE
    covfun.hpp
    covfun.cpp
    gpkernel.hpp
    gpkernel.cpp
    ${COVARIANCE_FUNCTIONS}
)

create_test(test_gpkernel gpkernel.test.cpp)

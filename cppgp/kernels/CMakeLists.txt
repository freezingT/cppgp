
set(COVARIANCE_FUNCTIONS
    rbf_covfun.hpp
    rbf_covfun.cpp
)

target_sources(libgp PRIVATE
    covfun.hpp
    covfun.cpp
    gpkernel.hpp
    gpkernel.cpp
    #rbfkernel.hpp
    #rbfkernel.cpp
    ${COVARIANCE_FUNCTIONS}
)

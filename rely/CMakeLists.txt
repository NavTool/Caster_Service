# add_subdirectory(rtklib-2.4.3-b34)
#libevent option
set(EVENT__LIBRARY_TYPE STATIC)
set(EVENT__DISABLE_OPENSSL ON)
set(EVENT__DISABLE_BENCHMARK ON)
set(EVENT__DISABLE_TESTS ON)
set(EVENT__DISABLE_REGRESS ON)
set(EVENT__DISABLE_SAMPLES ON)
add_subdirectory(libevent)


#yaml-cpp option
set(YAML_BUILD_SHARED_LIBS OFF)
set(YAML_CPP_BUILD_TOOLS OFF)
add_subdirectory(yaml-cpp)
# add_subdirectory(libssh2-libssh2-1.11.0/)
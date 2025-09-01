#libevent option
# set(EVENT_LIBRARY_TYPE STATIC)
set(EVENT__LIBRARY_TYPE STATIC CACHE STRING "Set library type to STATIC" FORCE)
set(EVENT__DISABLE_OPENSSL ON CACHE BOOL "force set this option" FORCE)
set(EVENT__DISABLE_BENCHMARK ON CACHE BOOL "force set this option" FORCE)
set(EVENT__DISABLE_TESTS ON CACHE BOOL "force set this option" FORCE)
set(EVENT__DISABLE_REGRESS ON CACHE BOOL "force set this option" FORCE)
set(EVENT__DISABLE_SAMPLES ON CACHE BOOL "force set this option" FORCE)
add_subdirectory(libevent)

#hiredis option
set(BUILD_SHARED_LIBS OFF CACHE BOOL "force set this option"  FORCE)
set(DISABLE_TESTS ON CACHE BOOL "force set this option"  FORCE)
add_subdirectory(hiredis)

#yaml-cpp option
set(YAML_BUILD_SHARED_LIBS OFF CACHE BOOL "force set this option"  FORCE)
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "force set this option"  FORCE)
add_subdirectory(yaml-cpp)

# add_subdirectory(libssh2-libssh2-1.11.0/)

# #json
# set(JSON_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/json/include")

# #spdlog
# set(SPDLOG_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/spdlog/include")


add_subdirectory(rtklib)
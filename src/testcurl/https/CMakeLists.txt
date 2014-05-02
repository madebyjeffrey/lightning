
cmake_minimum_required(VERSION 2.8)

include_directories(${GNUTLS_INCLUDE_DIR})

#add_definitions("-fprofile-arcs -ftest-coverage")

configure_file(
  "keypath.h.in"
  "${PROJECT_BINARY_DIR}/keypath.h"
)

macro(define_tests prefix)
  #message("Arg ${ARGN}")
  foreach(test ${ARGN})

    add_executable(${prefix}${test} "${test}.cpp")
    target_link_libraries(${prefix}${test} microhttpd ${GNUTLS_LIBRARIES} ${GCRYPT_LIBRARIES} ${CURL_LIBRARIES} "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/5.1/lib/darwin/libclang_rt.profile_osx.a")

    add_test(NAME ${prefix}${test} COMMAND ${prefix}${test})
  endforeach()
endmacro()

macro(define_teststls prefix)
  #message("Arg ${ARGN}")
  foreach(test ${ARGN})

    add_executable(${prefix}${test} "${test}.cpp" "tls_test_common.cpp" "../curl_version_check.cpp")
    target_link_libraries(${prefix}${test} microhttpd ${GNUTLS_LIBRARIES} ${GCRYPT_LIBRARIES} ${CURL_LIBRARIES} "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/5.1/lib/darwin/libclang_rt.profile_osx.a")

    add_test(NAME ${prefix}${test} COMMAND ${prefix}${test})
  endforeach()
endmacro()

define_teststls(https
  test_empty_response test_https_get test_https_get_parallel
  test_https_get_parallel_threads test_https_get_select
  test_https_multi_daemon test_https_session_info
  test_https_sni test_https_time_out
)

define_teststls(tls
  test_tls_authentication
   test_tls_options
)

# test_tls_extensions  ## won't compile

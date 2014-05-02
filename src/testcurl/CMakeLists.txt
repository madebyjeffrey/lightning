
cmake_minimum_required(VERSION 2.8)

include_directories(${GNUTLS_INCLUDE_DIR})

#add_definitions("-fprofile-arcs -ftest-coverage")

macro(define_tests prefix)
  #message("Arg ${ARGN}")
  foreach(test ${ARGN})

    add_executable(${prefix}${test} "${test}.cpp")
    target_link_libraries(${prefix}${test} microhttpd ${GNUTLS_LIBRARIES} ${GCRYPT_LIBRARIES} ${CURL_LIBRARIES} "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/5.1/lib/darwin/libclang_rt.profile_osx.a")

    add_test(NAME ${prefix}${test} COMMAND ${prefix}${test})
  endforeach()
endmacro()

define_tests(curl
  test_get test_get_chunked test_get_response_cleanup
  test_get_sendfile test_iplimit test_large_put
  test_long_header test_options test_parse_cookies
  test_post test_post_loop test_postform
  test_process_arguments test_process_headers
  test_put test_put_chunked test_quiesce
  test_start_stop test_termination test_timeout
  test_urlparse
)

add_subdirectory(https)

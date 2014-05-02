
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

define_tests(zzuf test_get test_get_chunked test_long_header test_post test_post_form test_put
             test_put_chunked test_put_large
)

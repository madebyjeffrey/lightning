
cmake_minimum_required(VERSION 2.8)

include_directories(${GNUTLS_INCLUDE_DIR})

set(LIBS ${LIBS} "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/5.1/lib/darwin/libclang_rt.profile_osx.a")

if(MAGIC_FOUND)
  include_directories(${MAGIC_INCLUDE_DIR})
  set(LIBS ${LIBS} ${MAGIC_LIBRARIES})
endif()

macro(define_examples prefix)
  #message("Arg ${ARGN}")
  foreach(example ${ARGN})
    add_executable(${prefix}${example} "${example}.cpp")
    target_link_libraries(${prefix}${example} microhttpd ${GNUTLS_LIBRARIES} ${GCRYPT_LIBRARIES} ${CURL_LIBRARIES} ${LIBS})
  endforeach()
endmacro()

define_examples("examples"
  authorization_example
  benchmark
  benchmark_https
  demo
  digest_auth_example
  dual_stack_example
  fileserver_example
  fileserver_example_dirs
  fileserver_example_external_select
  https_fileserver_example
  minimal_example
  minimal_example_comet
  post_example
  querystring_example
  refuse_post_example)


cmake_minimum_required(VERSION 2.8)

include_directories(${GNUTLS_INCLUDE_DIR})

if(BAUTH_SUPPORT)
  set(add_src ${add_src} "basicauth.cpp")
  set(add_src ${add_src} "base64.cpp")
endif()

if(DAUTH_SUPPORT)
  set(add_src ${add_src} "digestauth.cpp")
  set(add_src ${add_src} "md5.cpp")
endif()

if(POSTPROCESSOR_SUPPORT)
  set(add_src ${add_src} "postprocessor.cpp")
endif()

if(HTTPS)
  set(add_src ${add_src} "connection_https.cpp")
endif()

if(NOT HAVE_TSEARCH)
  set(add_src ${add_src} "tsearch.cpp")
endif()


add_library(microhttpd
  connection.cpp connection.h
  reason_phrase.cpp reason_phrase.h
  daemon.cpp
  internal.cpp internal.h
  memorypool.cpp memorypool.h
  response.cpp response.h
  ${add_src}
)

message("GNUTLS LIBRARIES: ${GNUTLS_LIBRARIES}")

add_executable(test_daemon test_daemon.cpp)
target_link_libraries(test_daemon microhttpd ${GNUTLS_LIBRARIES} ${GCRYPT_LIBRARIES})

add_test(NAME test_daemon COMMAND test_daemon)

if(POSTPROCESSOR_SUPPORT)
  add_executable(test_postprocessor test_postprocessor.cpp)
  target_link_libraries(test_postprocessor microhttpd ${GNUTLS_LIBRARIES} ${GCRYPT_LIBRARIES})
  add_executable(test_postprocessor_amp test_postprocessor_amp.cpp)
  target_link_libraries(test_postprocessor_amp microhttpd ${GNUTLS_LIBRARIES} ${GCRYPT_LIBRARIES})
  add_executable(test_postprocessor_large test_postprocessor_large.cpp)
  target_link_libraries(test_postprocessor_large microhttpd ${GNUTLS_LIBRARIES} ${GCRYPT_LIBRARIES})

  add_test(NAME test_postprocessor COMMAND test_postprocessor)
  add_test(NAME test_postprocessor_amp COMMAND test_postprocessor_amp)
  add_test(NAME test_postprocessor_large COMMAND test_postprocessor_large)
endif()


set(CMAKE_CXX_FLAGS "${MHD_LIB_CPPFLAGS} -DBUILDING_MHD_LIB=1")
set(CMAKE_CFLAGS "${MHD_LIB_CFLAGS}")
set(CMAKE_LD_FLAGS "${MHD_LIB_LDFLAGS} ${W32_MHD_LIB_LDFLAGS}
  -version-info 0.9.34")

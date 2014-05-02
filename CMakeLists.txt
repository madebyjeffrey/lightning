cmake_minimum_required(VERSION 2.8)

enable_testing()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

INCLUDE (${CMAKE_ROOT}/Modules/CheckIncludeFileCXX.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckIncludeFiles.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckLanguage.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckCXXSymbolExists.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckStructHasMember.cmake)

set(MHD_REQ_CURL_VERSION "7.16.4" CACHE STRING "required cURL version to run tests")
set(MHD_REQ_CURL_OPENSSL_VERSION "0.9.8" CACHE STRING "required cURL SSL version to run tests")
set(MHD_REQ_CURL_GNUTLS_VERSION "2.8.6" CACHE STRING "gnuTLS lib version - used in conjunction with cURL")
set(MHD_REQ_CURL_NSS_VERSION "3.12.0" CACHE STRING "NSS lib version - used in conjunction with cURL")

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "DEBUG")
endif()

macro(check_curl_feature feature resultvar)
  execute_process(
    COMMAND curl-config --features
    COMMAND grep -e "^${feature}$"
    RESULT_VARIABLE resultvar
    OUTPUT_QUIET
    ERROR_QUIET
  )

  if (resultvar EQUAL 0)
    message(STATUS "Looking for CURL feature ${feature} - found")
    set(resultvar 1 cache bool "Have feature ${feature}")
  else()
    message(STATUS "Looking for CURL feature ${feature} - not found")
    set(resultvar 0 cache bool "Have feature ${feature}")
  endif()
endmacro()

macro(check_curl_protocol protocol resultvar)
  execute_process(
    COMMAND curl-config --protocols
    COMMAND grep -e "^${protocol}$"
    RESULT_VARIABLE resultvar
    OUTPUT_QUIET
    ERROR_QUIET
  )

  if (resultvar EQUAL 0)
    message(STATUS "Looking for CURL protocol ${protocol} - found")
    set(resultvar 1 cache bool "Have protocol ${protocol}")
  else()
    message(STATUS "Looking for CURL protocol ${protocol} - not found")
    set(resultvar 0 cache bool "Have protocol ${protocol}")
  endif()
endmacro()

#add_definitions("-std=c++11 -Wall -Wextra")
add_definitions("-std=c++11 -w")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_VERBOSE_MAKEFILE off)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 9)
set(VERSION_PATCH 34)

option(HTTPS "HTTPS" ON)
project(microhttpd)

option(BAUTH_SUPPORT "Basic Authentication support" ON)
option(DAUTH_SUPPORT "Digest Authentication support" ON)
option(POSTPROCESSOR_SUPPORT "PostProcessor functionality" ON)
option(EPOLL_SUPPORT "epoll support" OFF) # test?
option(HAVE_MESSAGES "Support error messages" ON)

check_language(CXX)
if (CMAKE_CXX_COMPILER)
  enable_language(CXX)
else()
  message(FATAL_ERROR "C++ Cannot be enabled")
endif()

#set package info
set(PACKAGE "lightning")
set(PACKAGE_BUGREPORT "jptdrake@gmail.com")
set(PACKAGE_NAME "Lightning HTTPD Server")
set(PACKAGE_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
set(PACKAGE_NAME "${PACKAGE_NAME} ${PACKAGE_VERSION}")
set(PACKAGE_TARNAME "${PACKAGE}-${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.tar.gz")
set(PACKAGE_URL "no url yet")


# configure parameters
check_include_file_cxx("arpa/inet.h" HAVE_ARPA_INET_H)
check_cxx_symbol_exists("accept4" "sys/types.h" HAVE_ACCEPT4)
check_cxx_symbol_exists("clock_gettime" "time.h" HAVE_CLOCK_GETTIME)
check_cxx_symbol_exists("SOCK_NONBLOCK" "sys/types.h;sys/socket.h" HAVE_DECL_SOCK_NONBLOCK)
check_cxx_symbol_exists("TCP_CORK" "sys/types.h;sys/socket.h;netinet/tcp.h" HAVE_DECL_TCP_CORK)
check_cxx_symbol_exists("TCP_NOPUSH" "sys/types.h;sys/socket.h;netinet/tcp.h" HAVE_DECL_TCP_NOPUSH)
check_include_file_cxx("dlfcn.h" HAVE_DLFCN_H)
check_include_file_cxx("errno.h" HAVE_ERRNO_H)
check_include_file_cxx("fcntl.h" HAVE_FCNTL_H)
check_include_file_cxx("gcrypt.h" HAVE_GCRYPT_H)
check_include_file_cxx("gnutls/gnutls.h" HAVE_GNUTLS_GNUTLS_H)
check_include_file_cxx("inttypes.h" HAVE_INTTYPES_H)
check_include_file_cxx("limits.h" HAVE_LIMITS_H)
check_include_file_cxx("locale.h" HAVE_LOCALE_H)
check_include_file_cxx("magic.h" HAVE_MAGIC_H)
check_include_file_cxx("math.h" HAVE_MATH_H)
check_cxx_symbol_exists("memmem" "string.h" HAVE_MEMMEM)
check_include_file_cxx("memory.h" HAVE_MEMORY_H)
check_include_file_cxx("netdb.h" HAVE_NETDB_H)
check_include_file_cxx("netinet/in.h" HAVE_NETINET_IN_H)
check_include_file_cxx("netinet/tcp.h" HAVE_NETINET_TCP_H)
check_include_file_cxx("poll.h" HAVE_POLL_H)
check_include_file_cxx("pthread.h" HAVE_PTHREAD_H)
if(HAVE_PTHREAD_H)
  set(MHD_USE_POSIX_THREADS 1)
endif()
check_cxx_symbol_exists("PTHREAD_PRIO_INHERIT" "pthread.h" HAVE_PTHREAD_PRIO_INHERIT)
if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  message(STATUS "Not checking for pthread_setname_np because it only has one parameter on Mac.")
  check_cxx_symbol_exists("pthread_setname_np" "pthread.h" HAVE_PTHREAD_SETNAME_NP)
endif()
check_include_file_cxx("search.h" HAVE_SEARCH_H)
check_cxx_symbol_exists("tsearch" "search.h" HAVE_TSEARCH)
check_struct_has_member("struct sockaddr_in" "sin_len" "netinet/in.h" HAVE_SOCKADDR_IN_SIN_LEN)

check_include_file_cxx("stdint.h" HAVE_STDINT_H)
check_include_file_cxx("stdio.h" HAVE_STDIO_H)
check_include_file_cxx("stdlib.h" HAVE_STDLIB_H)
check_include_file_cxx("strings.h" HAVE_STRINGS_H)
check_include_file_cxx("string.h" HAVE_STRING_H)
check_include_file_cxx("sys/mman.h" HAVE_SYS_MMAN_H)
check_include_file_cxx("sys/msg.h" HAVE_SYS_MSG_H)
check_include_file_cxx("sys/select.h" HAVE_SYS_SELECT_H)
check_include_file_cxx("sys/socket.h" HAVE_SYS_SOCKET_H)
check_include_file_cxx("sys/stat.h" HAVE_SYS_STAT_H)
check_include_file_cxx("sys/time.h" HAVE_SYS_TIME_H)
check_include_file_cxx("sys/types.h" HAVE_SYS_TYPES_H)
check_include_file_cxx("time.h" HAVE_TIME_H)
check_include_file_cxx("unistd.h" HAVE_TUNISTD_H)
check_include_file_cxx("winsock2.h" HAVE_WINSOCK2_H)
check_include_file_cxx("ws2tcpip.h" HAVE_WS2TCPIP_H)

check_include_files("assert.h;limits.h;signal.h;stdlib.h;ctype.h;locale.h;stdarg.h;string.h;errno.h; math.h;stddef.h;time.h;float.h;setjmp.h;stdio.h" STDC_HEADERS)

find_package(curl)

if (CURL_FOUND)
  set(HAVE_LIBCURL 1)

  check_curl_feature("AsynchDNS" LIBCURL_FEATURE_ASYNCHDNS)
  check_curl_feature("IDN" LIBCURL_FEATURE_IDN)
  check_curl_feature("IPv6" LIBCURL_FEATURE_IPV6)
  check_curl_feature("KRB4" LIBCURL_FEATURE_KRB4)
  check_curl_feature("libz" LIBCURL_FEATURE_LIBZ)
  check_curl_feature("NTLM" LIBCURL_FEATURE_NTLM)
  check_curl_feature("SSL" LIBCURL_FEATURE_SSL)
  check_curl_feature("SSPI" LIBCURL_FEATURE_SSPI)
  check_curl_feature("NTLM" LIBCURL_FEATURE_NTLM)
  check_curl_feature("NTLM_WB" LIBCURL_FEATURE_NTLM_WB)

  check_curl_protocol("DICT" LIBCURL_PROTOCOL_DICT)
  check_curl_protocol("FILE" LIBCURL_PROTOCOL_FILE)
  check_curl_protocol("FTP" LIBCURL_PROTOCOL_FTP)
  check_curl_protocol("FTPS" LIBCURL_PROTOCOL_FTPS)
  check_curl_protocol("HTTP" LIBCURL_PROTOCOL_HTTP)
  check_curl_protocol("HTTPS" LIBCURL_PROTOCOL_HTTPS)
  check_curl_protocol("IMAP" LIBCURL_PROTOCOL_IMAP)
  check_curl_protocol("LDAP" LIBCURL_PROTOCOL_LDAP)
  check_curl_protocol("POP3" LIBCURL_PROTOCOL_POP3)
  check_curl_protocol("RTSP" LIBCURL_PROTOCOL_RTSP)
  check_curl_protocol("SMTP" LIBCURL_PROTOCOL_SMTP)
  check_curl_protocol("TELNET" LIBCURL_PROTOCOL_TELNET)
  check_curl_protocol("TFTP" LIBCURL_PROTOCOL_TFTP)
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  message("System is MacOS X")
  set(OSX 1)
  set(AC_APPLE_UNIVERSAL_BUILD 1)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  message("System  is Linux")
  set(LINUX 1)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "NetBSD") # not tested
  message("System  is NetBSD")
  set(NETBSD 1)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "OpenBSD") # not tested
  message("System  is OpenBSD")
  set(OPENBSD 1)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD") # not tested
  message("System is FreeBSD")
  set(FREEBSD 1) # not used?
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Cygwin") # not tested
  message("System is Cygwin")
  set(CYGWIN 1)
elseif(MINGW) # not tested
  message("System is MINGW")
endif()

find_package(magic)

if(HTTPS)
  find_package(gnutls)
  find_package(gcrypt)
  if(GNUTLS_FOUND AND GCRYPT_FOUND)
    set(HTTPS_SUPPORT 1)
    include_directories(${GNUTLS_INCLUDE_DIR} ${GCRYPT_INCLUDE_DIR})
    add_definitions(${GNUTLS_DEFINITIONS})
  elseif(NOT GNUTLS_FOUND)
    message(FATAL_ERROR "HTTPS enabled, but GNUTLS not found.")
  endif()
endif(HTTPS)

configure_file(
  "${PROJECT_SOURCE_DIR}/MHD_config.h.in"
  "${PROJECT_BINARY_DIR}/MHD_config.h"
)
include_directories("${PROJECT_BINARY_DIR}")


add_subdirectory(src)

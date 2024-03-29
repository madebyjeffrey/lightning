# This file is part of libmicrohttpd.
# (C) 2006-2014 Christian Grothoff (and other contributing authors)
#
# libmicrohttpd is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published
# by the Free Software Foundation; either version 3, or (at your
# option) any later version.
#
# libmicrohttpd is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with libmicrohttpd; see the file COPYING.  If not, write to the
# Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.
#
#
# Process this file with autoconf to produce a configure script.
#
#
AC_PREREQ([2.60])
LT_PREREQ([2.4.0])
AC_INIT([libmicrohttpd],[0.9.34],[libmicrohttpd@gnu.org])
AM_INIT_AUTOMAKE([silent-rules] [subdir-objects])
AC_CONFIG_HEADERS([MHD_config.h])
AC_CONFIG_MACRO_DIR([m4])
AH_TOP([#define _GNU_SOURCE  1])

LIB_VERSION_CURRENT=33
LIB_VERSION_REVISION=0
LIB_VERSION_AGE=23
AC_SUBST(LIB_VERSION_CURRENT)
AC_SUBST(LIB_VERSION_REVISION)
AC_SUBST(LIB_VERSION_AGE)

if test `uname -s` = "OS/390"
then
# configure binaries for z/OS
  if test -z "$CC"
  then
    CC=`pwd`"/contrib/xcc"
    chmod +x $CC || true
  fi
  if test -z "$CPP"
  then
    CPP="c89 -E"
  fi
  if test -z "$CXXCPP"
  then
    CXXCPP="c++ -E -+"
  fi
#  _CCC_CCMODE=1
#  _C89_CCMODE=1
fi

# Checks for programs.
AC_PROG_AWK
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_CANONICAL_HOST
AM_PROG_CC_C_O
LT_INIT([win32-dll])
LT_LANG([Windows Resource])

PACKAGE_VERSION_MAJOR=${PACKAGE_VERSION%.*.*}
PACKAGE_VERSION_MINOR=${PACKAGE_VERSION%.*}; PACKAGE_VERSION_MINOR=${PACKAGE_VERSION_MINOR#*.}
PACKAGE_VERSION_SUBMINOR=${PACKAGE_VERSION#*.*.}
AC_SUBST([PACKAGE_VERSION_MAJOR])
AC_SUBST([PACKAGE_VERSION_MINOR])
AC_SUBST([PACKAGE_VERSION_SUBMINOR])
AC_CONFIG_FILES([src/microhttpd/microhttpd_dll_res.rc])

MHD_LIB_CPPFLAGS=""
MHD_LIB_CFLAGS=""
MHD_LIB_LDFLAGS=""
MHD_LIBDEPS=""

AC_ARG_WITH([threads],
   [AC_HELP_STRING([--with-threads=LIB],[choose threading library (posix, w32, auto) [auto]])],
   [], [with_threads='auto'])
test "x$with_threads" = "xwin32" && with_threads='w32'
test "x$with_threads" = "xpthreads" && with_threads='posix'

# Check for posix threads support
AX_PTHREAD([HAVE_POSIX_THREADS='yes'],[HAVE_POSIX_THREADS='no'])
AM_CONDITIONAL([HAVE_POSIX_THREADS],[test "x$HAVE_POSIX_THREADS" = "xyes"])
# Simple check for W32 threads support
AC_CHECK_HEADER([windows.h],
  [
    AC_MSG_CHECKING([for CreateThread()])
    AC_LINK_IFELSE(
      [AC_LANG_PROGRAM([#include <windows.h>], [ HANDLE h = CreateThread(NULL, 0, NULL, NULL, 0, NULL);])],
      [
        AC_MSG_RESULT([yes])
        HAVE_W32_THREADS='yes'
      ],
      [
        AC_MSG_RESULT([no])
        HAVE_W32_THREADS='no'
      ])
  ],
  [HAVE_W32_THREADS='no'])

# for pkg-config
MHD_LIBDEPS=""
# Check system type
case "$host_os" in
*darwin* | *rhapsody* | *macosx*)
     AC_DEFINE_UNQUOTED(OSX,1,[This is an OS X system])
     CFLAGS="-no-cpp-precomp -fno-common $CFLAGS"
     ;;
freebsd*)
     AC_DEFINE_UNQUOTED(SOMEBSD,1,[This is a BSD system])
     AC_DEFINE_UNQUOTED(FREEBSD,1,[This is a FreeBSD system])
     ;;
openbsd*)
     AC_DEFINE_UNQUOTED(SOMEBSD,1,[This is a BSD system])
     AC_DEFINE_UNQUOTED(OPENBSD,1,[This is an OpenBSD system])
     ;;
netbsd*)
     AC_DEFINE_UNQUOTED(SOMEBSD,1,[This is a BSD system])
     AC_DEFINE_UNQUOTED(NETBSD,1,[This is a NetBSD system])
     ;;
*solaris*)
     AC_DEFINE_UNQUOTED(SOLARIS,1,[This is a Solaris system])
     AC_DEFINE_UNQUOTED(_REENTRANT,1,[Need with solaris or errno doesnt work])
     ;;
*arm-linux*)
     AC_DEFINE_UNQUOTED(LINUX,1,[This is a Linux kernel])
     AC_DEFINE_UNQUOTED(HAVE_LISTEN_SHUTDOWN,1,[can use shutdown on listen sockets])
     CFLAGS="-fPIC -pipe $CFLAGS"
     ;;
*linux*)
     AC_DEFINE_UNQUOTED(LINUX,1,[This is a Linux kernel])
     AC_DEFINE_UNQUOTED(HAVE_LISTEN_SHUTDOWN,1,[can use shutdown on listen sockets])
     ;;
*cygwin*)
     AC_DEFINE_UNQUOTED(CYGWIN,1,[This is a Cygwin system])
     os_is_windows=yes
     ;;
*mingw*)
     AC_DEFINE_UNQUOTED(MINGW,1,[This is a MinGW system])
     AC_DEFINE_UNQUOTED(WINDOWS,1,[This is a Windows system])
     LIBS="$LIBS -lws2_32"
     AC_CHECK_HEADERS([winsock2.h ws2tcpip.h],, AC_MSG_ERROR([[Winsock2 headers are required for W32]]))
     AC_CACHE_CHECK([for MS lib utility], [ac_cv_use_ms_lib_tool],
       [[mslibcheck=`lib 2>&1`
        if [[ $mslibcheck = "Microsoft (R) Library Manager"* ]]; then
          ac_cv_use_ms_lib_tool=yes
        else
          ac_cv_use_ms_lib_tool=no
        fi
         ]])
     if test "x$ac_cv_use_ms_lib_tool" = "xyes"; then
       AC_SUBST([MS_LIB_TOOL], [[lib]])
     fi
     AC_SUBST([lt_cv_objdir])
     os_is_windows=yes
     os_is_native_w32=yes
     ;;
*openedition*)
     AC_DEFINE_UNQUOTED(OS390,1,[This is a OS/390 system])
    ;;
*)
     AC_MSG_WARN([Unrecognised OS $host_os])
     AC_DEFINE_UNQUOTED(OTHEROS,1,[Some strange OS])
#    You might want to find out if your OS supports shutdown on listen sockets,
#    and extend the switch statement; if we do not have 'HAVE_LISTEN_SHUTDOWN',
#    pipes are used instead to signal 'select'.
#    AC_DEFINE_UNQUOTED(HAVE_LISTEN_SHUTDOWN,1,[can use shutdown on listen sockets])
;;
esac

if test "x$with_threads" = "xposix"; then
# forced posix threads
  if test "x$HAVE_POSIX_THREADS" = "xyes"; then
    USE_THREADS='posix'
  else
    if test "x$HAVE_W32_THREADS" = "xyes"; then
      AC_MSG_ERROR([[Posix threads are not available. Try to configure --with-threads=auto]])
    else
      AC_MSG_ERROR([[Posix threads are not available]])
    fi
  fi
elif test "x$with_threads" = "xw32"; then
# forced w32 threads
  if test "x$HAVE_W32_THREADS" = "xyes"; then
    USE_THREADS='w32'
  else
    if test "x$HAVE_POSIX_THREADS" = "xyes"; then
      AC_MSG_ERROR([[W32 threads are not available. Try to configure --with-threads=auto]])
    else
      AC_MSG_ERROR([[W32 threads are not available]])
    fi
  fi
else
# automatic threads lib selection
  if test "x$HAVE_POSIX_THREADS" = "xyes" && test "x$HAVE_W32_THREADS" = "xyes"; then
    if test "x$os_is_native_w32" = "xyes"; then
      USE_THREADS='w32'
    else
      USE_THREADS='posix'
    fi
  elif test "x$HAVE_POSIX_THREADS" = "xyes"; then
    USE_THREADS='posix'
  elif test "x$HAVE_W32_THREADS" = "xyes"; then
    USE_THREADS='w32'
  else
    AC_MSG_ERROR([[No threading lib is available. Cosider installing pthreads]])
  fi
fi

if test "x$USE_THREADS" = "xposix"; then
  CC="$PTHREAD_CC"
  AC_DEFINE([MHD_USE_POSIX_THREADS],[1],[define to use pthreads])
  MHD_LIB_CFLAGS="$MHD_LIB_CFLAGS $PTHREAD_CFLAGS"
  MHD_LIBDEPS="$PTHREAD_LIBS $MHD_LIBDEPS"
elif test "x$USE_THREADS" = "xw32"; then
  AC_DEFINE([MHD_USE_W32_THREADS],[1],[define to use W32 threads])
fi
AM_CONDITIONAL([USE_POSIX_THREADS], [test "x$USE_THREADS" = "xposix"])
AM_CONDITIONAL([USE_W32_THREADS], [test "x$USE_THREADS" = "xw32"])


AM_CONDITIONAL(HAVE_W32, [test "x$os_is_native_w32" = "xyes"])
w32_shared_lib_exp=no
if test "x$enable_shared" = "xyes" && test "x$os_is_native_w32" = "xyes"; then
  if test "x$ac_cv_use_ms_lib_tool" = "xyes" || test -n "$DLLTOOL"; then
    w32_shared_lib_exp=yes
  else
    AC_MSG_WARN([[GNU dlltool or MS lib.exe is required for creating shared library export on W32]])
    AC_MSG_WARN([[Export library libmicrohttpd.lib will not be created]])
  fi
fi
AM_CONDITIONAL(W32_SHARED_LIB_EXP, [test "x$w32_shared_lib_exp" = "xyes"])
AM_CONDITIONAL(USE_MS_LIB_TOOL, [test "x$ac_cv_use_ms_lib_tool" = "xyes"])

# set GCC options
# use '-fno-strict-aliasing', but only if the compiler can take it
AX_APPEND_COMPILE_FLAGS([[-fno-strict-aliasing]])

AC_C_BIGENDIAN

AC_CHECK_PROG([HAVE_CURL_BINARY],[curl],[yes],[no])
AM_CONDITIONAL([HAVE_CURL_BINARY],[test "x$HAVE_CURL_BINARY" = "xyes"])
AC_CHECK_PROG([HAVE_MAKEINFO_BINARY],[makeinfo],[yes],[no])
AM_CONDITIONAL([HAVE_MAKEINFO_BINARY],[test "x$HAVE_MAKEINFO_BINARY" = "xyes"])
AM_CONDITIONAL(W32_STATIC_LIB, [test "x$os_is_native_w32" = "xyes"  && test "x$enable_static" = "xyes"])


AC_ARG_ENABLE([[doc]],
  [AS_HELP_STRING([[--disable-doc]], [do not build any documentation])], ,
    [enable_doc=yes])
test "x$enable_doc" = "xno" || enable_doc=yes
AM_CONDITIONAL([BUILD_DOC], [test "x$enable_doc" = "xyes"])

AC_ARG_ENABLE([[examples]],
  [AS_HELP_STRING([[--disable-examples]], [do not build any examples])], ,
    [enable_examples=yes])
test "x$enable_examples" = "xno" || enable_examples=yes
AM_CONDITIONAL([BUILD_EXAMPLES], [test "x$enable_examples" = "xyes"])

AC_ARG_ENABLE([[epoll]],
  [AS_HELP_STRING([[--enable-epoll[=ARG]]], [enable epoll support (yes, no, auto) [auto]])],
    [enable_epoll=${enableval}],
    [enable_epoll='auto']
  )

if test "$enable_epoll" != "no"; then
  AX_HAVE_EPOLL
  if test "${ax_cv_have_epoll}" = "yes"; then
    AC_DEFINE([EPOLL_SUPPORT],[1],[define to 1 to enable epoll support])
    enable_epoll='yes'
  else
    AC_DEFINE([EPOLL_SUPPORT],[0],[define to 0 to disable epoll support])
    if test "$enable_epoll" = "yes"; then
      AC_MSG_ERROR([[Support for epoll was explicitly requested but cannot be enabled on this platform.]])
    fi
    enable_epoll='no'
  fi
fi

if test "x$HAVE_POSIX_THREADS" = "xyes"; then
  # Check for pthread_setname_np()
  SAVE_LIBS="$LIBS"
  SAVE_CFLAGS="$CFLAGS"
  LIBS="$PTHREAD_LIBS $LIBS"
  CFLAGS="$CFLAGS $PTHREAD_CFLAGS"
  AC_MSG_CHECKING([[for pthread_setname_np]])
  AC_LINK_IFELSE(
    [AC_LANG_PROGRAM([[#include <pthread.h>]], [[  pthread_setname_np(pthread_self(), "name")]])],
    [AC_DEFINE([[HAVE_PTHREAD_SETNAME_NP]], [[1]], [Define if you have pthread_setname_np function.])
     AC_MSG_RESULT([[yes]])],
    [AC_MSG_RESULT([[no]])] )
  LIBS="$SAVE_LIBS"
  CFLAGS="$SAVE_CFLAGS"
fi

# Check for headers that are ALWAYS required
AC_CHECK_HEADERS([fcntl.h math.h errno.h limits.h stdio.h locale.h sys/stat.h sys/types.h pthread.h],,AC_MSG_ERROR([Compiling libmicrohttpd requires standard UNIX headers files]))

# Check for optional headers
AC_CHECK_HEADERS([sys/types.h sys/time.h sys/msg.h netdb.h netinet/in.h netinet/tcp.h time.h sys/socket.h sys/mman.h arpa/inet.h sys/select.h poll.h search.h])
AM_CONDITIONAL([HAVE_TSEARCH], [test "x$ac_cv_header_search_h" = "xyes"])

AC_CHECK_MEMBER([struct sockaddr_in.sin_len],
   [ AC_DEFINE(HAVE_SOCKADDR_IN_SIN_LEN, 1, [Do we have sockaddr_in.sin_len?])
   ],
   [],
   [
    #ifdef HAVE_SYS_TYPES_H
      #include <sys/types.h>
    #endif
    #ifdef HAVE_SYS_SOCKET_H
      #include <sys/socket.h>
    #endif
    #ifdef HAVE_NETINET_IN_H
      #include <netinet/in.h>
    #endif
   ])

# Check for pipe/socketpair signaling
AC_MSG_CHECKING([[whether to enable signaling by socketpair]])

AC_ARG_ENABLE([[socketpair]],
	[AS_HELP_STRING([[--enable-socketpair[=ARG]]], [disable internal singalling by pipes and use socket pair instead (yes, no, try) [no]])], ,
	[AS_IF([[test "x$os_is_windows" = "xyes"]], [enable_socketpair=yes], [enable_socketpair=no])]
  )

AS_IF(
       [[test "x$enable_socketpair" != "xno"]],
         [AS_IF([[test "x$os_is_windows" = "xyes"]],
           [ AC_MSG_RESULT([[yes, forced on W32]]) ],
           [ AC_LINK_IFELSE(
             [ AC_LANG_PROGRAM([[
				#ifdef HAVE_SYS_TYPES_H
				#include <sys/types.h>
				#endif
				#ifdef HAVE_SYS_SOCKET_H
				#include <sys/socket.h>
				#endif
				]],[[
				  int sv[2];
				  if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv) != 0) return 1
				]])
             ],
             [ AC_MSG_RESULT([[yes, socketpair in available]]) ],
             [ AC_MSG_RESULT([[no, socketpair in not available]])
              AS_IF([[test "x$enable_socketpair" = "xyes"]], [ AC_MSG_ERROR([[socketpair signalling cannot be enabled.]]) ])
             ]
             )
           ]
          )
         ],
       [
        AC_MSG_RESULT([[no]])
        AS_IF([[test "x$os_is_windows" = "xyes"]], [ AC_MSG_ERROR([[socketpair must be enabled on W32]]) ])
       ]
     )
if test "x$enable_socketpair" = "xyes"; then
	AC_DEFINE([[MHD_DONT_USE_PIPES]], [[1]], [Define to use pair of sockets instead of pipes for signaling])
fi

AC_CHECK_FUNCS_ONCE([memmem accept4])
AC_MSG_CHECKING([[for gmtime_s]])
AC_LINK_IFELSE(
  [AC_LANG_PROGRAM(
    [[ #include <time.h>]], [[struct tm now; time_t t; time (&t); gmtime_s (&now, &t)]])
  ],
  [
    AC_DEFINE([HAVE_GMTIME_S], [1], [Define to 1 if you have `gmtime_s' function (only for W32).])
    AC_MSG_RESULT([[yes]])
  ],
  [AC_MSG_RESULT([[no]])
  ])


AC_CHECK_DECLS([SOCK_NONBLOCK], [AC_DEFINE([HAVE_SOCK_NONBLOCK], [1], [SOCK_NONBLOCK is defined in a socket header])], [],
                   [
                    #if defined HAVE_SYS_TYPES_H
                    #  include <sys/types.h>
                    #endif
                    #if defined HAVE_SYS_SOCKET_H
                    #  include <sys/socket.h>
                    #elif defined HAVE_WINSOCK2_H
                    #  include <winsock2.h>
                    #endif
                   ])


AC_SEARCH_LIBS([clock_gettime], [rt], [
				AC_DEFINE(HAVE_CLOCK_GETTIME, 1, [Have clock_gettime])
])

# IPv6
AC_MSG_CHECKING(for IPv6)
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <stdio.h>
#if HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#if HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#if HAVE_WINSOCK2_H
#include <winsock2.h>
#endif
#if HAVE_WS2TCPIP_H
#include <ws2tcpip.h>
#endif
]], [[
int af=AF_INET6;
int pf=PF_INET6;
struct sockaddr_in6 sa;
printf("%d %d %p\n", af, pf, &sa);
]])],[
have_inet6=yes;
AC_DEFINE([HAVE_INET6], [1], [Provides IPv6 headers])
],[
have_inet6=no
])
AC_MSG_RESULT($have_inet6)

# TCP_CORK and TCP_NOPUSH
AC_CHECK_DECLS([TCP_CORK, TCP_NOPUSH], [], [], [[#include <netinet/tcp.h>]])

HIDDEN_VISIBILITY_CFLAGS=""
case "$host" in
  *-*-mingw*)
    dnl on mingw32 we do -fvisibility=hidden and __declspec(dllexport)
    AC_DEFINE([_MHD_EXTERN], [__attribute__((visibility("default"))) __declspec(dllexport) extern],
              [defines how to decorate public symbols while building])
    HIDDEN_VISIBILITY_CFLAGS="-fvisibility=hidden"
    ;;
  *)
    dnl on other compilers, check if we can do -fvisibility=hidden
    AX_CHECK_COMPILE_FLAG([-fvisibility=hidden],
                           [AC_DEFINE([_MHD_EXTERN], [__attribute__((visibility("default"))) extern],
                                       [defines how to decorate public symbols while building])
                            HIDDEN_VISIBILITY_CFLAGS="-fvisibility=hidden"])
    ;;
esac
AC_SUBST(HIDDEN_VISIBILITY_CFLAGS)

# libcurl (required for testing)
AC_ARG_ENABLE([curl],
  [AS_HELP_STRING([--disable-curl],[disable cURL based testcases])],
  [enable_curl=${enableval}])
curl=0
if test "$enable_curl" != "no"
then
  LIBCURL_CHECK_CONFIG([yes],[7.16.4],[enable_curl=yes],
    [
      if test "x$enable_curl" = "xyes"; then
        AC_MSG_WARN([[cURL-based tests cannot be enabled because libcurl is missing]])
      fi
      enable_curl=no
    ])
fi
if test "$enable_curl" != "no"
then
# Lib cURL & cURL - OpenSSL versions
  AC_DEFINE([MHD_REQ_CURL_VERSION], ["7.16.4"], [required cURL version to run tests])
  AC_DEFINE([MHD_REQ_CURL_OPENSSL_VERSION], ["0.9.8"], [required cURL SSL version to run tests])
  AC_DEFINE([MHD_REQ_CURL_GNUTLS_VERSION], ["2.8.6"], [gnuTLS lib version - used in conjunction with cURL])
  AC_DEFINE([MHD_REQ_CURL_NSS_VERSION], ["3.12.0"], [NSS lib version - used in conjunction with cURL])
fi
AM_CONDITIONAL([HAVE_CURL], [test "x$enable_curl" = "xyes"])

AC_CHECK_LIB([[magic]], [[magic_open]],
  [AC_CHECK_HEADERS([magic.h],
   AM_CONDITIONAL(HAVE_MAGIC, true),
   AM_CONDITIONAL(HAVE_MAGIC, false))],
  AM_CONDITIONAL(HAVE_MAGIC, false))


# large file support (> 4 GB)
AC_SYS_LARGEFILE
AC_FUNC_FSEEKO

# optional: have error messages ?
AC_MSG_CHECKING(whether to generate error messages)
AC_ARG_ENABLE([messages],
   [AS_HELP_STRING([--disable-messages],
               [disable MHD error messages])],
   [enable_messages=${enableval}],
   [enable_messages=yes])
AC_MSG_RESULT($enable_messages)
if test "$enable_messages" = "yes"
then
 AC_DEFINE([HAVE_MESSAGES],[1],[Include error messages])
else
 AC_DEFINE([HAVE_MESSAGES],[0],[Disable error messages])
fi


# optional: have postprocessor?
AC_MSG_CHECKING(whether to enable postprocessor)
AC_ARG_ENABLE([postprocessor],
   [AS_HELP_STRING([--disable-postprocessor],
               [disable MHD PostProcessor functionality])],
   [enable_postprocessor=${enableval}],
   [enable_postprocessor=yes])
test "x$enable_postprocessor" = "xno" || enable_postprocessor=yes
AC_MSG_RESULT([[$enable_postprocessor]])
AM_CONDITIONAL([HAVE_POSTPROCESSOR],test "x$enable_postprocessor" != "xno")
if test "x$enable_postprocessor" != "xno"
then
 AC_DEFINE([HAVE_POSTPROCESSOR],[1],[define to 1 if MHD was build with postprocessor.c])
fi


# optional: have zzuf, socat?
AC_CHECK_PROG([have_zzuf],[zzuf], [yes], [no])
AC_CHECK_PROG([have_socat],[socat], [yes], [no])
AM_CONDITIONAL([HAVE_ZZUF], [test "x$have_zzuf" = "xyes"])
AM_CONDITIONAL([HAVE_SOCAT], [test "x$have_socat" = "xyes"])


# libgcrypt linkage: required for HTTPS support
AM_PATH_LIBGCRYPT([1.2.2], [have_gcrypt=yes], [have_gcrypt=no])
if test "x$have_gcrypt" = "xyes"
then
  SAVE_CFLAGS="$CFLAGS"
  CFLAGS="$CFLAGS $LIBGCRYPT_CFLAGS"
  # LIBGCRYPT_CFLAGS can be actually a CPPFLAGS, so check them both
  SAVE_CPPFLAGS="$CPPFLAGS"
  CPPFLAGS="$CPPFLAGS $LIBGCRYPT_CFLAGS"
  AC_CHECK_HEADERS([gcrypt.h], [], [have_gcrypt=no])
  CFLAGS="$SAVE_CFLAGS"
  CPPFLAGS="$SAVE_CPPFLAGS"
fi

# gnutls
GNUTLS_CPPFLAGS=""
GNUTLS_CFLAGS=""
GNUTLS_LDFLAGS=""
GNUTLS_LIBS=""
have_gnutls=no
have_gnutls_sni=no
AC_MSG_CHECKING(for gnutls)
AC_ARG_WITH(gnutls,
   [AC_HELP_STRING([--with-gnutls=PFX],[base of gnutls installation])],
   [AC_MSG_RESULT([using prefix $with_gnutls])
    case $with_gnutls in
      no)
        ;;
      yes)
        AC_CHECK_HEADERS([gnutls/gnutls.h],
            [AC_CHECK_LIB([gnutls], [gnutls_priority_set], [
              GNUTLS_LIBS="-lgnutls"
              AC_CHECK_LIB([gnutls], [gnutls_load_file], [AC_CHECK_LIB([gnutls], [gnutls_privkey_import_x509_raw], [have_gnutls_sni=yes])])
              have_gnutls=yes])])
        ;;
      *)
        SAVE_LDFLAGS="$LDFLAGS"
        SAVE_CPPFLAGS="$CPPFLAGS"
        LDFLAGS="-L$with_gnutls/lib $LDFLAGS"
        CPPFLAGS="-I$with_gnutls/include $CPPFLAGS"
        AC_CHECK_FILE([$with_gnutls/include/gnutls/gnutls.h],
          [AC_CHECK_HEADERS([gnutls/gnutls.h],
            [AC_CHECK_LIB([gnutls], [gnutls_priority_set],
              [
                GNUTLS_CPPFLAGS="-I$with_gnutls/include"
                GNUTLS_LDFLAGS="-L$with_gnutls/lib"
                GNUTLS_LIBS="-lgnutls"
                AC_CHECK_LIB([gnutls], [gnutls_load_file], [AC_CHECK_LIB([gnutls], [gnutls_privkey_import_x509_raw], [have_gnutls_sni=yes])])
                have_gnutls=yes
              ])])])
        AS_IF([test "x$have_gnutls" != "xyes"], [AC_MSG_ERROR([can't find usable libgnutls at specified prefix $with_gnutls])])
        LDFLAGS="$SAVE_LDFLAGS"
        CPPFLAGS="$SAVE_CPPFLAGS"
        ;;
    esac
   ],
   [AC_MSG_RESULT([--with-gnutls not specified])
    AC_CHECK_HEADERS([gnutls/gnutls.h],
        [AC_CHECK_LIB([gnutls], [gnutls_priority_set],
          [
            GNUTLS_LIBS="-lgnutls"
            AC_CHECK_LIB([gnutls], [gnutls_load_file], [AC_CHECK_LIB([gnutls], [gnutls_privkey_import_x509_raw], [have_gnutls_sni=yes])])
            have_gnutls=yes
          ])])])
AM_CONDITIONAL(HAVE_GNUTLS, test "x$have_gnutls" = "xyes")
AM_CONDITIONAL([HAVE_GNUTLS_SNI], [test "x$have_gnutls_sni" = "xyes"])

AC_SUBST([GNUTLS_CPPFLAGS])
AC_SUBST([GNUTLS_LDFLAGS])
AC_SUBST([GNUTLS_LIBS])

# optional: HTTPS support.  Enabled by default
AC_MSG_CHECKING(whether to support HTTPS)
AC_ARG_ENABLE([https],
   [AS_HELP_STRING([--enable-https],
               [enable HTTPS support (yes, no, auto)[auto]])],
   [enable_https=${enableval}])
if test "x$enable_https" != "xno"
then
  AS_IF([test "x$have_gnutls" = "xyes" && test "x$have_gcrypt" = "xyes"], [
          AC_DEFINE([HTTPS_SUPPORT],[1],[include HTTPS support])
          MHD_LIB_CPPFLAGS="$MHD_LIB_CPPFLAGS $GNUTLS_CPPFLAGS $LIBGCRYPT_CFLAGS"
          MHD_LIB_CFLAGS="$MHD_LIB_CFLAGS $GNUTLS_CFLAGS $LIBGCRYPT_CFLAGS"
          MHD_LIB_LDFLAGS="$MHD_LIB_LDFLAGS $GNUTLS_LDFLAGS"
          MHD_LIBDEPS="$GNUTLS_LIBS $LIBGCRYPT_LIBS $MHD_LIBDEPS"
          enable_https=yes
          MSG_HTTPS="yes (using libgnutls and libgcrypt)"
        ], [
          AS_IF([test "x$have_gnutls" = "xyes"], [crypt_missing="libgrypt"],
                [test "x$have_gcrypt" = "xyes"], [crypt_missing="libgnutls"],
                                                 [crypt_missing="libgrypt and libgnutls"])
          AS_IF([[test "x$enable_https" = "xyes" ]], [AC_MSG_ERROR([[HTTPS support cannot be enabled without $crypt_missing.]])])
          AC_DEFINE([HTTPS_SUPPORT],[0],[no libgcrypt or libgnutls])
          enable_https=no
          MSG_HTTPS="no (lacking $crypt_missing)"
        ])
else
  AC_DEFINE([HTTPS_SUPPORT],[0],[disable HTTPS support])
  MSG_HTTPS="no (disabled)"
fi
AC_MSG_RESULT([$MSG_HTTPS])

AM_CONDITIONAL([ENABLE_HTTPS], [test "x$enable_https" = "xyes"])

# optional: HTTP Basic Auth support. Enabled by default
AC_MSG_CHECKING(whether to support HTTP basic authentication)
AC_ARG_ENABLE([bauth],
		AS_HELP_STRING([--disable-bauth],
			[disable HTTP basic Auth support]),
		[enable_bauth=${enableval}],
		[enable_bauth=yes])
if test "x$enable_bauth" != "xno"
then
 enable_bauth=yes
 AC_DEFINE([BAUTH_SUPPORT],[1],[include basic Auth support])
else
 AC_DEFINE([BAUTH_SUPPORT],[0],[disable basic Auth support])
fi
AC_MSG_RESULT($enable_bauth)
AM_CONDITIONAL(ENABLE_BAUTH, [test "x$enable_bauth" != "xno"])

# optional: HTTP Digest Auth support. Enabled by default
AC_MSG_CHECKING(whether to support HTTP digest authentication)
AC_ARG_ENABLE([dauth],
		AS_HELP_STRING([--disable-dauth],
			[disable HTTP basic and digest Auth support]),
		[enable_dauth=${enableval}],
		[enable_dauth=yes])
if test "$enable_dauth" != "xno"
then
 enable_dauth=yes
 AC_DEFINE([DAUTH_SUPPORT],[1],[include digest Auth support])
else
 AC_DEFINE([DAUTH_SUPPORT],[0],[disable digest Auth support])
fi
AC_MSG_RESULT($enable_dauth)
AM_CONDITIONAL(ENABLE_DAUTH, [test "x$enable_dauth" != "xno"])



MHD_LIB_LDFLAGS="$MHD_LIB_LDFLAGS -export-dynamic -no-undefined"

# gcov compilation
AC_MSG_CHECKING(whether to compile with support for code coverage analysis)
AC_ARG_ENABLE([coverage],
              AS_HELP_STRING([--enable-coverage],
                             [compile the library with code coverage support]),
              [use_gcov=${enableval}],
              [use_gcov=no])
AC_MSG_RESULT($use_gcov)
AM_CONDITIONAL([USE_COVERAGE], [test "x$use_gcov" = "xyes"])

AX_COUNT_CPUS
AC_SUBST([CPU_COUNT])

AC_SUBST(MHD_LIB_CPPFLAGS)
AC_SUBST(MHD_LIB_CFLAGS)
AC_SUBST(MHD_LIB_LDFLAGS)

# for pkg-config
AC_SUBST(MHD_LIBDEPS)

AC_SUBST(CPPFLAGS)
AC_SUBST(LIBS)
AC_SUBST(LDFLAGS)

AC_CONFIG_FILES([
libmicrohttpd.pc
Makefile
contrib/Makefile
doc/Makefile
doc/doxygen/Makefile
doc/examples/Makefile
m4/Makefile
src/Makefile
src/include/Makefile
src/platform/Makefile
src/microhttpd/Makefile
src/examples/Makefile
src/testcurl/Makefile
src/testcurl/https/Makefile
src/testzzuf/Makefile])
AC_OUTPUT

# Finally: summary
if test "x$enable_curl" != "xyes"; then
 MSG_CURL="no, many unit tests will not run"
else
 MSG_CURL="yes"
fi

AC_MSG_NOTICE([Configuration Summary:
  Operating System:  ${host_os}
  Threading lib:     ${USE_THREADS}
  libcurl (testing): ${MSG_CURL}
  Target directory:  ${prefix}
  Messages:          ${enable_messages}
  Basic auth.:       ${enable_bauth}
  Digest auth.:      ${enable_dauth}
  Postproc:          ${enable_postprocessor}
  HTTPS support:     ${MSG_HTTPS}
  epoll support:     ${enable_epoll=no}
  build docs:        ${enable_doc}
  build examples:    ${enable_examples}
])

if test "x$enable_https" = "xyes"
then
 AC_MSG_NOTICE([HTTPS subsystem configuration:
  License         :  LGPL only
 ])
else
 AC_MSG_NOTICE([
  License         :  LGPL or eCos
])
fi

if HAVE_CURL
curltests = testcurl
if HAVE_ZZUF
if HAVE_SOCAT
zzuftests = testzzuf
endif
endif
endif

SUBDIRS = include platform microhttpd examples $(curltests) $(zzuftests) .

if BUILD_EXAMPLES
SUBDIRS += examples
endif

EXTRA_DIST = \
 datadir/cert-and-key.pem \
 datadir/cert-and-key-for-wireshark.pem \
 datadir/spdy-draft.txt

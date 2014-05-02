/*
  This file is a consideration for a design, based on the usage in some of the examples and
  tests, experimental only.
 */

#include <memory>
#include <string>

namespace lightning {
  namespace http {

    enum class options {
      internal_select
    };

    using
    MHD_AccessHandlerCallback
    class server {
      std::shared_ptr<daemon> server_;

      server(options opts, unsigned short port,
  };

}

#include <iostream>

#include "platform.h"
#include <microhttpd.h>

#define PAGE "<html><head><title>libmicrohttpd demo</title></head><body>libmicrohttpd demo</body></html>"

static int
ahc_echo (void *cls,
          struct MHD_Connection *connection,
          const char *url,
          const char *method,
          const char *version,
          const char *upload_data, size_t *upload_data_size, void **ptr)
{
  static int aptr;
  const char *me = (const char*)cls;
  struct MHD_Response *response;
  int ret;

  if (0 != strcmp (method, "GET"))
    return MHD_NO;              /* unexpected method */
  if (&aptr != *ptr)
    {
      /* do never respond on first call */
      *ptr = &aptr;
      return MHD_YES;
    }
  *ptr = NULL;                  /* reset when done */
  response = (MHD_Response*)MHD_create_response_from_buffer (strlen (me),
                (void *) me,
                MHD_RESPMEM_PERSISTENT);
  ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
  MHD_destroy_response (response);
  return ret;
}

int
main (int argc, char *const *argv)
{
  lightning::daemon *d;

  if (argc != 2)
    {
      std::cout << argv[0] << " PORT" << std::endl;
      return 1;
    }
  d = MHD_start_daemon (// MHD_USE_SELECT_INTERNALLY | MHD_USE_DEBUG | MHD_USE_POLL,
      MHD_USE_SELECT_INTERNALLY | MHD_USE_DEBUG,
      // MHD_USE_THREAD_PER_CONNECTION | MHD_USE_DEBUG | MHD_USE_POLL,
      // MHD_USE_THREAD_PER_CONNECTION | MHD_USE_DEBUG,
                        atoi (argv[1]),
                        NULL, NULL, &ahc_echo, (void*)PAGE,
      MHD_OPTION_CONNECTION_TIMEOUT, (unsigned int) 120,
      MHD_OPTION_END);
  if (d == NULL)
    return 1;
  (void) getc (stdin);
  MHD_stop_daemon (d);
  return 0;
}

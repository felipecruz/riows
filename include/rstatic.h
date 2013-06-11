#include <riows.h>
#include <rutils.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>

#ifndef _RSTATIC_H_
#define _RSTATIC_H_
char* extension (char *value);
char* extract_query_string (char *value);
void handle_static (rio_worker_t *worker, rio_client_t *client);
#endif

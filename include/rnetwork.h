#include <rioev.h>
#include <riows.h>
#include <rstatic.h>
#include <rhttp.h>
#include <rutils.h>
#include <cws.h>
#include <http_parser.h>
#include <hash.h>

#ifndef _RNETWORK_H_
#define _RNETWORK_H_
int rnetwork_loop (rio_worker_t *worker);
void del_and_close (rio_worker_t *worker, rio_client_t *client);
#endif

#include <rutils.h>
#include <riows.h>
#include "http_parser.h"

#ifndef _RHTTP_H_
#define _RHTTP_H_
int rio_on_message_begin (http_parser *parser);
int rio_on_uri(http_parser *parser, const char *at, size_t len);
int rio_on_status_complete (http_parser *parser);
int rio_on_header_field(http_parser *parser, const char *at, size_t len);
int rio_on_header_value(http_parser *parser, const char *at, size_t len);
int rio_on_headers_complete(http_parser *parser);
int rio_on_body(http_parser *parser, const char *at, size_t len);
int rio_on_message_complete(http_parser *parser);
#endif

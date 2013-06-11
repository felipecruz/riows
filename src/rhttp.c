#include <rhttp.h>

int rio_on_message_begin (http_parser *parser)
{
    return 0;
}

int rio_on_status_complete (http_parser *parser)
{
    return 0;
}

int rio_on_uri (http_parser *parser, const char *at, size_t len)
{
    rio_client_t *client = parser->data;
    client->path = malloc (sizeof (char) * (len + 1));

    if (client->path == NULL)
        handle_error ("Malloc");

    strncpy (client->path, at, len);
    client->path[len] = '\0';

    client->method = (unsigned char) parser->method;

    return 0;
}

int rio_on_header_field (http_parser *parser, const char *at, size_t len)
{
    return 0;
}

int rio_on_header_value (http_parser *parser, const char *at, size_t len)
{
    return 0;
}

int rio_on_headers_complete (http_parser *parser)
{
    return 0;
}

int rio_on_body (http_parser *parser, const char *at, size_t len)
{
    return 0;
}

int rio_on_message_complete (http_parser *parser)
{
    return 0;
}

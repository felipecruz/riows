#include <rssl.h>

void init_ssl_context (rio_worker_t *worker)
{
    int rc;
    const SSL_METHOD *ssl_method;

    SSL_library_init ();
    SSL_load_error_strings ();
    ssl_method = SSLv23_method ();
    worker->ctx = SSL_CTX_new (ssl_method);

    if (worker->ctx == NULL)
        handle_error ("Error initializing SSL Context");

    rc = SSL_CTX_use_certificate_file (worker->ctx, "misc/ssl.pem", SSL_FILETYPE_PEM);

    if (rc == -1)
        handle_error ("Error loading Cerificate file");

    rc = SSL_CTX_use_PrivateKey_file (worker->ctx, "misc/ssl.key", SSL_FILETYPE_PEM);

    if (rc == -1)
        handle_error ("Error loading Key file");

    if (!SSL_CTX_load_verify_locations (worker->ctx, "misc/ssl.pem", NULL))
        handle_error ("Error verifying locations");

    SSL_CTX_set_verify (worker->ctx, SSL_VERIFY_PEER, NULL);
    SSL_CTX_set_verify_depth (worker->ctx, 1);
}

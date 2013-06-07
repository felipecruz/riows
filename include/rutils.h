#ifndef _RUTILS_H_
#define _RUTILS_G_
#define handle_error(msg) \
       do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define log_err(M, ...) \
        fprintf(stderr, "[ERROR] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)

#define log_warn(M, ...) \
        fprintf(stderr, "[WARN] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)

#define log_info(M, ...) \
        fprintf(stderr, "[INFO] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif

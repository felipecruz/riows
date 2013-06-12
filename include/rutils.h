#ifndef _RUTILS_H_
#define _RUTILS_G_
#define handle_error(msg) \
       do { perror(msg);                                      \
            fprintf (stderr, "Errno: %d %s\n", errno,         \
                     clean_errno());                          \
            exit(EXIT_FAILURE); } while (0)

#define clean_errno() (errno == 0 ? "None" : strerror(errno))

#ifdef DEBUG
#define log_err(M, ...) \
        fprintf(stderr, "[ERROR] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)
#define log_warn(M, ...) \
        fprintf(stderr, "[WARN] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)
#define log_info(M, ...) \
        fprintf(stderr, "[INFO] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define log_debug(M, ...) \
        fprintf(stderr, "[DEBUG] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define log_err(M, ...) \
        fprintf(stderr, "[ERROR] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)
#define log_warn(M, ...) \
        fprintf(stderr, "[WARN] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)
#define log_info(M, ...) \
        fprintf(stderr, "[INFO] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define log_debug(M, ...)
#endif
#endif

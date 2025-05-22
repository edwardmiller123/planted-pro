#ifndef LOG
#define LOG

typedef enum log_level {
    FATAL,
    ERROR,
    WARNING,
    INFO,
    DEBUG,
} log_level;

// setLogLevel sets the global logging level
void set_log_level(log_level logLevel);

void logger(log_level level, char *msg);

// logs a formatted message at the given log level over USART1
void loggerf(log_level level, char *msg, uint32_t int_args[], uint32_t int_arg_count, char *str_args[], uint32_t str_args_count);

#endif
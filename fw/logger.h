#ifndef LOGGER
#define LOGGER

typedef enum log_level {
    FATAL,
    ERROR,
    WARNING,
    INFO,
    DEBUG,
} log_level;

// set_log_level sets the global logging level
void set_log_level(log_level logLevel);

void logger(log_level level, char *msg);

// Only make only make the logging function call if we have a log level selected
#ifdef LOGLEVEL
#define LOG(level, msg) logger(level, msg)
#else
#define LOG(...) {}
#endif


// logs a formatted message at the given log level over USART1
void loggerf(log_level level, char *msg, uint32_t int_args[], uint32_t int_arg_count, char *str_args[], uint32_t str_args_count);

// Only include the log lines if we have a log level selected
#ifdef LOGLEVEL
#define LOGF(level, msg, int_args, int_arg_count, str_args, str_args_count) loggerf(level, msg, int_args, int_arg_count, str_args, str_args_count)
#else
#define LOGF(...) {}
#endif

#endif
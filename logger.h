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
void loggerf(log_level level, char *msg, uint32_t args[], uint32_t arg_count);

#endif
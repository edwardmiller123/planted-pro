#include <stdio.h>
#include <stdint.h>

#include "logger.h"
#include "utils.h"
#include "gpio.h"
#include "interrupts.h"

// maximum length of a log message
#define MAX_MESSAGE_LENGTH 256
#define MAX_MESSAGE_PART_COUNT 10
#define MAX_MESSAGE_PART_SIZE 64


static log_level current_level = INFO;

// set_log_level sets the global logging level
void set_log_level(log_level level)
{
    current_level = level;
}

void logger_send(char *level, char *msg)
{
    char buf[MAX_MESSAGE_LENGTH];
    mem_zero((uint8_t *)buf, MAX_MESSAGE_LENGTH);

    char *finished_msg = buf;
    if (str_len(level) + str_len(msg) > MAX_MESSAGE_LENGTH)
    {
        finished_msg = "ERROR: Log message character limit exceeded";
    }
    else
    {
        str_cat(level, msg, finished_msg);
    }

    str_cat(finished_msg, "\r\n", finished_msg);

    if (usart1_send_buffer((uint8_t *)finished_msg, str_len(finished_msg)) == -1)
    {
        fast_blink();
    }
}

void log_debug(char *msg)
{
    if (current_level < DEBUG)
    {
        return;
    }

    char *level = "DEBUG: ";
    logger_send(level, msg);
}

void log_info(char *msg)
{
    if (current_level < INFO)
    {
        return;
    }

    char *level = "INFO: ";
    logger_send(level, msg);
}

void log_warning(char *msg)
{
    if (current_level < WARNING)
    {
        return;
    }

    char *level = "WARN: ";
    logger_send(level, msg);
}

void log_error(char *msg)
{
    if (current_level < ERROR)
    {
        return;
    }

    char *level = "ERROR: ";
    logger_send(level, msg);
}

void log_fatal(char *msg)
{
    char *level = "FATAL: ";
    logger_send(level, msg);

    // force a system reset since by definition FATAL is unrecoverable
    software_reset();
    
}

void logger(log_level level, char *msg)
{
    switch (level)
    {
    case DEBUG:
        log_debug(msg);
        break;
    case INFO:
        log_info(msg);
        break;
    case WARNING:
        log_warning(msg);
        break;
    case ERROR:
        log_error(msg);
        break;
    case FATAL:
        log_fatal(msg);
        break;
    }
}

// logs a formatted message at the given log level over USART1
void loggerf(log_level level, char *msg, uint32_t args[], uint32_t arg_count)
{
    if (msg == NULL)
    {
        log_error("Log message cannot be NULL");
        return;
    }

    int args_digit_count = 0;
    for (int i = 0; i < arg_count; i++)
    {
        args_digit_count += get_digit_count(args[i]);
    }

    int msg_length = str_len(msg) + args_digit_count;

    if (msg_length > MAX_MESSAGE_LENGTH)
    {
        log_error("Log message character limit exceeded");
        return;
    }

    char msg_parts[MAX_MESSAGE_PART_COUNT][MAX_MESSAGE_PART_SIZE];

    // this is full of garbage values so we zero before writing
    // our log message
    for (int i = 0; i < MAX_MESSAGE_PART_COUNT; i++)
    {
        mem_zero((uint8_t *)msg_parts[i], MAX_MESSAGE_PART_SIZE);
    }

    int arg_idx = 0;
    int part_idx = 0;
    int j = 0;
    for (int i = 0; i < str_len(msg); i++)
    {
        if (msg[i] == '$')
        {
            msg_parts[part_idx][j] = '\0';
            part_idx++;
            j = 0;

            // int_to_string needs the space for the string pre allocated so we pass in the
            // next array in msg_parts
            int_to_string(args[arg_idx], msg_parts[part_idx]);
            arg_idx++;
        }
        else
        {
            // Conditionally move on to the next message part if
            // an argument was just inserted and we still have more string to parse
            if (j == 0 && part_idx != 0)
            {
                part_idx++;
            }

            msg_parts[part_idx][j] = msg[i];
            j++;
        }

        if (part_idx > MAX_MESSAGE_PART_COUNT)
        {
            log_error("Log message has to many parts");
            return;
        }

        if (arg_idx > arg_count)
        {
            log_error("Logger argument limit exceeded");
            return;
        }
    }

    // now concatenate the parts of the string
    char formatted_msg[MAX_MESSAGE_LENGTH];

    int i = 0;
    uint8_t * pos = (uint8_t *)formatted_msg;
    uint32_t total_length = 0;
    do {
        pos = byte_copy((uint8_t *)msg_parts[i], pos, str_len(msg_parts[i]));
        total_length += str_len(msg_parts[i]);
        i++;
    } while (i <= part_idx);
    formatted_msg[total_length] = '\0';

    logger(level, formatted_msg);
}

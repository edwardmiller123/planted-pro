#include <stdint.h>

#include "logger.h"
#include "utils.h"
#include "gpio.h"
#include "usart.h"
#include "interrupts.h"

// maximum length of a log message
#define MAX_MESSAGE_LENGTH 512
#define MAX_MESSAGE_PART_COUNT 10
#define MAX_MESSAGE_PART_SIZE 128

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

    if (usart_send_buffer(USART3, (uint8_t *)finished_msg, str_len(finished_msg)) == -1)
    {
        fast_blink();
    }
}

void log_error(char *msg)
{
    char *level = "ERROR: ";
    logger_send(level, msg);
}

void logger(log_level level, char *msg)
{
    if (current_level < level)
    {
        return;
    }

    char * level_str;

    switch (level)
    {
    case DEBUG:
        level_str = "DEBUG: ";
        logger_send(level_str, msg);
        break;
    case INFO:
        level_str = "INFO: ";
        logger_send(level_str, msg);
        break;
    case WARNING:
        level_str = "WARN: ";
        logger_send(level_str, msg);
        break;
    case ERROR:
        log_error(msg);
        break;
    case FATAL:
        level_str = "FATAL: ";
        logger_send(level_str, msg);
        break;
    }
}

// logs a formatted message at the given log level over USART1
void loggerf(log_level level, char *msg, uint32_t int_args[], uint32_t int_arg_count, char *str_args[], uint32_t str_args_count)
{
    if (current_level < level)
    {
        return;
    }

    if (msg == NULL)
    {
        log_error("Log message cannot be NULL");
        return;
    }

    int args_digit_count = 0;
    for (int i = 0; i < int_arg_count; i++)
    {
        args_digit_count += get_digit_count(int_args[i]);
    }

    int str_args_total_length = 0;
    for (int i = 0; i < str_args_count; i++)
    {
        str_args_total_length += str_len(str_args[i]);
    }

    int msg_length = str_len(msg) + args_digit_count + str_args_total_length;

    if (msg_length > MAX_MESSAGE_LENGTH)
    {
        log_error("Log message character limit exceeded");
        return;
    }

    char msg_parts[MAX_MESSAGE_PART_COUNT][MAX_MESSAGE_PART_SIZE];

    // zero to remove garbage values from the message
    for (int i = 0; i < MAX_MESSAGE_PART_COUNT; i++)
    {
        mem_zero((uint8_t *)msg_parts[i], MAX_MESSAGE_PART_SIZE);
    }

    int str_arg_idx = 0;
    int int_arg_idx = 0;
    int part_idx = 0;
    int j = 0;
    for (int i = 0; i < str_len(msg); i++)
    {
        if (msg[i] == '$' || msg[i] == '&')
        {
            msg_parts[part_idx][j] = '\0';
            part_idx++;
            j = 0;

            switch (msg[i])
            {
            case '$':
                // int_to_string needs the space for the string pre allocated so we pass in the
                // next array in msg_parts
                int_to_string(int_args[int_arg_idx], msg_parts[part_idx]);
                int_arg_idx++;
                break;
            case '&':
                byte_copy((uint8_t *)str_args[str_arg_idx], (uint8_t *)msg_parts[part_idx], str_len(str_args[str_arg_idx]));
                str_arg_idx++;
                break;
            }
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

        if (int_arg_idx > int_arg_count)
        {
            log_error("Log message argument limit exceeded");
            return;
        }
    }

    // now concatenate the parts of the string
    char formatted_msg[MAX_MESSAGE_LENGTH];

    int i = 0;
    uint8_t *pos = (uint8_t *)formatted_msg;
    uint32_t total_length = 0;
    do
    {
        pos = byte_copy((uint8_t *)msg_parts[i], pos, str_len(msg_parts[i]));
        total_length += str_len(msg_parts[i]);
        i++;
    } while (i <= part_idx);
    formatted_msg[total_length] = '\0';

    logger(level, formatted_msg);
}

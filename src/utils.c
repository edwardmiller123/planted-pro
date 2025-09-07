#include <stdint.h>
#include <stdbool.h>

#include "utils.h"
#include "time.h"

#define MAX_STRING_SIZE 1024
#define MAX_DIGITS 10

uint32_t str_len(char *str)
{
	uint32_t len = 0;

	if (str == NULL)
	{
		return len;
	}

	for (int i = 0; i < MAX_STRING_SIZE; i++)
	{
		if (str[i] == '\0')
		{
			break;
		}
		len++;
	}
	return len;
}

// copies n bytes from src to dst returning a pointer to the end of the copied region
uint8_t *byte_copy(uint8_t *src, uint8_t *dst, uint32_t n)
{
	for (int i = 0; i < n; i++)
	{
		*dst++ = *src++;
	}
	return dst;
}

void mem_zero(uint8_t *dst, uint32_t n)
{
	for (int i = 0; i < n; i++)
	{
		dst[i] = 0;
	}
}

// concatenates the two null terminated strings in the given buffer. The new buffer must
// have enough room for both strings and no args can be NULL.
void str_cat(char *str1, char *str2, char *buf)
{
	uint32_t str1_len = str_len(str1);
	uint32_t str2_len = str_len(str2);
	uint8_t *pos = byte_copy((uint8_t *)str1, (uint8_t *)buf, str1_len);
	pos = byte_copy((uint8_t *)str2, pos, str2_len);
	*pos = '\0';
}

// returns the reverse of the provided string.
char *reverse_string(char *str, char *new_str)
{
	int j = str_len(str) - 1;
	for (int i = 0; i < str_len(str); i++)
	{
		new_str[i] = str[j];
		j--;
	}

	// Add back the terminating character
	new_str[str_len(str)] = '\0';
	return new_str;
}

// returns the number of digits in the given base 10 integer
int get_digit_count(int num)
{
	int multiple = 10;
	int count = 1;
	while (num > multiple)
	{
		count++;
		multiple *= 10;
	}
	return count;
}

// converts an integer to a string.
char *int_to_string(uint32_t integer, char *new_str)
{
	uint32_t digit_count = get_digit_count(integer);
	// check the number doesnt exceed the digit count. This shouldnt be possible
	// on a 32 bit system but checking just to be safe.
	if (digit_count > MAX_DIGITS)
	{
		return NULL;
	}

	char reverse_str[MAX_DIGITS];
	uint32_t n = integer;
	uint32_t i = 0;
	uint32_t a = integer;
	uint32_t b;
	do
	{
		b = a % 10;
		reverse_str[i] = b + 0x30;
		a /= 10;
		n /= 10;
		i++;
	} while (n > 0);

	reverse_str[i] = '\0';

	char *result_string = reverse_string(reverse_str, new_str);

	return result_string;
}

int wait_for_condition(bool (*cond)(void), uint32_t ms)
{
	uint32_t timeout = get_system_counter() + ms;
	while (get_system_counter() < timeout)
	{
		if ((*cond)())
		{
			return 0;
		}
	}
	return -1;
}

uint8_t char_to_int(char character, result_code *status)
{
	if (status != NULL)
	{
		*status = SUCCESS;
	}

	switch (character)
	{
	case '0':
		return 0;
	case '1':
		return 1;
	case '2':
		return 2;
	case '3':
		return 3;
	case '4':
		return 4;
	case '5':
		return 5;
	case '6':
		return 6;
	case '7':
		return 7;
	case '8':
		return 8;
	case '9':
		return 9;
	}

	if (status != NULL)
	{
		*status = BAD_INPUT;
	}

	return 123;
}

uint32_t string_to_uint_base_10(char *str, uint32_t len, result_code *status)
{

	if (status != NULL)
	{
		*status = SUCCESS;
	}

	uint32_t total = 0;
	uint8_t current_num = 0;
	result_code conversion_status;

	for (int i = 0; i < len; i++)
	{
		current_num = char_to_int(str[i], &conversion_status);
		if (conversion_status != SUCCESS)
		{
			if (status != NULL)
			{
				*status = conversion_status;
			}

			return 0;
		}

		total *= 10;
		total += current_num;
	}
	return total;
}
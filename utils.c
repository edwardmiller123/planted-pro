#include <stdint.h>

#define MAX_STRING_SIZE 256

uint32_t strlen(char *str)
{
	uint32_t len = 0;
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
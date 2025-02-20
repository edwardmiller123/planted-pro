#ifndef IO_H
#define IO_H

static inline void write_word(unsigned int *port, unsigned int data)
{
    *(volatile unsigned int *)port = data;
}

static inline unsigned int read_word(unsigned int *port)
{
    return *(volatile unsigned int *)port;
}

static inline unsigned int toggle_bit(unsigned int num, unsigned int n)
{
    return num ^ ((unsigned int)1 << n);
}

#endif
#ifndef UART_IO_H
#define UART_IO_H

#include <avr/io.h>
#include <stdio.h>
#include <stdarg.h>

/** Prints a single char over serial com */
void uart_putc(unsigned char c);

/** Like 'printf' but over serial com */
int uart_printf(char *s, ...);

/** Reads a single char from serial com */
unsigned char uart_readc();

/** Tries to read one of several keywords over serial com
    Matching occurs only after a '\n' was read (which will no be treated as part
    of the keyword).
    @param kws array of '\0' terminated key words
    @param kws_len length of 'kws' array
    @returns index of keyword if matched, < 0 on error
*/
int uart_read_keyword(char *kws[], uint8_t kws_len);

/* Initializes serial com
   Sets communication to 8n1 (one startbit, one stop bit, no parity bit)
   and baudrate defined by #F_CPU and #BAUD.

   Default values (if not changed before building) are
   - F_CPU 16000000L
   - BAUD 9600
 */
void uart_init();
#endif /* UART_IO_H */

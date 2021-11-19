#define MYUBRR F_CPU/16/BAUD-1
#define READ_KW_BUF_LEN 32
#include <stdlib.h>
#include <string.h>
#include "uart_io.h"

/**************************************
            PRINTING
**************************************/

void uart_putc(unsigned char c) {
    while (!(UCSR0A & (1<<UDRE0))) {}
    UDR0 = c;
}

int uart_printf(char *s, ...) {
    char buffer[128];
    va_list myargs;
    va_start(myargs, s);
    int ret = vsnprintf(buffer, sizeof(buffer), s, myargs);
    va_end(myargs);

    if (ret >= 0) {
        for (int i = 0; i < ret; i++) {
            if (!buffer[i]) {
                break;
            } else {
                uart_putc(buffer[i]);
            }
        }
    }

    return ret;
}

/**************************************
            READING
**************************************/

unsigned char uart_readc() {
    while (!(UCSR0A & (1<<RXC0))) ;
    return UDR0;
}

static void uart_erro_keyword_length() {
    uart_printf("EKWLEN Cannot read keywords longer than %d\n", READ_KW_BUF_LEN);
}


static void uart_error_bad_keyword(
    char buffer[],
    uint8_t buf_len,
    char *kws[],
    uint8_t kws_len
  ) {
    char *buf_tmp = malloc(buf_len * sizeof(char) + 1);
    memcpy(buf_tmp, buffer, buf_len * sizeof(char));
    buf_tmp[buf_len * sizeof(char)] = '\0';
    uart_printf("EBADKW got %s\n", buf_tmp);
    free(buf_tmp);
    uart_printf("Valid keywords are:\n");
    for (int i = 0; i < kws_len; i++) {
        uart_printf("- %s\n", kws[i]);
    }
}

int uart_read_keyword(char *kws[], uint8_t kws_len) {
    char buffer[READ_KW_BUF_LEN];
    uint8_t buf_pos = 0;

    while (1) {
        if (buf_pos >= READ_KW_BUF_LEN) {
            uart_erro_keyword_length();
            return -2;
        } else {
            buffer[buf_pos] = uart_readc();
            if (buffer[buf_pos] == '\n') {
                for (int i = 0; i < kws_len; i++) {
                    char* kw = kws[i];
                    size_t kw_len = strlen(kw);
                    if (kw_len == buf_pos && 0 == strncmp(kw, buffer, kw_len)) {
                        return i;
                    }
                }
                uart_error_bad_keyword(buffer, buf_pos, kws, kws_len);
                return -1;
            }
            buf_pos++;
        }
    }
}

/**************************************
            INITIALISING
**************************************/

void uart_init() {
    UBRR0H = (MYUBRR>>8);
    UBRR0L = MYUBRR;

    /* Set to asynchronous USART using 8n1 encoding */
    UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
    /*Enable transmitter */
    UCSR0B = (1<<TXEN0) | (1<<RXEN0);
}

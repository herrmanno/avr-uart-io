# uart io

Serial io for avr (currently only atmega328p) controllers.

## Build & install

```sh
make
make install
```

### Defaults
The following defaults are assumed by the makefile and can be adjusted before build:
- processor: atmega328p
- CPU frequency: 16MHz
- BAUD rate for serial io: 9600
- Directory where to install header file(s): /usr/local/include
- Directory where to install library file(s): /usr/local/lib

## Example

```c
#include <uart_io.h>

int main() {
    /* init serial communication */
    uart_init();

    /* print string w/ formatted arguments */
    uart_printf("Hey there %s", "you");

    /* try to read a pre-defined keyword */
    char* kws[] = { "run", "stop" };
    uint8_t kws_len = 2;

    int result = uart_read_keyword(kws, kws_len);
    switch (result) {
        /* got keyword 0 */
        case 0:
            /* ... */
            break;
        /* got keyword 1 */
        case 1:
            /* ... */
            break;
        /* returns < 0 value in case no valid keyword could be read */
    }
}
```


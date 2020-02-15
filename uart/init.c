#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "uart_pl011.h"

char buf[64];
uint8_t buf_idx = 0u;

static void parse_cmd(void) {
    if (!strncmp("help\r", buf, strlen("help\r"))) {
        uart_write("Just type and see what happens!\n");
    } else if (!strncmp("uname\r", buf, strlen("uname\r"))) {
        uart_write("bare-metal arm 06_uart\n");
    }
}

int c_entry() {
        uart_config config = {
            .data_bits = 8,
            .stop_bits = 1,
            .parity = false,
            .baudrate = 9600
        };
        uart_configure(&config);

        uart_putchar('A');
        uart_putchar('B');
        uart_putchar('C');
        uart_putchar('\n');

        uart_write("I love drivers!\n");
        uart_write("Type below...\n");

        while (1) {
            char c;
            if (uart_getchar(&c) == UART_OK && (c != 0x7f)) {
                uart_putchar(c);
                buf[buf_idx % 64] = c;
                buf_idx++;

                if (c == '\r') {
                    uart_write("\n");
                    buf_idx = 0u;
                    parse_cmd();
                }
            }
            else if(c == 0x7f) {
                //backspace control here
            }
        }

        return 0;
}

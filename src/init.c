#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "uart_pl011.h"
#include "shell.h"

char buf[64];
uint8_t buf_idx = 0u;

static void parse_cmd(void) {
    if (!strncmp("help\r", buf, strlen("help\r"))) {
        uart_write("Supported instructions:\nuname\n");
    } else if (!strncmp("uname\r", buf, strlen("uname\r"))) {
        uart_write("armos_0.1\n");
    }

    shell_write();
}

int c_entry() {
        uart_config config = {
            .data_bits = 8,
            .stop_bits = 1,
            .parity = false,
            .baudrate = 9600
        };
        uart_configure(&config);

        uart_write("\n##############################################\n");
        uart_write("##### Welcome to ArmOs Operating System! #####\n");
        uart_write("######## This is version armos_0.1 :) ########\n");
        uart_write("##############################################\n\n");
        uart_write("Type \"help\" to see the available commands...\n\n");
        shell_write();

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

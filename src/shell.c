#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "shell.h"
#include "uart_pl011.h"

void shell_write(void)
{
    uart_write("root@armos_0.1:$ ");
}

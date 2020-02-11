#include <stdint.h>

volatile uint8_t* uart0 = (uint8_t*)0x10009000;

void print(const char* str)
{
	while (*str) {
		*uart0 = *str++;
	}
}

int main() {
    print("Hello world from bare metal!\n");



	while (1)
        ;
    //unreachable
	return 0;
}

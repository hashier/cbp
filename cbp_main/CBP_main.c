#include "stdio.h"

int _cbp_main();

// debug method to output register %eax and some given text
void print_eax(long eax, const char* message) {
	printf("eax=%ld %s\n", eax, message);
}

int main() {
    int ret = _cbp_main();
    return ret;
}

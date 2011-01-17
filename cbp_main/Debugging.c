#include <stdio.h>

// debug method to output register %eax and some given text
void print_eax(long eax, const char* message) {
    printf("eax=%ld %s\n", eax, message);
}

void assert_equals_int32(int expected, int actual) {
    if(expected == actual) {
        printf("OK:\t%d == %d\n", expected, actual);
    }
    else {
        fprintf(stderr, "FAIL:\tExpected %d, but found %d\n", expected, actual);
    }
}

void assert_equals_uint32(unsigned int expected, unsigned int actual) {
    if(expected == actual) {
        printf("OK:\t%u == %u\n", expected, actual);
    }
    else {
        fprintf(stderr, "FAIL:\tExpected %u, but found %u\n", expected, actual);
    }
}

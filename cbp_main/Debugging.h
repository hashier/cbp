#include <stdio.h>

// debug method to output only register %eax
void print_eax() {
    int value;
    asm (
        "movl %%eax, %0" /* move register %eax to variable value */
        :"=r"(value) /* output: goes to variable value referenced by %0 */
        :            /* input:  not needed, because only %eax is used */
        :"%eax"      /* clobbered register: gcc don't store other values in %eax */
    );
    printf("eax=%i\n", value);
}

// debug method to output the given long value and some text
void print_int_with_chars(long eax, const char* message) {
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

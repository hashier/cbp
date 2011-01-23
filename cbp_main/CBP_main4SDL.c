#include "SDL/SDL.h"
#include "additional_functions.h"
#include "Debugging.h"

int _cbp_main();

int main(int argc, char *argv[]) {
    int ret = _cbp_main();
    return ret;
}

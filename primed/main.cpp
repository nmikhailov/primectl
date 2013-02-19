#include <cstdlib>
#include <unistd.h>

int main(int argc, char **argv) {
    daemon(0, 0);
    return 0;
}


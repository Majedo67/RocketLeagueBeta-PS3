
#include <sys/timer.h>

int main() {
    while (true) {
        sys_timer_usleep(16667);
    }

    return 0;
}
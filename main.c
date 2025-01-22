#include "gpio.h"
#include <signal.h>
#include <unistd.h>

int main() {
    signal(SIGINT, cleanup_gpio); // 中斷信號ctrl+c
    setup_gpio();
    set_gpio_mode(17, 0b001); // 設GPIO 17輸出

    while (1) {
        gpio_write(17, 1); 
        usleep(500000);    
        gpio_write(17, 0);  
        usleep(500000);    
    }

    return 0;
}


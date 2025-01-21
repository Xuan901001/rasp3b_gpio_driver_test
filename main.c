#include "gpio.h"
#include <signal.h> // 信號處理
#include <unistd.h>

int main() {
    signal(SIGINT, cleanup_gpio); // 註冊中斷信號處理
    setup_gpio();
    set_gpio_mode(17, 0b001); // 設置 GPIO 17 為輸出模式

    while (1) {
        gpio_write(17, 1);  // 高電位
        usleep(500000);     // 延遲 500ms
        gpio_write(17, 0);  // 低電位
        usleep(500000);     // 延遲 500ms
    }

    return 0;
}


#include "gpio.h"
#include <fcntl.h>      // 打開文件
#include <sys/mman.h>   // mmap 映射
#include <unistd.h>     // usleep, close
#include <stdlib.h>     // exit
#include <stdio.h>      // perror

volatile uint32_t *gpio; //指向gpio的指標

void setup_gpio() {
    int mem_fd;
    void *gpio_map; //指向映射內存的指標

    if ((mem_fd = open("/dev/gpiomem", O_RDWR | O_SYNC)) < 0) { //以讀寫方式
        perror("Failed to open /dev/gpiomem");
        exit(EXIT_FAILURE);
    }

    gpio_map = mmap(NULL, BCM2835_BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, 0);
    // 遮邊參數 NULL讓系統自動選擇映射的地址 設定大小 以及設定記憶體區域可讀可寫 並設定內存映射共享模式
    // 偏移亮(從頭開始)
    close(mem_fd);

    if (gpio_map == MAP_FAILED) {
        perror("mmap error");
        exit(EXIT_FAILURE);
    }

    gpio = (volatile uint32_t *)gpio_map;
}

//引腳模式
void set_gpio_mode(uint8_t pin, uint8_t mode) {
    uint32_t reg_offset = pin / 10; // 每個暫存器控制10個GPIO引腳
    uint32_t shift = (pin % 10) * 3; // 去計算引腳對應位移量，每個引腳佔用 3 個位元，用pin % 10去找引腳在寄存器相對位置
    volatile uint32_t *fsel = gpio + (GPFSEL0 / 4) + reg_offset; //定位到目標寄存器 從起始偏移量去/4(每個暫存器是 4 字節寬)加上 reg_offset就可以定位到目標寄存器
    *fsel &= ~(7 << shift); // 清除引腳模式(舊有的)
    *fsel |= (mode << shift); // 設定目標的模式
}

void gpio_write(uint8_t pin, uint8_t value) {
    if (value) {
        *(gpio + (GPSET0 / 4)) = (1 << pin);
    } else {
        *(gpio + (GPCLR0 / 4)) = (1 << pin);
    }
}

void cleanup_gpio(int signal) {
    gpio_write(17, 0);
    printf("Cleaning up GPIO and exiting...\n");
    exit(0);
}


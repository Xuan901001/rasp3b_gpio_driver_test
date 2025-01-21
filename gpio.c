#include "gpio.h"
#include <fcntl.h>      // 打開文件
#include <sys/mman.h>   // mmap 映射
#include <unistd.h>     // usleep, close
#include <stdlib.h>     // exit
#include <stdio.h>      // perror

volatile uint32_t *gpio;

void setup_gpio() {
    int mem_fd;
    void *gpio_map;

    if ((mem_fd = open("/dev/gpiomem", O_RDWR | O_SYNC)) < 0) {
        perror("Failed to open /dev/gpiomem");
        exit(EXIT_FAILURE);
    }

    gpio_map = mmap(NULL, BCM2835_BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, 0);
    close(mem_fd);

    if (gpio_map == MAP_FAILED) {
        perror("mmap error");
        exit(EXIT_FAILURE);
    }

    gpio = (volatile uint32_t *)gpio_map;
}

void set_gpio_mode(uint8_t pin, uint8_t mode) {
    uint32_t reg_offset = pin / 10;
    uint32_t shift = (pin % 10) * 3;
    volatile uint32_t *fsel = gpio + (GPFSEL0 / 4) + reg_offset;
    *fsel &= ~(7 << shift);
    *fsel |= (mode << shift);
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


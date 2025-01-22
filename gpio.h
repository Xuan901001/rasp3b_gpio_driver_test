#ifndef GPIO_H
#define GPIO_H

#include <stdint.h> // 固定整數(寬度)
#include <signal.h> 


#define BCM2835_PERI_BASE 0x3F000000         // 樹莓派gpio外設的基地址 
#define BCM2835_GPIO_BASE (BCM2835_PERI_BASE + 0x200000) // GPIO基地址
#define BCM2835_BLOCK_SIZE (4 * 1024)      // GPIO寄存器空間大小

#define GPFSEL0 0x0000  // 功能
#define GPSET0  0x001c  // 設置高電位
#define GPCLR0  0x0028  // 設置低電位
#define GPLEV0  0x0034  // GPIO 電平

void setup_gpio(void);
void set_gpio_mode(uint8_t pin, uint8_t mode);
void gpio_write(uint8_t pin, uint8_t value);
void cleanup_gpio(int signal);

#endif 


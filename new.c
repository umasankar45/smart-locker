
#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC26X2.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/devices/cc13x2_cc26x2/driverlib/ioc.h>
#include <ti/devices/cc13x2_cc26x2/driverlib/gpio.h>

#define TEST_DURATION_MS (30 * 1000) // 30 seconds
#define TOTAL_PINS 32 // CC2652R7 has DIO0 to DIO31

void *mainThread(void *arg0)
{
    Power_init();
    for (uint8_t i = 0; i < TOTAL_PINS; i++) {
        if (i != 13 && i != 14) {
            IOCPortConfigureSet(i, IOC_PORT_GPIO, IOC_STD_INPUT);
            GPIO_clearDio(i);
        }
    }
    Power_releaseDependency(PowerCC26XX_PERIPH_UART0);
    Power_releaseDependency(PowerCC26XX_PERIPH_I2C0);
    Power_releaseDependency(PowerCC26XX_PERIPH_GPT0); // Add for timer
    uint8_t currentPin = 0;
    while (1) {
        if (currentPin != 13 && currentPin != 14) {
            IOCPortConfigureSet(currentPin, IOC_PORT_GPIO, IOC_STD_INPUT);
            GPIO_clearDio(currentPin);
        }
        do {
            currentPin = (currentPin + 1) % TOTAL_PINS;
        } while (currentPin == 13 || currentPin == 14);
        if (currentPin != 13 && currentPin != 14) {
            IOCPortConfigureSet(currentPin, IOC_PORT_GPIO, IOC_STD_OUTPUT);
            GPIO_setDio(currentPin);
        }
        Task_sleep(TEST_DURATION_MS * 1000 / Clock_tickPeriod);
    }
    return NULL;
}
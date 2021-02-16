
#define FREERTOS_TEST
//#define NOOS_TEST
//#define SIMPLE_TEST

#ifdef SIMPLE_TEST

#include <stm32f1xx_hal.h>

#define LED_PORT                GPIOC
#define LED_PIN                 GPIO_PIN_13
#define LED_PORT_CLK_ENABLE     __HAL_RCC_GPIOC_CLK_ENABLE

extern "C" {
void SysTick_Handler(void)
{
    HAL_IncTick();

    // 1 Hz blinking
    if ((HAL_GetTick() % 500) == 0)
        HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
}
}

void initGPIO()
{
    GPIO_InitTypeDef GPIO_Config;

    GPIO_Config.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Config.Pull = GPIO_NOPULL;
    GPIO_Config.Speed = GPIO_SPEED_FREQ_HIGH;

    GPIO_Config.Pin = LED_PIN;

    LED_PORT_CLK_ENABLE();
    HAL_GPIO_Init(LED_PORT, &GPIO_Config);
}

int main(void)
{
    HAL_Init();
    initGPIO();
    // 1kHz ticks
    HAL_SYSTICK_Config(SystemCoreClock / 1000);

    for (;;)
        __WFI();

    return 0;
}

#else

#ifdef FREERTOS_TEST
#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
#endif

#include <main.h>
#include <gpio.h>
#include <cstddef>

#ifdef FREERTOS_TEST
static void blinky(void *arg)
{
    MX_GPIO_Init();

    for(;;)
    {
        vTaskDelay(500);
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    }
}
#endif

extern "C" {
void SystemClock_Config(void);
}

int main()
{
    HAL_Init();
    SystemClock_Config();


#ifdef NOOS_TEST
    MX_GPIO_Init();

    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);

    for (;;) {
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
        HAL_Delay(500);
    }
#endif

#ifdef FREERTOS_TEST
    xTaskCreate(blinky, "blinky", configMINIMAL_STACK_SIZE * 4, NULL, tskIDLE_PRIORITY + 1, NULL);
    
    vTaskStartScheduler();
    for (;;);
#endif
    
    return 0;

}

#ifdef FREERTOS_TEST
extern "C" {
void vApplicationTickHook(void)
{
}

void vApplicationIdleHook(void)
{
}

void vApplicationMallocFailedHook(void)
{
    taskDISABLE_INTERRUPTS();
    for(;;);
}

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
    (void) pcTaskName;
    (void) pxTask;

    taskDISABLE_INTERRUPTS();
    for(;;);
}
}
#endif

#endif

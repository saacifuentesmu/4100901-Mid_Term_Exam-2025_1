/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Sam C
 * @brief          : Main program body
 ******************************************************************************
 */
#include "gpio.h"
#include "systick.h"
#include "nvic.h"
#include "uart.h"
#include "tim.h"
#include "incubator_control.h"

void heartbeat_led_toggle(void)
{
    static uint32_t last_tick = 0;
    if (systick_get_tick() - last_tick >= 500) { // Cambia cada 500 ms
        gpio_toggle_pin(HEARTBEAT_LED_PORT, HEARTBEAT_LED_PIN);
        last_tick = systick_get_tick();
    }
}

/**
 * @brief Función principal del programa.
 *        Configura los periféricos y entra en un bucle infinito.
 *        El LED de heartbeat parpadea cada 500 ms.
*/
int main(void)
{
    // Inicialización de SysTick
    systick_init_1ms(); // Utiliza SYSCLK_FREQ_HZ (ej. 4MHz) de rcc.h

    // LED Heartbeat
    gpio_setup_pin(GPIOA, HEARTBEAT_LED_PIN, GPIO_MODE_OUTPUT, 0);

    // LED Externo ON/OFF
    gpio_setup_pin(GPIOA, EXTERNAL_LED_ONOFF_PIN, GPIO_MODE_OUTPUT, 0);

    // Botón B1
    gpio_setup_pin(GPIOC, 13, GPIO_MODE_INPUT, 0);
    nvic_exti_pc13_button_enable();

    // USART2
    uart2_init(9600); // Tarea del estudiante: cambiar a 115200
    nvic_usart2_irq_enable();

    // TIM3 Canal 1 para PWM (Ventilador)
    tim3_ch1_pwm_init(1000); // 1000 Hz para el ventilador
    
    // Calefactor (Pin GPIO)
    gpio_setup_pin(GPIOB, 10, GPIO_MODE_OUTPUT, 0);

    // Inicialización de la Lógica de la Aplicación
    incubator_app_init();

    // Mensaje de estado
    uart2_send_string("\r\nSistema Inicializado. Esperando comandos...\r\n");
    while (1) {
        heartbeat_led_toggle();
        incubator_run(); // Ejecutar la lógica principal de la aplicación
    }
}


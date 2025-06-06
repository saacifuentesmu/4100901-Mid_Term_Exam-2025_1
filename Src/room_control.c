/**
 ******************************************************************************
 * @file           : room_control.c
 * @author         : Sam C
 * @brief          : Room control driver for STM32L476RGTx
 ******************************************************************************
 */
#include "room_control.h"

#include "gpio.h"    // Para controlar LEDs y leer el botón (aunque el botón es por EXTI)
#include "systick.h" // Para obtener ticks y manejar retardos/tiempos
#include "uart.h"    // Para enviar mensajes
#include "tim.h"     // Para controlar el PWM


static volatile uint32_t g_door_open_tick = 0;
static volatile uint8_t g_door_open = 0;
static volatile uint32_t g_last_button_tick = 0;

// 1. Control automático de iluminación
static volatile uint8_t g_lamp_on = 0;
static volatile uint32_t g_lamp_idle_brightness = 20;
// 7. animatcion de lampara
static volatile uint8_t g_lamp_animation = 0;
static volatile uint32_t g_lamp_animation_tick = 0;



void room_control_app_init(void)
{
    gpio_write_pin(EXTERNAL_LED_ONOFF_PORT, EXTERNAL_LED_ONOFF_PIN, GPIO_PIN_RESET);
    g_door_open = 0;
    g_door_open_tick = 0;

    tim3_ch1_pwm_set_duty_cycle(g_lamp_idle_brightness); // Lámpara al 20%
    g_lamp_on = 0;

    // 4. Mensaje de bienvenida 
    uart2_send_string("\r\nControlador de Sala v1.0\r\n");
    uart2_send_string("Desarrollador: Sam C\r\n");
    uart2_send_string("Estado inicial:\r\n");
    uart2_send_string("  - Lámpara: 20%\r\n");
    uart2_send_string("  - Puerta: Cerrada\r\n");
}

void room_control_on_button_press(void)
{
    uint32_t now = systick_get_tick();
    if (now - g_last_button_tick < 50) return;  // Anti-rebote de 50 ms
    g_last_button_tick = now;

    uart2_send_string("Evento: Botón presionado - Abriendo puerta.\r\n");

    gpio_write_pin(EXTERNAL_LED_ONOFF_PORT, EXTERNAL_LED_ONOFF_PIN, GPIO_PIN_SET);
    g_door_open_tick = now;
    g_door_open = 1;
    // 1. Control automático de iluminación
    tim3_ch1_pwm_set_duty_cycle(100);
    uart2_send_string("Lámpara encendida al 100%.\r\n");
    g_lamp_on = 1;
}

void room_control_on_uart_receive(char cmd)
{
    switch (cmd) {
        case '1':
            g_lamp_idle_brightness = 100;
            tim3_ch1_pwm_set_duty_cycle(g_lamp_idle_brightness);
            uart2_send_string("Lámpara: brillo al 100%.\r\n");
            break;

        case '2':
            g_lamp_idle_brightness = 70;
            tim3_ch1_pwm_set_duty_cycle(g_lamp_idle_brightness);
            uart2_send_string("Lámpara: brillo al 70%.\r\n");
            break;

        case '3':
            g_lamp_idle_brightness = 50;
            tim3_ch1_pwm_set_duty_cycle(g_lamp_idle_brightness);
            uart2_send_string("Lámpara: brillo al 50%.\r\n");
            break;

        case '4':
            g_lamp_idle_brightness = 20;
            tim3_ch1_pwm_set_duty_cycle(g_lamp_idle_brightness);
            uart2_send_string("Lámpara: brillo al 20%.\r\n");
            break;

        case '0':
            g_lamp_idle_brightness = 0;
            tim3_ch1_pwm_set_duty_cycle(g_lamp_idle_brightness);
            uart2_send_string("Lámpara apagada.\r\n");
            break;

        case 'o':
        case 'O':
            gpio_write_pin(EXTERNAL_LED_ONOFF_PORT, EXTERNAL_LED_ONOFF_PIN, GPIO_PIN_SET);
            g_door_open_tick = systick_get_tick();
            g_door_open = 1;
            uart2_send_string("Puerta abierta remotamente.\r\n");
            break;

        case 'c':
        case 'C':
            gpio_write_pin(EXTERNAL_LED_ONOFF_PORT, EXTERNAL_LED_ONOFF_PIN, GPIO_PIN_RESET);
            g_door_open = 0;
            uart2_send_string("Puerta cerrada remotamente.\r\n");
            break;

        // 5. Comando UART para ver estado actual
        case 's':
        case 'S':
            uart2_send_string("Estado:\r\n");
            uart2_send_string("  - Lámpara: ");
            if (g_lamp_idle_brightness == 0) {
                uart2_send_string("0%\r\n");
            } else {
                uart2_send_string(g_lamp_idle_brightness == 100 ? "100%\r\n" :
                    g_lamp_idle_brightness == 70 ? "70%\r\n" :
                    g_lamp_idle_brightness == 50 ? "50%\r\n" :
                    g_lamp_idle_brightness == 20 ? "20%\r\n" : "Desconocido\r\n");
            }
            uart2_send_string(g_door_open ? "  - Puerta: Abierta\r\n" : "  - Puerta: Cerrada\r\n");
            break;

        // 6. Comando de ayuda UART 
        case '?':
        case 'h':
        case 'H':
            uart2_send_string("Comandos disponibles:\r\n");
            uart2_send_string("  '1'-'4': Ajustar brillo lámpara (100%, 70%, 50%, 20%)\r\n");
            uart2_send_string("  '0': Apagar lámpara\r\n");
            uart2_send_string("  'o': Abrir puerta remotamente\r\n");
            uart2_send_string("  'c': Cerrar puerta remotamente\r\n");
            uart2_send_string("  's': Ver estado actual (lámpara y puerta)\r\n");
            uart2_send_string("  '?', 'h': Mostrar esta ayuda\r\n");
            break;

        // 7. Comando para incrememntar brillo de la lámpara
        case 'g':
        case 'G':
            g_lamp_idle_brightness = 0;
            tim3_ch1_pwm_set_duty_cycle(g_lamp_idle_brightness);
            g_lamp_animation = 1;
            g_lamp_animation_tick = systick_get_tick();
            uart2_send_string("Animación de lámpara iniciada.\r\n");
            break;
        default:
            uart2_send_string("Comando desconocido.\r\n");
            break;
    }
}

void room_control_tick(void)
{
    if (g_door_open && (systick_get_tick() - g_door_open_tick >= 3000)) {
        gpio_write_pin(EXTERNAL_LED_ONOFF_PORT, EXTERNAL_LED_ONOFF_PIN, GPIO_PIN_RESET);
        uart2_send_string("Puerta cerrada automáticamente tras 3 segundos.\r\n");
        g_door_open = 0;
    }
    // 1. Control automático de iluminación
    if (g_lamp_on && (systick_get_tick() - g_door_open_tick >= 10000)) {
        tim3_ch1_pwm_set_duty_cycle(g_lamp_idle_brightness);
        uart2_send_string("Lámpara restaurada automáticamente tras 10 segundos.\r\n");
        g_lamp_on = 0;
    }

    // 7. Transición gradual de brillo
    if (g_lamp_animation && (systick_get_tick() - g_lamp_animation_tick >= 500)) {
        g_lamp_animation_tick = systick_get_tick();
        if (g_lamp_idle_brightness < 100) {
            g_lamp_idle_brightness += 10; // Incrementa el brillo en 10%
        } else {
            g_lamp_animation = 0; // Detiene la animación al llegar a 100%
            g_lamp_idle_brightness = 20; // Resetea a 20% si supera 100%
            uart2_send_string("Animación de lámpara completada.\r\n");
        }
        tim3_ch1_pwm_set_duty_cycle(g_lamp_idle_brightness);
    }
}

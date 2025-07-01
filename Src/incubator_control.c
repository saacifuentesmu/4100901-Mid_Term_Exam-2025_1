/**
 ******************************************************************************
 * @file           : incubator_control.c
 * @author         : [Estudiante]
 * @brief          : Lógica de la aplicación para el Controlador de Incubadora.
 ******************************************************************************
 */

#include "incubator_control.h"

#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"
#include "systick.h"
#include "uart.h"
#include "tim.h"
#include <stdio.h>
#include <string.h>


// --- Variables Globales ---
// Nota: 'volatile' se usa para variables que pueden ser modificadas por interrupciones.

// Umbral de temperatura para la alarma de sobrecalentamiento.
int32_t umbral_temperatura = 38;
// Temperatura simulada actual. El estudiante puede cambiarla para probar la alarma.
volatile int32_t temperatura_actual = 35;
// Flag para el estado de alarma.
bool en_alarma = false;

// --- Definiciones de pines y constantes ---
// Ventilador (PWM) -> PA6, TIM3_CH1
// Calefactor (GPIO) -> PB10
// #define HEATER_PORT GPIOB
// #define HEATER_PIN  GPIO_PIN_10

static uint8_t fan_speed = 20; // Velocidad inicial del ventilador

void incubator_app_init(void) {
    // Estado inicial de los actuadores
    gpio_write_pin(HEATER_PORT, HEATER_PIN, GPIO_PIN_RESET); // Calefactor apagado
    tim3_ch1_pwm_set_duty_cycle(fan_speed); // Ventilador a velocidad inicial

    // 4. Mensaje de bienvenida
    // OBJETIVO: Enviar el mensaje de bienvenida por UART.
    uart2_send_string("\r\nControlador de Incubadora v1.0\r\n");
    uart2_send_string("Listo.\r\n");
}

void incubator_on_button_press(void) {
    // Anti-rebote simple
    static uint32_t last_press_tick = 0;
    if (systick_get_tick() - last_press_tick < 300) {
        return;
    }
    last_press_tick = systick_get_tick();

    // Lógica a implementar por el estudiante
}

void incubator_on_uart_receive(char cmd) {
    // Buffer para procesar comandos de varios caracteres como 'TXX'
    static char command_buffer[4];
    static uint8_t buffer_idx = 0;

    if (cmd == '\r' || cmd == '\n') { // Fin del comando
        // Procesar comando si hay algo en el buffer (para TXX)
        if (buffer_idx > 0) {
            command_buffer[buffer_idx] = '\0'; // Terminar el string

            // 5. Comando para Configurar Umbral de Temperatura
            // OBJETIVO: Implementar la lógica para el comando 'TXX'
            if (command_buffer[0] == 'T') {
                // El estudiante debe implementar el parsing de 'TXX' aquí.
                uart2_send_string("Comando TXX recibido. Implementar parsing.\r\n");
            }

            buffer_idx = 0; // Resetear el buffer
        }
    } else if (buffer_idx > 0 || cmd == 'T' || cmd == 'v' || cmd == 'H' || cmd == 'h' || cmd == 'e' || cmd == 'r' || cmd == '?') {
        // Acumular caracteres para comandos largos, o procesar comandos de un caracter
        if (buffer_idx < sizeof(command_buffer) - 1) {
            command_buffer[buffer_idx++] = cmd;
        }
    }


    switch (cmd) {
        // Comandos de un solo caracter
        case 'v': // Este es un ejemplo, la lógica completa es para el estudiante.
            // La lógica para 'v0', 'v1', etc. debería manejarse aquí o en el buffer.
            break;

        // 6. Comando de Ayuda
        // OBJETIVO: Implementar el comando '?'
        case '?':
            uart2_send_string("Comando '?' recibido. Implementar ayuda.\r\n");
            break;

        // Agregue aquí el resto de los casos para los comandos de un solo caracter.
        // Por ejemplo: 'H', 'h', 'e', 'r'
    }
}


void incubator_run(void) {
    // Esta función se llama repetidamente desde el bucle principal en main.c

    // 1. Alarma de Sobrecalentamiento
    // OBJETIVO: Verificar si temperatura_actual > umbral_temperatura y activar la alarma.
    // Si la alarma está activa, hacer parpadear el LED y controlar los actuadores.


    // 7. Ciclo Automático de "Curado de Huevos"
    // OBJETIVO: Implementar la máquina de estados para el ciclo de curado.

}

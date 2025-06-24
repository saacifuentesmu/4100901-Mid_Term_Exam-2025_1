/**
 ******************************************************************************
 * @file           : incubator_control.h
 * @author         : [Estudiante]
 * @brief          : Header file for Incubator Control Application
 ******************************************************************************
 */
#ifndef INCUBATOR_CONTROL_H
#define INCUBATOR_CONTROL_H

#include <stdint.h>
#include <stdbool.h>

// Temperatura simulada actual de la incubadora. El estudiante puede modificarla para probar.
extern volatile int32_t temperatura_actual;
// Umbral de temperatura para la alarma de sobrecalentamiento.
extern int32_t umbral_temperatura;
// Estado de la alarma
extern bool en_alarma;

/**
 * @brief Función a ser llamada por EXTI15_10_IRQHandler cuando se detecta
 *        la pulsación del botón B1 (si se usa).
 */
void incubator_on_button_press(void);

/**
 * @brief Función a ser llamada por USART2_IRQHandler cuando se recibe un carácter.
 * @param received_char El carácter recibido por UART.
 */
void incubator_on_uart_receive(char received_char);

/**
 * @brief Función a ser llamada periódicamente por el bucle principal para
 *        ejecutar la lógica de control (máquinas de estado, etc).
 */
void incubator_run(void);

/**
 * @brief Función para realizar inicializaciones específicas de la aplicación,
 *        como el mensaje de bienvenida y el estado inicial de las variables.
 */
void incubator_app_init(void);

#endif // INCUBATOR_CONTROL_H

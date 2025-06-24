/**
 ******************************************************************************
 * @file           : tim.h
 * @author         : Sam C
 * @brief          : Header file for Timer driver for STM32L476RGTx
 ******************************************************************************
 */
#ifndef TIM_H
#define TIM_H

#include <stdint.h>


typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMCR;
    volatile uint32_t DIER;
    volatile uint32_t SR;
    volatile uint32_t EGR;
    volatile uint32_t CCMR1;
    volatile uint32_t CCMR2;
    volatile uint32_t CCER;
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
    volatile uint32_t RESERVED1;
    volatile uint32_t CCR1;
    volatile uint32_t CCR2;
    volatile uint32_t CCR3;
    volatile uint32_t CCR4;
    volatile uint32_t RESERVED2;
    volatile uint32_t DCR;
    volatile uint32_t DMAR;
} TIM_TypeDef;


#define TIM3_BASE           (0x40000400UL) // TIM3 está en APB1
#define TIM3                ((TIM_TypeDef *) TIM3_BASE)

#define TIM4_BASE           (0x40000800UL) // TIM4 está en APB1
#define TIM4                ((TIM_TypeDef *) TIM4_BASE)


void tim3_ch1_pwm_init(uint32_t pwm_freq_hz);
void tim3_ch1_pwm_set_duty_cycle(uint8_t duty_cycle_percent); // duty_cycle en % (0-100)

/**
 * @brief Configura el TIM4 Canal 1 para operar en modo PWM.
 *        El estudiante debe implementar esta función.
 * @param pwm_freq_hz Frecuencia deseada para la señal PWM.
 */
void tim4_ch1_pwm_init(uint32_t pwm_freq_hz);

/**
 * @brief Establece el ciclo de trabajo para el TIM4 Canal 1.
 *        El estudiante debe implementar esta función.
 * @param duty_cycle_percent Ciclo de trabajo en % (0-100).
 */
void tim4_ch1_pwm_set_duty_cycle(uint8_t duty_cycle_percent);

#endif // TIM_H

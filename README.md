### Examen Práctico Alternativo: Controlador de Incubadora

**Curso:** 4100901 - Estructuras Computacionales

**Condiciones:** Supervisado, con acceso al código fuente y a internet (sin uso de IA)

---

### ✅ Actividades Técnicas a Realizar (100 minutos)

#### 1. Alarma de Sobrecalentamiento y Acción de Emergencia [20%]
El sistema debe monitorear continuamente una variable global `temperatura_actual`. Si esta variable supera un `UMBRAL_TEMPERATURA` (inicialmente 38°C):
- El LED de usuario (LD2, en `PA5`) debe empezar a parpadear a 2Hz (250ms encendido, 250ms apagado).
- El ventilador debe ponerse al 100% de velocidad para enfriar.
- El calefactor debe apagarse por seguridad.
Este estado de "emergencia" debe persistir hasta que el usuario envíe el comando `'r'` (reset) por UART.

*   **Recomendación:** Use `systick_get_tick()` para implementar el parpadeo de forma no bloqueante.

---
#### 2. Aumento de la Velocidad de Comunicación [10%]
Modifique la configuración de la UART para que la comunicación se realice a **115200 baudios** en lugar de los 9600 por defecto.

*   **Recomendación:** Consulte la sección "USART baud rate generation" del manual de referencia para calcular el valor correcto del registro `USART_BRR`.

---
#### 3. Control Avanzado del Calefactor con un Nuevo Timer [15%]
Actualmente, el calefactor se maneja como un pin digital simple (ON/OFF). Modifique el sistema para:
1.  Configurar el pin `PB6` para que use su función alternativa como `TIM4_CH1`.
2.  Configurar el periférico **Timer 4** para generar una señal PWM de 1 Hz.
3.  Implementar dos comandos UART: `'H'` para poner el calefactor a un 50% de su ciclo de trabajo (simulando media potencia) y `'h'` para apagarlo (0%).

*   **Recomendación:** No olvide habilitar el reloj para el periférico TIM4 y configurar el GPIO en modo de función alternativa.

---
#### 4. Mensaje de Bienvenida [5%]
Al iniciar, mostrar por UART un mensaje de bienvenida estático.
```
Controlador de Incubadora v1.0
Listo.
```

---
#### 5. Comando para Configurar Umbral de Temperatura [15%]
Implemente un comando UART que permita al usuario cambiar el `UMBRAL_TEMPERATURA`. El formato del comando será `TXX`, donde `XX` son dos dígitos decimales. Por ejemplo, enviar `T40` por la terminal debe actualizar el umbral a 40°C.

*   **Recomendación:** Recuerde que los datos recibidos por UART son caracteres. Deberá convertirlos a un valor numérico.

---
#### 6. Comando de Ayuda Dinámico [10%]
Implemente el comando `'?'` que muestre los comandos disponibles y el valor *actual* del umbral de temperatura.
```
Comandos:
 v0..v4: Vel. ventilador (0,25,50,75,100%)
 H/h: Calefactor (50% / 0%)
 TXX: Umbral temp. (actual: 38 C)
 e: Iniciar ciclo de curado
 r: Resetear alarma
 ?: Ayuda
```

---
#### 7. Ciclo Automático de "Curado de Huevos" [25%]
Implemente el comando `'e'` (egg) que inicie un ciclo automático de 60 segundos dividido en tres fases distintas, controlando tanto el ventilador como el calefactor:
- **Fase 1 (0-20s):** Calefactor al 70%, Ventilador al 30%.
- **Fase 2 (20-40s):** Calefactor al 50%, Ventilador al 50%.
- **Fase 3 (40-60s):** Calefactor al 30%, Ventilador al 70%.
Al finalizar el ciclo, ambos deben apagarse.

*   **Recomendación:** Utilice una máquina de estados y `systick_get_tick()` para gestionar las fases y los tiempos. 
### Examen Práctico Alternativo: Controlador de Incubadora

**Curso:** 4100901 - Estructuras Computacionales

**Condiciones:** Supervisado, con acceso al código fuente y a internet (sin uso de IA)

---

### ✅ Actividades Técnicas a Realizar (100 minutos)

#### 1. Alarma de Sobrecalentamiento y Acción de Emergencia [40%]
El sistema debe monitorear continuamente una variable global `temperatura_actual`. Si esta variable supera un `UMBRAL_TEMPERATURA` (inicialmente 38°C):
- El LED de usuario (LD2, en `PA5`) debe empezar a parpadear a 2Hz (250ms encendido, 250ms apagado).
- El ventilador debe ponerse al 100% de velocidad para enfriar.
- El calefactor debe apagarse por seguridad.
Este estado de "emergencia" debe persistir hasta que el usuario envíe el comando `'r'` (reset) por UART.

*   **Recomendación:** Use `systick_get_tick()` para implementar el parpadeo de forma no bloqueante.

---
#### 2. Control Avanzado del Calefactor con un Nuevo Timer [30%]
Actualmente, el calefactor se maneja como un pin digital simple (ON/OFF). Modifique el sistema para:
1.  Configurar el pin `PB6` para que use su función alternativa como `TIM4_CH1`.
2.  Configurar el periférico **Timer 4** para generar una señal PWM de 1 Hz.
3.  Implementar dos comandos UART: `'H'` para poner el calefactor a un 50% de su ciclo de trabajo (simulando media potencia) y `'h'` para apagarlo (0%).

*   **Recomendación:** No olvide habilitar el reloj para el periférico TIM4 y configurar el GPIO en modo de función alternativa.

---
#### 3. Comando para Configurar Umbral de Temperatura [20%]
Implemente un comando UART que permita al usuario cambiar el `UMBRAL_TEMPERATURA`. El formato del comando será `X`, donde `X` es un dígito decimal que regula la temperatura de 20°C a 40°C en pasos de 2°C. Por ejemplo, enviar `4` por la terminal debe actualizar el umbral a 28°C.

*   **Recomendación:** Recuerde que los datos recibidos por UART son caracteres. Deberá convertirlos a un valor numérico.

---
#### 4. Comando de Ayuda Dinámico [10%]
Implemente el comando `'?'` que muestre los comandos disponibles y el valor *actual* del umbral de temperatura.
```
Comandos:
 H/h: Calefactor (50% / 0%)
 X: Umbral temp (Actual: X).
 r: Resetear alarma
 ?: Ayuda
```

/*
 * @file main.c
 * @brief
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#ifdef __USE_MCUEXPRESSO
#include <cr_section_macros.h> /* MCUXpresso-specific macros */
#endif

#include "lpc17xx_gpio.h"    /* GPIO handling */
#include "lpc17xx_pinsel.h"  /* Pin function selection */
#include "lpc17xx_systick.h" /* SysTick handling */
#include "lpc17xx_exti.h"    /* External interrupt handling */
#include "lpc17xx_nvic.h"

/*

EJERCICIO 1-
Utilizando Systick e interrupciones externas escribir un código en C que cuente indefinidamente de 0
a 9. Un pulsador conectado a Eint0 reiniciará la cuenta a 0 y se mantendrá en ese valor mientras el
pulsador se encuentre presionado. Un pulsador conectado a Eint1 permitirá detener o continuar la
cuenta cada vez que sea presionado. Un pulsador conectado a Eint2 permitirá modificar la velocidad
de incremento del contador. En este sentido, cada vez que se presione ese pulsador el contador pasará
a incrementar su cuenta de cada 1 segundo a cada 1 milisegundo y viceversa. Considerar que el
microcontrolador se encuentra funcionando con un reloj (cclk) de 16 Mhz. El código debe estar
debidamente comentado y los cálculos realizados claramente expresados. En la siguiente figura se
muestra una tabla que codifica el display y el esquema del hardware sobre el que funcionará el
programa.

*/
/**
 * @brief Main function.
 */

int main (void)
{
  SystemInit(); //---------------------------------->
  config_pins();
  config_extInt();
  config_SysTick();
  
    while (1)
    {
        /* Wait for SysTick interrupt */
    }

    return 0; /* Program should never reach this point */
}

void config_pins() {
  // CONFIGURACION DEL BOTON DE RESET
  PINSEL_CFG_Type pinReset;
}

void config_extInt() {

}

void config_SysTick() {

}

void SysTick_Handler();
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

#define OUTPUT 0
#define INPUT 1

#define FALSE 0
#define TRUE 1

#define INT_BUTTON_RESET ((uint32_t)(1 << 10))
#define INT_BUTTON_PAUSE_CONTINUE ((uint32_t)(1 << 11))
#define INT_BUTTON_SPEED ((uint32_t)(1 << 12))

uint32_t aux[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67} // Numeros en hexa del display del 0 al 9

/*
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
void config_port(void);
void config_extInt(void);
int main(void)
{
  SystemInit();
  config_port();
  config_extInt();
  NVIC_SetPriority();

    while (1)
    {
        /* Wait for SysTick interrupt */
    }

    return 0; /* Program should never reach this point */
}

void config_port()
{
  // CONFIGURACION DEL BOTON DE RESET
  PINSEL_CFG_Type pin210;
  pin210.Portnum = PINSEL_PORT_2;
  pin210.Pinnum = PINSEL_PIN_10;
  pin210.Funcnum = PINSEL_FUNC_1;
  pin210.Pinmode = PINSEL_PINMODE_PULLDOWN;
 // pin210.Opendrain = FALSE;
 PINSEL_ConfigPin(&pin210);
 GPIO_SetDir(PINSEL_PORT_2, INT_BUTTON_RESET, INPUT);

 // CONFIGURACION DEL BOTON DE PAUSE
 PINSEL_CFG_Type pin211;
 pin211.Portnum = PINSEL_PORT_2;
 pin211.Pinnum = PINSEL_PIN_11;
 pin211.Funcnum = PINSEL_FUNC_1;
 pin211.Pinmode = PINSEL_PINMODE_PULLDOWN;

 PINSEL_ConfigPin(&pin211);
 GPIO_SetDir(PINSEL_PORT_2, INT_BUTTON_PAUSE_CONTINUE, INPUT);
 // CONFIGURACION DEL BOTON DE VELOCIDAD
 PINSEL_CFG_Type pin212;
 pin212.Portnum = PINSEL_PORT_2;
 pin212.Pinnum = PINSEL_PIN_12;
 pin212.Funcnum = PINSEL_FUNC_1;
 pin212.Pinmode = PINSEL_PINMODE_PULLDOWN;

 PINSEL_ConfigPin(&pin212);
 GPIO_SetDir(PINSEL_PORT_2, INT_BUTTON_SPEED, INPUT);

}

void config_extInt()
{
  
  EXTI_Init();
  // CONFIGURACION DE LA INTERRUPCION EXTERNA POR EL BOTON DE RESET
  EXTI_InitTypeDef intExt0;
  intExt0.EXTI_Line = EXTI_EINT0;
  intExt0.EXTI_Mode = EXTI_MODE_EDGE_SENSITIVE;
  intExt0.EXTI_polarity = EXTI_POLARITY_HIGH_ACTIVE_OR_RISING_EDGE;

  EXTI_Config(&intExt0);
  NVIC_EnableIRQ(EINT1_IRQn);

  // CONFIGURACION DE LA INTERRUPCION EXTERNA POR EL BOTON DE PAUSA
  EXTI_InitTypeDef intExt1;
  intExt1.EXTI_Line = EXTI_EINT1;
  intExt1.EXTI_Mode = EXTI_MODE_EDGE_SENSITIVE;
  intExt1.EXTI_polarity = EXTI_POLARITY_HIGH_ACTIVE_OR_RISING_EDGE;

  EXTI_Config(&intExt1);
  NVIC_EnableIRQ(EINT1_IRQn);

  
}
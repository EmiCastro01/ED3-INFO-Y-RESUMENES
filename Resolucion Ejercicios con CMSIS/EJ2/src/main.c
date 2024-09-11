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

#define PIN_BIT0_INPUT ((uint32_t)(1 << 0))
#define PIN_BIT1_INPUT ((uint32_t)(1 << 1))
#define PIN_BIT2_INPUT ((uint32_t)(1 << 2))
#define PIN_BIT3_INPUT ((uint32_t)(1 << 3))

#define FALLING_EDGE 1
#define RISING_EDGE 0

#define INPUT 0
#define OUTPUT 1
/*
EJERCICIO 2- 
 Utilizando interrupciones por GPIO realizar un código en C que permita, mediante 4 pines de 
entrada GPIO, leer y guardar un número compuesto por 4 bits. Dicho número puede ser 
cambiado por un usuario mediante 4 switches, los cuales cuentan con sus respectivas 
resistencias de pull up externas. El almacenamiento debe realizarse en una variable del tipo 
array de forma tal que se asegure tener disponible siempre los últimos 10 números elegidos 
por el usuario, garantizando además que el número ingresado más antiguo, de este conjunto 
de 10, se encuentre en el elemento 9 y el número actual en el elemento 0 de dicho array.  La 
interrupción por GPIO empezará teniendo la máxima prioridad de interrupción posible y cada 
200 números ingresados deberá disminuir en 1 su prioridad hasta alcanzar la mínima posible. 
Llegado este momento, el programa deshabilitará todo tipo de interrupciones producidas por 
las entradas GPIO. Tener en cuenta que el código debe estar debidamente comentado.  
*/
/**
 * @brief Main function.
 */
int index = 0; 
  uint8_t data[10] = {};
  void configure_pins(void);
  void configure_interr(void);
int main (void)
{
  SystemInit();
  configure_pins();
  configure_interr();

  
    while (1)
    {
        manage_data_with_switches();
    }

    return 0; /* Program should never reach this point */
}



  void configure_pins(void)
  {
    //  Using port 0 for interrupting by GPIO
    PINSEL_CFG_Type pins_entry_data;
    pins_entry_data.Portnum = PINSEL_PORT_0;
    pins_entry_data.Funcnum = PINSEL_FUNC_0;
    pins_entry_data.Pinmode = PINSEL_PINMODE_PULLDOWN;
    for(int i = 0; i < 3; i++){
      pins_entry_data.Pinnum = i;
      PINSEL_ConfigPin(&pins_entry_data);
    }
    GPIO_SetDir(PINSEL_PORT_0, (0x0F), INPUT);

    // Using port 1 for managing switches info
    PINSEL_CFG_Type pins_switches;
    pins_switches.Portnum = PINSEL_PORT_1;
    pins_switches.Funcnum = PINSEL_FUNC_0;
    pins_switches.Pinmode = PINSEL_PINMODE_PULLUP;
    for(int i =; i < 3; i++){
      pins_switches.Pinnum = i;
      PINSEL_ConfigPin(&pins_switches);
    }
    GPIO_SetDir(PINSEL_PORT_1, (0x0F), INPUT);

    //return
    return;
  }

  void configure_interr(void)
  {
    GPIO_IntCmd(PINSEL_PORT_0, PIN_BIT0_INPUT | PIN_BIT1_INPUT | PIN_BIT2_INPUT | PIN_BIT3_INPUT, RISING_EDGE);
    NVIC_EnableIRQ(EINT3_IRQn);
    return;
  }

  void EINT3_IRQHandler(void) 
  {
    if(GPIO_GetIntStatus(PINSEL_PORT_0,  PIN_BIT0_INPUT | PIN_BIT1_INPUT | PIN_BIT2_INPUT | PIN_BIT3_INPUT, ENABLE)){
      write_data_from_port();
    }
    GPIO_ClearInt(PINSEL_PORT_0,  PIN_BIT0_INPUT | PIN_BIT1_INPUT | PIN_BIT2_INPUT | PIN_BIT3_INPUT);
    EXTI_ClearEXTIFlag(EXTI_EINT3);
    return;
  }

  void write_data_from_port (void)
  {

    data[index] = GPIO_ReadValue(PINSEL_PORT_0 & (0x0F));
   if(index == 10){
    index = 0;
   }


  }
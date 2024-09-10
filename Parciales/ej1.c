/*
===============================================================================
 Name        : PRIMER_PARCIAL_EJERCICIO1.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description :

 EJERCICIO 1

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



EN LA IMAGEN SE PUEDE VER EL PIN P210 A VCC
Y LOS PINES P11 Y P12 A MASA

===============================================================================
*/

/*
#include "LPC17xx.h"
#include "LPC17xx_gpio.h"
#include "LPC17xx_pinsel.h"
#include "LPC17xx_systick.h"
#include "LPC17xx_exti.h"

*/

#define OUTPUT (uint8_t) 1
#define INPUT  (uint8_t) 0


uint32_t aux[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67};

int cuenta = 0;           // Contador principal
int pausa = 0;            // Estado de pausa (0 = contando, 1 = pausado)
int velocidad = 150;     // Velocidad en milisegundos (1000ms o 1ms)
int reset_flag = 0;    	// Indica si el botón de reset está presionado




void retardo(int count);

void confGpio(void);
void confIntExt(void);
//void confIntSystick(void);

int main(void){
	confGpio();
	confIntExt();
	confIntSystick();

	while(1){

		if (!pausa && !reset_flag) {
		            GPIO_ClearValue(0, 0x7F);                 // Limpiar el display
		            GPIO_SetValue(0, aux[cuenta] & 0x7F);     // Mostrar el número actual en el display
		        }

		// Si se presiona el botón de reset y luego se suelta
		        if (reset_flag && !(GPIO_ReadValue(2) & (1 << 10))) {
		            reset_flag = 0;  // Se restablece el flag de reset al soltar el botón
		        }



	}





	return 0;
}

void confGpio(void){
	//configurar los pines de las interrupciones EINT0,1 Y 2

	//CONFIGURACION DE LOS PUERTOS DE ENTRADA QUE GENERAN LA INTERRUPCION.
	PINSEL_CFG_Type ConfP210;

	ConfP210.Portnum = PINSEL_PORT_2;
	ConfP210.Pinnum  = PINSEL_PIN_10;
	ConfP210.Funcnum = PINSEL_FUNC_1;
	ConfP210.Pinmode = PINSEL_PINMODE_PULLDOWN;

	PINSEL_ConfigPin(&ConfP210);
	GPIO_SetDir(2, (1<<10), INPUT);

	PINSEL_CFG_Type ConfP211;
	ConfP211.Portnum = PINSEL_PORT_2;
	ConfP211.Pinnum  = PINSEL_PIN_11;
	ConfP211.Funcnum = PINSEL_FUNC_1;
	ConfP211.Pinmode = PINSEL_PINMODE_PULLUP;
	PINSEL_ConfigPin(&ConfP211);
	GPIO_SetDir(2, (1<<11), INPUT);

	PINSEL_CFG_Type ConfP212;
	ConfP212.Portnum = PINSEL_PORT_2;
	ConfP212.Pinnum  = PINSEL_PIN_12;
	ConfP212.Funcnum = PINSEL_FUNC_1;
	ConfP212.Pinmode = PINSEL_PINMODE_PULLUP;
	PINSEL_ConfigPin(&ConfP212);
	GPIO_SetDir(2, (1<<12), INPUT);

	//CONFIGURACION DE LOS PUERTOS DE SALIDA, DEL P0.0 AL P0.6 PARA CUMPLIR CON EL DISPLAY DE 7 SEGMENTOS
	PINSEL_CFG_Type ConfP0a6;
	ConfP0a6.Portnum = PINSEL_PORT_0;
	ConfP0a6.Funcnum = PINSEL_FUNC_0;

	for (int i=0; i<7; i++){
		ConfP0a6.Pinnum = i;
	}
	PINSEL_ConfigPin(&ConfP0a6);
	GPIO_SetDir(0, (0x7F), OUTPUT);

}

void confIntExt(void){

	EXTI_Init();

	//EINT0

	EXTI_InitTypeDef intExt0;
	intExt0.EXTI_Line = EXTI_EINT0;
	intExt0.EXTI_Mode = EXTI_MODE_EDGE_SENSITIVE;
	intExt0.EXTI_polarity = EXTI_POLARITY_HIGH_ACTIVE_OR_RISING_EDGE;

	EXTI_Config(&intExt0);
	NVIC_EnableIRQ(EINT0_IRQn);

	//EINT1

	EXTI_InitTypeDef intExt1;
	intExt1.EXTI_Line = EXTI_EINT1;
	intExt1.EXTI_Mode = EXTI_MODE_EDGE_SENSITIVE;
	intExt1.EXTI_polarity = EXTI_POLARITY_LOW_ACTIVE_OR_FALLING_EDGE;

	EXTI_Config(&intExt1);
	NVIC_EnableIRQ(EINT1_IRQn);

	//EINT2

	EXTI_InitTypeDef intExt2;
	intExt2.EXTI_Line = EXTI_EINT2;
	intExt2.EXTI_Mode = EXTI_MODE_EDGE_SENSITIVE;
	intExt2.EXTI_polarity = EXTI_POLARITY_LOW_ACTIVE_OR_FALLING_EDGE;

	EXTI_Config(&intExt2);
	NVIC_EnableIRQ(EINT2_IRQn);

	return;

}

void confIntSystick(void){

	SYSTICK_Cmd(1);
	SYSTICK_IntCmd(1);
	SYSTICK_InternalInit(velocidad); //10miliseguntos

}

void SysTick_Handler(void){

	 if (!pausa && !reset_flag) {
	        cuenta = (cuenta + 1) % 10;  // Incrementar la cuenta de 0 a 9
	    }


}

void EINT0_IRQHandler(void){

	// Reinicia la cuenta a 0 y la mantiene mientras el botón esté presionado

	cuenta = 0;
	reset_flag = 1;

	EXTI_ClearEXTIFlag(EXTI_EINT0);

	return;

}

void EINT1_IRQHandler(void){

	 // Cambia el estado de pausa

	 pausa = !pausa;

	EXTI_ClearEXTIFlag(EXTI_EINT1);

	return;

}

void EINT2_IRQHandler(void){

	    // Cambia la velocidad entre 1 segundo y 1 milisegundo

	    if (velocidad == 150) {
	        velocidad = 10;
	    } else {
	        velocidad = 150;
	    }

	SYSTICK_InternalInit(velocidad);
	EXTI_ClearEXTIFlag(EXTI_EINT2);

	return;
}

void retardo (int count){

	for(int u=0;u<count; u++){}

}
/*
 * File:   LCD.c
 * Author: RAMIR
 *
 * Created on March 6, 2022, 10:57 PM
 */

#include "LCD.h"

void LCD_Inicar (){
    LCD_E = 0; // Se activa y se apaga para pasar informacion al lcd
    LCD_RS = 0;// Se activa para imprimir en pantalla 
    BUS = 0;   // PORTA en 0
    
    // Ciclo recomendado para activar configuracion del LCD
    __delay_ms (16);
    LCD_Instruccion (0b00110000);
    __delay_ms (5);
    LCD_Instruccion (0b00110000);
    __delay_us (200);
    LCD_Instruccion (0b00110000);
    
    // Reset
    __delay_ms (100);
    LCD_Instruccion(0b00111000);
    __delay_ms (100);
    LCD_Instruccion(0b00001100);
    /*LCD_Instruccion (0b00111000); // Activar visualizador
    LCD_Instruccion (0b00001100); // Desplazar cursor
    LCD_Instruccion (0b00001111); // Desplazar cursor
    */
    return;
}

// Para subir instrucciones
void LCD_Instruccion (uint8_t Comando){
    LCD_RS = 0;     // Las instrucciones en RS 0
    BUS = Comando;  // Pasar valor al BUS
    LCD_Clock_I();  // Clocazo en E
    return;
}

void LCD_Imprimir_Char(uint8_t Comando){
    LCD_RS = 1;     // Para imprimir char
    BUS = Comando;  // Pasar valor apropiado a PORTA   
    LCD_Clock_I();  // Clocazo
    return;
}

// La funcion capta un texto y lo recorre completo
void LCD_Imprimir_Texto(char a[]){
    uint8_t i;
    for (i = 0; i < strlen(a); i++){
        LCD_Imprimir_Char(a[i]); //Imprime char por
    }
    return;
}

void LCD_Clock_I (void){
    LCD_E = 1;
    __delay_ms (E_delay);
    LCD_E = 0;
    return;
}

/*
void LCD_Limpiar (void){
    LCD_Instruccion(1);
    return;
}
*/

void LCD_Set_Cursor (unsigned char a,unsigned char b){
    
    switch(a){
        // Fila 0
        case  0:
            // 0b10000000 es de fila 1, en and es para la posicion
            LCD_Instruccion(0b10000000 + b);
            break;
        case 1:
            // 0b10000000 es de fila 2,en and es para la posicion
            LCD_Instruccion(0b11000000 + b);
            break;
        default:
            LCD_Instruccion(0b10000000 + b);
            break;
    }
    return;
}



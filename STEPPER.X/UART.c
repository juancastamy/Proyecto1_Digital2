
#include <xc.h>
#include <stdint.h>
#include "UART.h"

char UART_INIT(const long int baudrate){
	{
  unsigned int x;
  x = (_XTAL_FREQ - baudrate*64)/(baudrate*64);   //SPBRG for Low Baud Rate
  if(x>255)                                       //If High Baud Rage Required
  {
    x = (_XTAL_FREQ - baudrate*16)/(baudrate*16); //SPBRG for High Baud Rate
    BRGH = 1;                                     //Setting High Baud Rate
  }
  if(x<256)
  {
    SPBRG = x;                                    //Writing SPBRG Register
    SYNC = 0;                                     //Setting Asynchronous Mode, ie UART
    SPEN = 1;                                     //Enables Serial Port
    TRISC7 = 1;                                   //As Prescribed in Datasheet
    TRISC6 = 1;                                   //As Prescribed in Datasheet
    CREN = 1;                                     //Enables Continuous Reception
    TXEN = 1;                                     //Enables Transmission
    return 1;                                     //Returns 1 to indicate Successful Completion
  }
  return 0;                                       //Returns 0 to indicate UART initialization failed
}
}
//******************************************************************************

//Funci�n para leer un caracter por UART

//******************************************************************************

char UART_READ(){
  while(!RCIF);
  return RCREG;
}
//******************************************************************************
//Funci�n para mandar un caracter por UART
//******************************************************************************
void UART_WRITE(char data){
    {
  while(!TRMT);
  TXREG = data;
}
}
/*
//******************************************************************************
//Funci�n para leer un string por UART
//******************************************************************************
void UART_Read_Text(char *Output, unsigned int length)
{
	unsigned int i;
	for(int i=0;i<length;i++)
		Output[i] = UART_Read();
}
//******************************************************************************
//Funci�n para mandar un string por UART
//******************************************************************************
void UART_Write_Text(char *text)
{
  int i;
  for(i=0;text[i]!='\0';i++) //Por cada caracter del string, mandelo y aumente el contador
	  UART_Write(text[i]);
}
*/









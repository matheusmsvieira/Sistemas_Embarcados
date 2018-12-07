gcc gpio_sysfs.c Ola_LCD.c -o olalcd

#include "gpio_sysfs.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>


#define EN 4
#define RS 27
#define D4 22
#define D5 23
#define D6 24
#define D7 25
#define DADO 1
#define COMANDO 0

void config_pins(void){
  setGPIO_Out(EN);
  setGPIO_Out(RS);
  setGPIO_Out(D4);
  setGPIO_Out(D5);
  setGPIO_Out(D6);
  setGPIO_Out(D7);
  }

void free_pins(void){
  unsetGPIO(EN);
  unsetGPIO(RS);
  unsetGPIO(D4);
  unsetGPIO(D5);
  unsetGPIO(D6);
  unsetGPIO(D7);
}

char send_nibble(char nibble, char nibble_type){
  if ((nibble_type != DADO)||(nibble_type != COMANDO))
    return -1;
  GPIO_Write(EN, 1); //nome do pino e valor
  GPIO_Write(RS, nibble_type);
  GPIO_Write(D4, nibble&1);
  GPIO_Write(D5, (nibble&2)>>1); // outra forma (nibble>>1)&1
  GPIO_Write(D6, (nibble>>2)&1);
  GPIO_Write(D7, (nibble>>3)&1);
  GPIO_Write(EN, 0);
  usleep(1000); // 1us
  return 0;
  }

char send_byte(char byte, char byte_type){
  if (send_nibble(byte>>4, byte_type)==-1)
    return -1;
  send_nibble(byte & 0xF, byte_type);
    return 0;
}
void clear_LCD(void){
 send_byte(0x01, COMANDO); //limpar display
 usleep(20000);
 send_byte(0x02, COMANDO); //inverter
 usleep(20000);
}

void config_LCD(void){
  usleep(10000);
  config_pins();
  send_nibble(0x2, COMANDO); //avisar que mandou só 4 bits
  send_byte(0x20, COMANDO);
  send_byte(0x14, COMANDO);
  send_byte(0xBC, COMANDO);
  send_byte(0xB5, COMANDO);
  clear_LCD();
}

void send_string(char *str){
  int i = 0;
  while(str[i]!='\0'){
    send_byte(str[i], DADO); //str[i++]
    i++;
    }
}


int main ()

  char str[200] = "12345";
  
  config_LCD();
  send_string("OLA LCD!", DADOS);
  while(strcmp(str, "EXIT")!=0){
    puts("Digite uma palavra (EXIT para sair):");
    scanf("%s", str);
    clear_LCD();
    send_string(str);
  }
  Free_Pins()
  return 0;

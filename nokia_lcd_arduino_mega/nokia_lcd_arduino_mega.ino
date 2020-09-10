#include <LCD5110_Basic.h>
 
LCD5110 tela(8,9,10,12,11);
/*Cria objeto da classe LCD5110
CLK – Pino 8
DIN – Pino 9
DC – Pino 10
RST – Pino 12
CE – Pino 11
*/
 
//Obtendo as fontes
extern uint8_t SmallFont[];
extern uint8_t MediumNumbers[];
extern uint8_t BigNumbers[];
 
void setup()
{
 tela.InitLCD(); //Inicializando o display
}
 
void loop()
{
 tela.setFont(SmallFont); //Definindo a fonte
 //Escreve a palavra vida alinhada à esquerda começando na linha 0
 tela.print("Vida", LEFT, 0);
 delay(500);
 tela.clrScr(); //Apaga o contéudo que estiver na tela
 tela.print("de", CENTER, 20);
 delay(500);
 tela.clrScr();
 tela.print("Silicio", RIGHT, 40);
 delay(500);
 tela.clrScr();
 delay(1000);
 
tela.print("VIDA", LEFT, 0);
 tela.print("DE", CENTER, 20);
 tela.print("SILICIO", RIGHT, 40);
 delay(1000);
 tela.clrScr();
 
tela.print("Numero medio:", LEFT, 0);
 tela.setFont(MediumNumbers);
 tela.printNumI(123, CENTER, 16);
 delay(2000);
 tela.clrScr();
 
tela.setFont(SmallFont);
 tela.print("Numero grande:", LEFT, 0);
 tela.setFont(BigNumbers);
 tela.printNumI(123, CENTER, 16);
 delay(2000);
 tela.clrScr();
 
}

#include <Arduino.h>
#include <math.h>

/*-----------Mapeamento de Hardware---------------*/
#define dac1    25
#define dac2    26
#define touch1  27
#define touch2  12

/*---------------Variáveis Globais----------------*/

double  canal1, 
        canal2,
        amplitude  = 1,
        offset     = 1.65,
        frequencia = 0.1,
        radianos   = 0;

/*---------------Constantes Globais----------------*/

long double constante_DAC = 255/3.3;

/*-----------Declaração de Funções---------------*/

bool le_touch(int);
long double seno(double,double,double);


/*-----------Configuralções Iniciais-------------*/

void setup() {
  Serial.begin(115200);

}

void loop() {
  /*
  Serial.print(le_touch(touch1));
  Serial.print("  ");
  Serial.println(le_touch(touch2));
  delay(100);
*/


  Serial.println(canal1);
  dacWrite(dac1,canal1);
  canal1=seno(offset,frequencia,amplitude)*(constante_DAC);
  delay(1);
  
}

/*-----------Implementação das Funções-------------*/

bool le_touch(int touch)
{
  return (touchRead(touch)<35);
}

long double seno(double offset, double frequencia,double amplitude)
{
  radianos = radianos + frequencia/1000;
  return (offset + (amplitude * sin(radianos*2*PI)));
}

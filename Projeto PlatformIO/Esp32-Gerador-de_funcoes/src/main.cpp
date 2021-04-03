#include <Arduino.h>
#include <math.h>

/*-----------Mapeamento de Hardware---------------*/
#define dac1    25
#define dac2    26
#define touch1  27
#define touch2  12

/*---------------Variáveis Globais----------------*/

double  canal1             ,canal2,
        amplitude1  = 1    ,amplitude2  = 1,
        offset1     = 1.65 ,offset2     = 1.65,
        frequencia1 = 0.1  ,frequencia2 = 0.1,
        radianos1   = 0    ,radianos2   = 0;
        
        

       

/*---------------Constantes Globais----------------*/

long double constante_DAC = 255/3.3;

/*-----------Declaração de Funções---------------*/

bool le_touch(int);
long double seno(double,double,double,double &);


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


  //Serial.println(canal1);

  dacWrite(dac1,canal1);
  dacWrite(dac1,canal2);
  canal1=seno(offset1,frequencia1,amplitude1,radianos1)*(constante_DAC);
  canal2=seno(offset2,frequencia2,amplitude2,radianos2)*(constante_DAC);
  delay(1);
}

/*-----------Implementação das Funções-------------*/

bool le_touch(int touch)
{
  return (touchRead(touch)<35);
}

long double seno(double offset, double frequencia,double amplitude,double &radianos)
{
  radianos = radianos + frequencia/1000;
  return (offset + (amplitude * sin(radianos*2*PI)));
}

#include <Arduino.h>
#include <math.h>

/*-----------Mapeamento de Hardware---------------*/
#define dac1    25
#define dac2    26
#define touch1  27
#define touch2  12


double   resultado,
         amplitude  = 1,
         offset     = 0,
         frequencia = 1,
         grau=0;

/*-----------Mapeamento de Hardware---------------*/
bool le_touch(int);

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


  grau=(grau+(frequencia*360));
  if (grau>360)
  {Serial.print("======================");
    grau=0;
  }
  resultado = (offset+(amplitude * sin(grau*PI/180)));
  Serial.print(grau);
  Serial.print(":  ");
  Serial.println(resultado);
  delay(1000);
}

bool le_touch(int touch)
{
  return (touchRead(touch)<35);
}

#include <Arduino.h>
#include <math.h>
#include <LiquidCrystal.h>


/*-----------Mapeamento de Hardware---------------*/
#define dac1    26
#define dac2    25
#define t_volta 27
#define t_enter 12
#define t_mais  14
#define t_menos 13
#define lcd_rs  19
#define lcd_en  23
#define lcd_d4  18
#define lcd_d5  17
#define lcd_d6  16
#define lcd_d7  15


/*------Inicialização da biblioteca do display----*/
LiquidCrystal lcd(lcd_rs, lcd_en, lcd_d4, lcd_d5, lcd_d6, lcd_d7);


/*---------------Variáveis Globais----------------*/
double  canal1             ,canal2,
        amplitude1  = 1    ,amplitude2  = 1,
        offset1     = 1.65 ,offset2     = 1.65,
        frequencia1 = 0.1  ,frequencia2 = 0.1,
        radianos1   = 0    ,radianos2   = 0;
bool flag_touch;
int tempo_touch;
        
      
/*---------------Constantes Globais----------------*/
const long double constante_DAC = 255/3.3;
const int tempo_ms=200;
const int teclado_touch[4]= {t_volta,t_enter,t_mais,t_menos};


/*-----------Declaração de Funções---------------*/
int le_touch();
long double seno(double,double,double,double &);
long double dente_de_serra(double,double,double,double &);


/*------------Configurações Iniciais-------------*/
void setup() {
  pinMode(t_enter,INPUT);
  pinMode(t_volta,INPUT_PULLDOWN);
  pinMode(t_mais,INPUT_PULLDOWN);
  pinMode(t_menos,INPUT);
  Serial.begin(115200);
  lcd.begin(16, 2);
  lcd.print("Gerador de Funcoes");
}


/*-----------------Loop Infinito------------------*/
void loop() {
  /*
  dacWrite(dac1,canal1);
  dacWrite(dac1,canal2);
  canal1=dente_de_serra(offset1,frequencia1,amplitude1,radianos1)*(constante_DAC);
  canal2=seno(offset2,frequencia2,amplitude2,radianos2)*(constante_DAC);
  
  delay(1);
  */
  Serial.println(int(le_touch()));
}


/*-----------Implementação das Funções-------------*/
int le_touch(){
  if ((millis()-tempo_touch)>=tempo_ms)
  {
    for (int botao=0;botao<4;botao++)
    {
      if ((touchRead(teclado_touch[botao])<35)&&(touchRead(teclado_touch[botao])<35))
      {
        flag_touch=true;
        tempo_touch=millis();
        return teclado_touch[botao];
      }
    }
    return false;
  }
  else if (flag_touch)
  {
    flag_touch=false;
    tempo_touch=millis();
  }
  return false;
}

long double seno(double offset, double frequencia,double amplitude,double &radianos)
{
  radianos = radianos + frequencia/1000;
  return (offset + (amplitude * sin(radianos*2*PI)));
}

long double dente_de_serra(double offset, double frequencia,double amplitude,double &radianos)
{
  radianos = radianos + frequencia/1000;
  if (radianos>1000)
    radianos =0;

  return offset - amplitude + 2*amplitude*radianos/1000;
}

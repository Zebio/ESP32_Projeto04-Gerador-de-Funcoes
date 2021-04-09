#include <Arduino.h>
#include <math.h>
#include <LiquidCrystal.h>


/*-----------Mapeamento de Hardware---------------*/
#define dac1    26
#define dac2    25
#define t_volta 27
#define t_enter 13
#define t_mais  14
#define t_menos 12
#define lcd_rs  19
#define lcd_en  23
#define lcd_d4  18
#define lcd_d5  17
#define lcd_d6  16
#define lcd_d7  15

#define seno        0
#define dente       1
#define quadrada    2
#define triangular  3


/*------Inicialização da biblioteca do display----*/
LiquidCrystal lcd(lcd_rs, lcd_en, lcd_d4, lcd_d5, lcd_d6, lcd_d7);

      
/*---------------Constantes Globais----------------*/
const long double constante_DAC = 255/3.3;
const unsigned long tempo_ms=400;
const unsigned long tempo_10_ms = 10;
const int teclado_touch[4]= {t_volta,t_enter,t_mais,t_menos};
const int canal1=0;
const int canal2=1;


/*---------------Variáveis Globais----------------*/
double  canal[2],
        amplitude[2]  ={0.5,0.5},
        offset[2]     ={2.8,2.8},
        frequencia[2] ={1,   1},
        radianos[2]   ={0,   0};
int     onda[2]       ={seno,seno};

bool flag_touch;
unsigned long tempo_touch;
bool canal_out[2]={false,false};
int botao_pressionado=0;
int estado_menu_principal=canal1;


/*-----------Declaração de Funções---------------*/
void ajuste_canal(int);
void lcd_ajusta_parametro(int,int);
void lcd_imprime_canal_parametro(int,int,bool);
void lcd_print_main();
bool le_touch();
bool delay_10ms();
long double senoide(int);
long double dente_de_serra(int);
long double onda_quadrada(int);
long double onda_triangular(int);


/*------------Configurações Iniciais-------------*/
void setup() {
  Serial.begin(115200);
  lcd.begin(16, 4);
  lcd.setBacklight(HIGH);
  lcd_print_main();
}


/*-----------------Loop Infinito------------------*/
void loop() {
  if(delay_10ms())
  {
    dacWrite(dac1,canal[canal1]);
    dacWrite(dac2,canal[canal2]);
    for(int canal_index=0;canal_index<2;canal_index++)
    {
      if (canal_out[canal_index])
      {
        switch (onda[canal_index])
        {
          case seno:
            canal[canal_index]=senoide(canal_index)*(constante_DAC);
            break;
          case dente:
            canal[canal_index]=dente_de_serra(canal_index)*(constante_DAC);
            break;
          case quadrada:
            canal[canal_index]=onda_quadrada(canal_index)*(constante_DAC);
            break;
          case triangular:
            canal[canal_index]=onda_triangular(canal_index)*(constante_DAC);
            break;
        }
      }
      else
        canal[canal_index]=0;
    }
  }
  if (le_touch())
  {
    if (botao_pressionado==t_mais||botao_pressionado==t_menos)
      estado_menu_principal=!estado_menu_principal;
    if (botao_pressionado==t_enter)
      ajuste_canal(estado_menu_principal);
    lcd_print_main();
  }
}

/*-----------Implementação das Funções-------------*/
void ajuste_canal (int canal)
{
  int parametro=0;
  lcd_imprime_canal_parametro(canal,parametro,false);
  while(botao_pressionado!=t_volta)
  {
    if (le_touch())
    {
      if (botao_pressionado==t_mais)
     {
        parametro--;
        if (parametro<0)
          parametro=4;
      }
      if (botao_pressionado==t_menos)
      {
        parametro++;
        if (parametro>4)
          parametro=0;
      }
      if (botao_pressionado==t_enter)
      {
        lcd_ajusta_parametro(canal,parametro);
        botao_pressionado=0;
      }
      lcd_imprime_canal_parametro(canal,parametro,false);
    }    
  }
}

void lcd_ajusta_parametro(int canal,int parametro)
{
  lcd_imprime_canal_parametro(canal,parametro,true);
  do
  {
    if(le_touch()&&(botao_pressionado==t_mais||botao_pressionado==t_menos))
    {
      bool flag=0;
      if (botao_pressionado==t_mais)
        flag=true;
      switch (parametro)
      {
        case 0:
          canal_out[canal]=!canal_out[canal];
          break;
        case 1:
          if (flag)
          {
            onda[canal]--;
            if (onda[canal]<0)
              onda[canal]=3;
          }
          else
          {
            onda[canal]++;
            if (onda[canal]>3)
              onda[canal]=0;
          }
          break;
        case 2:
          if (flag)
          {
            amplitude[canal]=(amplitude[canal]+0.05);
            if (amplitude[canal]>3.3)
              amplitude[canal]=3.3;
          }
          else
          {
            amplitude[canal]=(amplitude[canal]-0.05);
            if (amplitude[canal]<0)
              amplitude[canal]=0;
          }
          break;
        case 3:
          if (flag)
          {
            offset[canal]=(offset[canal]+0.05);
            if (offset[canal]>3.3)
              offset[canal]=3.3;
          }
          else
          {
            offset[canal]=(offset[canal]-0.05);
            if (offset[canal]<0)
              offset[canal]=0;
          }
          break;
          
        case 4:
          if (flag)
          {
            frequencia[canal]=(frequencia[canal]+1);
            if (frequencia[canal]>10000)
              frequencia[canal]=10000;
          }
          else
          {
            frequencia[canal]=(frequencia[canal]-1);
            if (frequencia[canal]<1)
              frequencia[canal]=1;
          }
          break;
      }
      lcd_imprime_canal_parametro(canal,parametro,true);
    }
  }while(botao_pressionado!=t_volta);
}

void lcd_imprime_canal_parametro(int canal,int parametro,bool ajuste)
{
  lcd.clear();
  lcd.print("CANAL");
  lcd.setCursor(5,0);
  if (canal==canal1)
    lcd.print("1:");
  else 
    lcd.print("2:");
  if (ajuste)
      lcd.print(" +-");
  lcd.setCursor(0,1);
  switch (parametro)
  {
    case 0:
    lcd.print("Output:");
    lcd.setCursor(7,1);
    if (canal_out[canal])
      lcd.print(" ON");
    else
      lcd.print(" OFF");
    break;
    case 1:
    lcd.print("Onda: ");
    lcd.setCursor(5,1);
    if (onda[canal]==seno)
      lcd.print("    senoide");
    else if (onda[canal]==dente)
      lcd.print("      dente");
    else if (onda[canal]==quadrada)
      lcd.print("   quadrada");
    else if (onda[canal]==triangular)
      lcd.print(" triangular");
    break;
    case 2:
      lcd.print("Amplitude:        V");
      lcd.setCursor(11,1);
      lcd.print(amplitude[canal]);
    break;
    case 3:
      lcd.print("Offset:     V");
      lcd.setCursor(11,1);
      lcd.print(offset[canal]);
    break;
    case 4:
      lcd.print("Freq:         Hz");
      lcd.setCursor(10,1);
      lcd.print(frequencia[canal]);
    break;
  }
}

void lcd_print_main()
{
  lcd.clear();
  lcd.print(" CANAL 1: ");
  lcd.setCursor(11,0);
  if (canal_out[canal1])
    lcd.print("ON");
  else
    lcd.print("OFF");
  
  lcd.setCursor(0,1);
  lcd.print(" CANAL 2: ");
  lcd.setCursor(11,1);
  if (canal_out[canal2])
    lcd.print("ON");
  else
    lcd.print("OFF");
  lcd.setCursor(0,estado_menu_principal);
  lcd.print(">");
}

bool le_touch(){
  if ((millis()-tempo_touch)>=tempo_ms)
  {
    for (int botao=0;botao<4;botao++)
    {
      if ((touchRead(teclado_touch[botao])<35)&&(touchRead(teclado_touch[botao])<35))
      {
        flag_touch=true;
        tempo_touch=millis();
        botao_pressionado=teclado_touch[botao];
        return true;
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

bool delay_10ms()
{
  static unsigned long tempo_ms;
  if ((millis()-tempo_ms)>=tempo_10_ms)
  {
    tempo_ms=millis();
    return true;
  }
  else  
    return false;
}

long double senoide(int canal)
{
  radianos[canal] = radianos[canal] + frequencia[canal]/100;
  return (offset[canal] + (amplitude[canal] * sin(radianos[canal]*2*PI)));
}

long double dente_de_serra(int canal)
{
  radianos[canal] = radianos[canal] + frequencia[canal]/100;
  if (radianos[canal]>=1)
    radianos[canal] =0;

  return offset[canal] - amplitude[canal] + 2*amplitude[canal]*radianos[canal];
}

long double onda_quadrada(int canal)
{
  radianos[canal] = radianos[canal] + (frequencia[canal]/100);
  if (radianos[canal]>=1)
    radianos[canal] =0;
  if (radianos[canal]<0.5)
    return offset[canal]-amplitude[canal];
  else 
    return offset[canal]+amplitude[canal];
}

long double onda_triangular(int canal)
{
  radianos[canal] = radianos[canal] + frequencia[canal]/100;
  if (radianos[canal]>=1)
    radianos[canal] =0;
  if (radianos[canal]<=0.5)
    return offset[canal] - amplitude[canal] + amplitude[canal]*radianos[canal]*2;
  else 
    return offset[canal] + amplitude[canal] - (amplitude[canal]*(radianos[canal]-0.5)*2);
}

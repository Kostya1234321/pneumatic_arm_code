#include "HX711.h"                                            // подключаем библиотеку для работы с тензодатчиком

#define DT  3                                               // Указываем номер вывода, к которому подключен вывод DT  датчика
#define SCK 2                                                // Указываем номер вывода, к которому подключен вывод SCK датчика
#define PIN_ON 10
#define PIN_OFF 11
#define Matric_ON 0
#define Matric_OFF 1


HX711 scale;                                                  // создаём объект scale

float calibration_factor = 200;                            // вводим калибровочный коэффициент
float units;                                                  // задаём переменную для измерений в граммах
float ounces;                                                 // задаём переменную для измерений в унциях


int signal_on_off;
int Time[2]={500,500};

void setup() {
  Serial.begin(9600);
  pinMode(PIN_OFF, OUTPUT);
  pinMode(PIN_ON, OUTPUT);

  scale.begin(DT, SCK);                                       // инициируем работу с датчиком
  scale.set_scale();                        // устанавливаем калибровочный коэффициент
  scale.tare();    
  scale.set_scale(calibration_factor);                        // устанавливаем калибровочный коэффициент
}

void loop() {

  if (Serial.available() > 0) {
    signal_on_off = Serial.read(); // Зчитує перше значення 
    
    if ((signal_on_off - '0') == Matric_ON) ON_OFF(PIN_OFF,Matric_ON); // В if 
    if ((signal_on_off - '0') == Matric_OFF) ON_OFF(PIN_ON,Matric_OFF);
    if (signal_on_off == 'A') Change_Time(Matric_ON);
    if (signal_on_off == 'B') Change_Time(Matric_OFF);
    if (signal_on_off == 'G') Change_gramm();
    if (signal_on_off == 'O') scale.tare();   
  }
}

void Change_gramm()
{
  ounces = scale.get_units(10);                               // получаем значение с датчика, усреднённое по 10 измерениям
  units = ounces * 0.035274;                                  // переводим вес из унций в граммы
  Serial.print(ounces);                                       // выводим в монитор последовательного порта вес в граммах
  Serial.println(" grams");                                   // выводим текст в монитор последовательного порт
}

void Change_Time(int Matric_OFF_ON) //
{
Time[Matric_OFF_ON]=Serial.parseInt();
}

void ON_OFF(int PIN_ON_OFF,int Matric_OFF_ON) {
  digitalWrite(PIN_ON_OFF, HIGH);
  delay(Time[Matric_OFF_ON]);
  digitalWrite(PIN_ON_OFF, LOW);
}

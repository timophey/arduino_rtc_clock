#include <RTC.h>
#include <TM1637Display.h>
const int CLK = 9; //Set the CLK pin connection to the display
const int DIO = 8; //Set the DIO pin connection to the display
TM1637Display display(CLK, DIO);  //set up the 4-Digit Display.
int  TM1637DisplayColon = false;
bool isTimeSet = false;

int led = 13; // фоторезистор питание
int ldr = 3; // фоторезистор датчик

RTC    time;
void setup() {
    delay(300);
    Serial.begin(9600);
         time.begin(RTC_DS3231);
    //display.setBrightness(0x0f);
    digitalWrite(led, HIGH); // включаем фоторезистор
   //time.settime(30,42,20,12,4,15,2);
   
}
void loop(){
//  if(millis()%1000==0){ // если прошлo пол секунды
//    
    if (Serial.available()) { //поступила команда с временем http://zelectro.cc/RTC_DS1307_arduino
        setTimeFromFormatString(Serial.readStringUntil('\n'));
        isTimeSet = true; //дата была задана  
    }
//    if(millis()%1000==0){ // если прошла 1 секунда
      time.gettime();
      int CurTimeHour = time.Hours;
      int segtoHourDec = CurTimeHour / 10;
      int segtoHour = CurTimeHour % 10;
      // show hour with points and minutes with leading zero
      uint8_t segto = (TM1637DisplayColon) ? (0x80 | display.encodeDigit(segtoHour)) : display.encodeDigit(segtoHour);
      uint8_t segtoDec = (segtoHourDec) ? display.encodeDigit(segtoHourDec) : 0x00;
      //display.showNumberDec(segtoHourDec,false,1,0);
      display.setSegments(&segtoDec,1,0);
      display.setSegments(&segto, 1, 1);
      display.showNumberDec(time.minutes,true,2,2);
      //if()
      //Serial.println(segtoHour);   
      //Serial.println(segtoHourDec);
//      Serial.println(time.seconds);
      //Serial.println(segto);
      //Serial.println(TM1637DisplayColon);/**/
//    }
    TM1637DisplayColon = !TM1637DisplayColon;
//  }
  // brightness
  int phr = analogRead(ldr);
  int br = phr / 128; if(!br) br=1;
  uint8_t br_s = (uint8_t) (br+7);
  display.setBrightness(br_s);
  Serial.println(phr);
  delay(500);
}

void setTimeFromFormatString(String timestr)
{
  //ДД.ММ.ГГ чч:мм:сс
  // YYYY-m-d H:i:s
  /*int day = timestr.substring(8, 10).toInt();
  int month = timestr.substring(5, 7).toInt();
  int year = timestr.substring(0, 4).toInt();
  int hours = timestr.substring(11, 13).toInt();
  int minutes = timestr.substring(14, 16).toInt();
  int seconds = timestr.substring(17, 19).toInt();*/
  int hours = timestr.substring(0, 2).toInt();
  int minutes = timestr.substring(3, 5).toInt();
  int seconds = timestr.substring(6, 8).toInt();  
  if(hours && minutes && seconds)
    time.settime(seconds,minutes,hours);//,day,month,year);
  // settime(секунды [, минуты [, часы [, день [, месяц [, год [, день недели]]]]]]):
  //TimeElements te;
  /*te.Second = seconds;
  te.Minute = minutes;
  te.Hour = hours;
  te.Day = day;
  te.Month = month;
  te.Year = year + 30;*/ //год в библиотеке отсчитывается с 1970. Мы хотим с 2000
  /*time_t timeVal = makeTime(te);
  RTC.set(timeVal);
  setTime(timeVal);*/
}

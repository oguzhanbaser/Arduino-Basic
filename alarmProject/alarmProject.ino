// Date and time functions using just software, based on millis() & timer

#include <Arduino.h>
#include <Wire.h>         // this #include still required because the RTClib depends on it
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>

//Değişken tanımları
int waitTime = 10;

bool alarmStatus = false, alarmSetButton = false, alarmSet = false, alarmWillSet = false;
bool stopped = false, buzerStatus = false, buzzerBlink = false;

unsigned long lastTime = 0, lastTime2 = 0;
unsigned int lastSec;
int ledPin = 13, butonPin = 3;

//LCD tanımı
LiquidCrystal_I2C lcd(0x27, 16, 2);

#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
#define Serial SerialUSB
#endif

//RTC ayarı
RTC_Millis rtc;

String alarmYear = "", alarmTime = "";

void setup () {
  Serial.begin(57600);
  // following line sets the RTC to the date & time this sketch was compiled
  rtc.begin(DateTime(F(__DATE__), F(__TIME__)));
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));

  lcd.init();

  //İnterruptlar durduruldu
  cli();

  //timer ayarları
  //--------------------------
  //timer1 1Hz' e ayarlanıyor
  //registerler sıfırlanır
  TCCR1A = 0;// TCCR1A register 0'lanıyor
  TCCR1B = 0;// TCCR1B register 0'lanıyor
  TCNT1  = 0;//sayac değeri  0'la
  // OCRxA karşılaştırma registeri 1Hz değer için ayarlanıyor
  //16 MHz osilatör,1Hz timer1 ın çalışma frekansı,1024 prescalar
  OCR1A = 1562;// = (16*10^6) / (1*1024) - 1 (değer 65536 dan küçük)
  //   CTC mod açılıyor.
  TCCR1B |= (1 << WGM12);
  //   CS10 ve CS12 bitleri 1024 prescaler için ayarlanıyor
  TCCR1B |= (1 << CS12) | (1 << CS10);
  // timer karşılaştırma interruptı aktifleştiriliyor
  TIMSK1 |= (1 << OCIE1A);

  //interruptlar tekrar başlattık
  sei();

  pinMode(ledPin, OUTPUT);
  pinMode(2, INPUT);
  pinMode(butonPin, INPUT);
  //pinMode(butonPin2, INPUT);

  // Turn on the blacklight and print a message.
  lcd.backlight();
  attachInterrupt(0, intFunction, RISING);
  lcd.clear();
  lcd.print("Waiting to start");

  lastSec = waitTime;
}

//timer fonksiyonu
ISR(TIMER1_COMPA_vect) {  //timer1 interrupt ı 1Hz de tetikleniyor.
  if (buzerStatus)
  {
    if (buzzerBlink)
    {
      noTone(11);
      buzzerBlink = false;
    } else {
      tone(11, 1000);
      buzzerBlink = true;
    }
  }
}

//interrupt fonksiyonu
//interrupt gelince çalışacak
void intFunction()
{
  //interrupt geldiğinde alarm kurulu ise
  if (alarmSet)
  {
    //Real time clock dan o anki verileri oku
    DateTime now = rtc.now();

    alarmYear = String(now.year()) + "/";
    alarmYear +=  String(now.month()) + "/";
    alarmYear += String(now.day());

    alarmTime = String(now.hour()) + ":";
    alarmTime += String(now.minute()) + ":";
    alarmTime += String(now.second());

    buzerStatus = true;   //buzzer durumunu aktif et
    alarmStatus = true;   //alarm durumunu aktif et
  }
}


//ana döngü
void loop () {

  //alarm kurmak için butonab asıldı mı?
  alarmSetButton = digitalRead(butonPin); 

  //alarm durumu aktif mi
  if (alarmStatus)
  {

    //1sn de bir ekranı güncelle
    if (millis() - lastTime > 1000)
    {
      digitalWrite(ledPin, !digitalRead(ledPin));
      lastTime = millis();

      //lcd yi güncelle
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(alarmYear);
      lcd.setCursor(7, 1);
      lcd.print(alarmTime);
    }

    //Butona alarm aktifken basılırsa alarmı durdur
    if (alarmSetButton)
    {
      lcd.clear();
      lcd.print("Alarm cleared");
      lcd.setCursor(0, 1);
      lcd.print("Waiting to start");

      alarmStatus = false;
      alarmSet = false;
      stopped = true;
      buzerStatus = false;            
      
      digitalWrite(ledPin, LOW);
      noTone(11);                     //buzzer i sustur
      while (digitalRead(butonPin));  //buton basılı iken bekle
    }

  } else if (!alarmWillSet) {
    //alarm duurmu aktif edilmemişken butona basılmışsa
    //geri sayıp alarmı kur
    //(bu kısım alternatifir)
    alarmWillSet = alarmSetButton;
    if (alarmWillSet)
    {
      lcd.clear();
      lcd.print("Alarm will");
      lcd.setCursor(0, 1);
      lcd.print("set");
      lcd.setCursor(4, 1);
      lcd.print(lastSec);
      lcd.setCursor(7, 1);
      lcd.print("sec");
    }
  }

  //alarm duurmu aktif edilmemişken butona basılmışsa
  //geri sayıp alarmı kur
  if (alarmWillSet)
  {
    if (millis() - lastTime2 > 1000)
    {
      lcd.clear();
      lcd.print("Alarm will");
      lcd.setCursor(0, 1);
      lcd.print("set");
      lcd.setCursor(4, 1);
      lcd.print(lastSec);
      lcd.setCursor(7, 1);
      lcd.print("sec");

      //geri sayma işlemi bitmişse alarmı kur
      if (lastSec <= 0)
      {
        alarmSet = true;
        alarmWillSet = false;
        lastSec = waitTime;
        lcd.clear();
        lcd.print("Alarm set");
      }

      lastSec--;

      lastTime2 = millis();
    }

  }

}


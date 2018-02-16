#include <Nextion.h>
#include <NextionPage.h>
#include <NextionProgressBar.h>
#include <NextionButton.h>
#include <NextionText.h>
#include <NextionDualStateButton.h>

#include <SoftwareSerial.h>
SoftwareSerial nextionSerial(10, 11); // RX, TX

Nextion nex(nextionSerial);
NextionPage page1(nex, 0, 0, "page0");
NextionPage page2(nex, 1, 0, "page1");
NextionButton btn1(nex, 0, 2, "b0");
NextionButton btn2(nex, 0, 3, "b1");
NextionText text1(nex, 0, 4, "t0");
NextionText text2(nex, 0, 5, "t1");
NextionText textPercent(nex, 1, 6, "t0");
NextionProgressBar progressBar(nex, 1, 5, "j0");
NextionDualStateButton swBtn(nex, 1, 4, "bt0");

float vol1, vol2;
unsigned long sendLastTime = 0;

int led1 = 13, led2 = 12;
int potPin1 = A0, potPin2 = A1, potPin3 = A3;

#define NUM_SAMPLES 10

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  nextionSerial.begin(9600);      //seri iletişim 9600 baud rate
  nex.init();                     //ile başlatılıyor

  btn1.attachCallback(&btn1Callback);   //buton 1 basılma eventi
  btn2.attachCallback(&btn2Callback);   //buton 2 basılma eventi  
  swBtn.attachCallback(&swCallback);    //switch basılma eventi

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  float sum1 = 0, sum2 = 0, sum3 = 0;
  int percent = 0;

  //analog değerin stabil olması için 
  //10 değeri toplayıp ortalamasını aldık
  for (int i = 0; i < NUM_SAMPLES; i++)
  {
    sum1 += analogRead(potPin1);
    sum2 += analogRead(potPin1);
    sum3 += analogRead(potPin3);
    delay(5);
  }

  sum1 /= NUM_SAMPLES;
  sum2 /= NUM_SAMPLES;
  sum3 /= NUM_SAMPLES;

  //analog değerleri gerilime çevirdik
  vol1 = sum1 * 5.0 / 1023.0;
  vol2 = sum2 * 5.0 / 1023.0;

  //belli zaman aralıklarında ekrandaki değerleri güncelledik
  if (millis() - sendLastTime > 1000)   //son veri yazdırma işleminden 1 sn geçti mi
  {
    char txt[20];           //değişkeni tanımladık
    String str = "Volt1: " + String(vol1) + " V";   //verimizi string tipinde oluşturduk
    str.toCharArray(txt, str.length());       //string tipindeki veriyi char [] tipinde değişkene aktadrık
    text1.setText(txt);                   //veriyi arayüzde yazdırdık

    str = "Volt2: " + String(vol2) + " V";
    str.toCharArray(txt, str.length());
    text2.setText(txt);

    percent = map(sum3, 0, 1023, 0, 100);
    str = "% " + String(percent) + " " + String(sum3);
    str.toCharArray(txt, str.length());
    textPercent.setText(txt);
    progressBar.setValue(percent);
    Serial.println(txt);

    sendLastTime = millis();
  }

  nex.poll();
}

void btn1Callback(NextionEventType type, INextionTouchable *widget)
{
if (type == NEX_EVENT_POP)      //buton bırakma eventi
  {
    digitalWrite(led1, HIGH);
  }
}

void btn2Callback(NextionEventType type, INextionTouchable *widget)
{
if (type == NEX_EVENT_POP)
  {
    digitalWrite(led1, LOW);
  }
}

void swCallback(NextionEventType type, INextionTouchable *widget)
{
  if (type == NEX_EVENT_PUSH)
  {
    bool isSwOn = swBtn.isActive();   //anahtar durumu
    digitalWrite(led2, isSwOn);   // anahtar durumunu lede yaz
  }
}

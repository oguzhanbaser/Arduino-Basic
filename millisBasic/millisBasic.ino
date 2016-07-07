//pin tanımları
int ledPin1 = 2, ledPin2 = 3, ledPin3 = 4;

//son millis değerini tutacağımız değişken
unsigned long lastTime1 = 0, lastTime2 = 0, lastTime3 = 0;

void setup() {
  // put your setup code here, to run once:

  //led pinlerini çıkış olarak tanımladık
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  //son millis değeri ile arada 1000 fark varsa 
  if (millis() - lastTime1 > 1000)
  {
    digitalWrite(ledPin1, !digitalRead(ledPin1)); //led durumunu değiştir
    lastTime1 = millis();                         //son değere o anki millis 
                                                  //değerini kaydet
  }

  //son millis değeri ile arada 500 fark varsa 
  if (millis() - lastTime2 > 500)
  {
    digitalWrite(ledPin2, !digitalRead(ledPin2)); //led durumunu değiştir
    lastTime2 = millis();                         //son değere o anki millis 
                                                  //değerini kaydet
  }

  //son millis değeri ile arada 1500 fark varsa 
  if (millis() - lastTime3 > 1500)
  {
    digitalWrite(ledPin3, !digitalRead(ledPin3)); //led durumunu değiştir
    lastTime3 = millis();                         //son değere o anki millis 
                                                  //değerini kaydet
  }
}

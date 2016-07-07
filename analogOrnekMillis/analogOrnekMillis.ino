//led tanımlamarı ve millis değerini tutacak değişken tanımları
int ledPin = 13, potPin = A0;   
unsigned long lastTime = 0, lastTimeSeriPort = 0, ledLastTime = 0;
int sayac = 0;                //3 örneği sayacak sayaç
double ortalama, deger = 0;   //ortalama için değişkenler

void setup() {
  // put your setup code here, to run once:

  //ledi çıkış potu giriş olarak atadık ve haberleşmeyi
  //başlattık
  pinMode(ledPin, OUTPUT);
  pinMode(potPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  //3 örneği 100 ms arlıklarla alalım
  if (millis() - lastTime > 100)
  {
    deger += analogRead(potPin);
    sayac++;
    if (sayac == 3)   //sayaç değeri 3 olmuşsa ortalamayı al
    {
      ortalama = (double)deger / 3.0;
      sayac = 0;    //değerleri sıfırla
      deger = 0;
    }
    lastTime = millis();    
  }

  //500 ms de bir ortalama değerini gönder
  if (millis() - lastTimeSeriPort > 500)
  {
    Serial.println(ortalama);
    lastTimeSeriPort = millis();
  }

  //eğer ortalama 512 den büyükse ledi yakıp söndür
  if (ortalama > 512)
  {
    if (millis() - ledLastTime > 1000)
    {
      digitalWrite(ledPin, !digitalRead(ledPin));
      ledLastTime = millis();
    }
  }
}

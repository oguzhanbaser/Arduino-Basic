#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>

#ifdef __AVR__
#include <avr/power.h>
#endif

#define DEBUG         //haberleşme mesajlarını göster
#define NUM_LEDS 16   //Neopixel led saysıı
#define PIN 4         //Neopixel in bağlı olduğu digital pin

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
SoftwareSerial btSerial(10, 11);

bool adaptive = true;
uint8_t rVal, gVal , bVal, lastBright = 0, brVal1 = 0, setBright = 0;
int effectIndex = 0;

void setup() {
  // put your setup code here, to run once:
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif

  strip.begin();
  strip.show();
  btSerial.begin(9600);         //bluetooth haberleşmesi başlattık
  Serial.begin(9600);           //usb seri haberleşmeyi başlattık

  adaptive = EEPROM.read(0);    //ldr nin adaptif olma durumu
  rVal = EEPROM.read(1);        //kırmızı renk değeri
  gVal = EEPROM.read(2);        //yeşil renk değeri
  bVal = EEPROM.read(3);        //sarı renk değeri

  strip.setBrightness(EEPROM.read(4));    //paralıklık değeri

  effectIndex = EEPROM.read(5);    //efekt indisi

  //hafızzadaki efekt indisine göre başlangıç efektini seç
  switch (effectIndex)
  {
    case 0:
      colorWipe(strip.Color(rVal, gVal, bVal), 50);
      break;
    case 1:
      theaterChase(strip.Color(rVal, gVal, bVal), 50);
      break;
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  double sumVals = 0;
  for (int i = 0 ; i < 10; i++)
  {
    sumVals += (analogRead(A0) / 1024.0);
  }

  int brightVal = (sumVals / 10) * 255;

  if (adaptive)
  {
    int val = changeBrightness(brightVal);
    Serial.println(val);
    strip.setBrightness(val);
    colorWipe(strip.Color(rVal, gVal, bVal), 1);
  }
  
  parseData(&btSerial);
}

void parseData(SoftwareSerial *s)
{
  if (s->available())
  {
    char c = s->read();

#ifdef DEBUG
    Serial.print("Data: ");
    Serial.print(c);
    Serial.print("|");
#endif

    if (c == '#')           //renk seçimi
    {
      rVal = s->parseInt();
      gVal = s->parseInt();
      bVal = s->parseInt();

      switch (effectIndex)
      {
        case 0:
          colorWipe(strip.Color(rVal, gVal, bVal), 50);
          break;
        case 1:
          //SnowSparkle(strip.Color(rVal, gVal, bVal), 20, random(100, 1000));
          theaterChase(strip.Color(rVal, gVal, bVal), 50);
          break;
      }

      EEPROM.write(1, rVal);
      EEPROM.write(2, gVal);
      EEPROM.write(3, bVal);

#ifdef DEBUG
      Serial.print(rVal);
      Serial.print("|");
      Serial.print(gVal);
      Serial.print("|");
      Serial.println(bVal);
#endif
    }
    else if (c == '&')      //parlaklık ayarı
    {
      int brVal = s->parseInt();
      EEPROM.write(4, brVal);

#ifdef DEBUG
      Serial.println(brVal);
#endif

      strip.setBrightness(brVal);
      colorWipe(strip.Color(rVal, gVal, bVal), 1);
    } else if (c == '+')      //otomatik yada manuel parlaklık seçimi
    {
      int var = s->parseInt();

#ifdef DEBUG
      Serial.println(var);
#endif

      if (var == 0)
      {
        adaptive = false;
      } else {
        adaptive = true;
      }
      EEPROM.write(0, adaptive);
    } else if (c == '$')      //efekt seçimi
    {
      int eIndex = s->parseInt();
      effectIndex = eIndex;

      EEPROM.write(5, effectIndex);

#ifdef DEBUG
      Serial.println(effectIndex);
#endif
    }
  }
}

int changeBrightness(uint8_t p_val)
{
  if (p_val < brVal1)
  {
    brVal1--;
  } else if (p_val > brVal1)
  {
    brVal1++;
  }

  delay(5);
  return brVal1;
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void SnowSparkle(uint32_t c, int SparkleDelay, int SpeedDelay) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
  }

  int Pixel = random(NUM_LEDS);
  strip.setPixelColor(Pixel, c);
  strip.show();
  delay(SparkleDelay);
  strip.setPixelColor(Pixel, c);
  strip.show();
  delay(SpeedDelay);
}

void theaterChase(uint32_t c, uint8_t wait) {
  for (int j = 0; j < 10; j++) { //do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, c);  //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}



unsigned long lastTime = 0;
int16_t pwmVal = 0;

int led1 = 18;
int led2 = 19;
int ledOnBoard = 25;
int ledPwm = 20;
int potPin = A2;

void setup() {
  // put your setup code here, to run once:
  pinMode(ledOnBoard, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(ledPwm, OUTPUT);
  pinMode(potPin, INPUT);
  Serial.begin(115200);
  analogWriteFreq(2000);
}

uint8_t cnt = 0;
void loop() {
  // put your main code here, to run repeatedly:
  if (millis() - lastTime > 1000) {
    uint16_t adcVal = analogRead(A2);
    Serial.print("Hello World: ");
    Serial.print(cnt++);
    Serial.print(" ADC: ");
    Serial.println(adcVal);
    digitalWrite(ledOnBoard, !digitalRead(ledOnBoard));
    lastTime = millis();
  }

  if (Serial.available() > 0)
  {
    char c = Serial.read();
    switch (c)
    {
      case 'a':
        digitalWrite(led1, Serial.read() == '0' ? 0 : 1);
        break;
      case 'b':
        digitalWrite(led2, Serial.read() == '0' ? 0 : 1);
        break;
      case 'l':
        char cc = Serial.read();
        if (cc == '+')
        {
          pwmVal += 25;
          pwmVal = pwmVal > 255 ? 255 : pwmVal;
        } else if (cc == '-')
        {
          pwmVal -= 25;
          pwmVal = pwmVal < 0 ? 0 : pwmVal;
        }
        analogWrite(20, pwmVal);
        break;
    }
  }
}

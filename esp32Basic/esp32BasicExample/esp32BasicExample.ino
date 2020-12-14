// Pin configurations
const int potPin1 = 34, potPin2 = 35;
const int ledPin1 = 17, ledPin2 = 5, ledPin3 = 18;

// pwm and adc settings
const int ledChannel1 = 0, ledChannel2 = 1;   // pwm channels
const int freq = 5000;                        // pwm freq
const int resolution = 10;                    // pwm and adc resolution


void setup() {
  // put your setup code here, to run once:
  // set adc pins as a input
  // set led pins as a output
  pinMode(potPin1, INPUT);
  pinMode(potPin2, INPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);

  // set pwm channels freq and resolution
  ledcSetup(ledChannel1, freq, resolution);
  ledcSetup(ledChannel2, freq, resolution);

  // connect led pins to pwm channels
  ledcAttachPin(ledPin1, ledChannel1);
  ledcAttachPin(ledPin2, ledChannel2);

  // set adc resolution same as pwm resolution
  analogReadResolution(resolution);

  // start serial communication
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:

  // read analog vaules from potantiometers
  int adcVal1 = analogRead(potPin1);
  int adcVal2 = analogRead(potPin2);

  // set analog values as a duty cycle to pwm channels
  ledcWrite(ledChannel1, adcVal1);
  ledcWrite(ledChannel2, adcVal2);

  // show analog values in serial monitor
  Serial.print(adcVal1);
  Serial.print("\t");
  Serial.println(adcVal2);

  // toggle third led continuously
  digitalWrite(ledPin3, !digitalRead(ledPin3));

  delay(100);
}
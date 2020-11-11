// ADC Interrupt example


bool working = false;   // adc working status flag
int val = 0;
int cnt = 0;
int adcPin = 0;
float InternalReferenceVoltage = 1.1;

float tempVal, voltVal, adc0Val, adc1Val;

// adc interrupt vect
ISR(ADC_vect)
{
  val = ADC;
  working = false;      // set working status as a false
}

void setup() {
  // put your setup code here, to run once:

  ADCSRA =  bit (ADEN);   // turn ADC on
  ADCSRA |= bit (ADPS0) |  bit (ADPS1) | bit (ADPS2);  // Prescaler of 128

  ADMUX = bit (REFS0) | bit (REFS1)  | 0x08;    // temperature sensor
  ADCSRA |= bit (ADSC) | bit (ADIE);
  working = true;

  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (!working) // check is adc completed
  {
    switch (cnt)		// read analog data from another source
    {
      case 0:
        // get temp value from last reading
        tempVal = val - 330;

        // set adc for reading channel 0
        adcPin = 0;
        ADMUX  =  bit (REFS0) | (adcPin & 0x07);    // set reference voltage 5V and input channel A0
        break;
      case 1:
        // get pin 0 value from last reading
        adc0Val = val;

        // set adc for reading channel 1
        adcPin = 1;
        ADMUX  =  bit (REFS0) | (adcPin & 0x07);    // set reference voltage 5V and input channel A1
        break;
      case 2:
        // get pin 1 value from last reading
        adc1Val = val;

        // set adc for voltage sensor
        ADMUX = bit (REFS0) | bit (MUX3) | bit (MUX2) | bit (MUX1);
        break;
      case 3:
        // get voltage value from last reading
        voltVal = InternalReferenceVoltage / float (val + 0.5) * 1024.0;

        // set adc for temp sensor
        ADMUX = bit (REFS0) | bit (REFS1)  | 0x08;    // sicaklik sensoru
        break;
    }
  
	delay(20);
	ADCSRA |= bit (ADSC) | bit (ADIE);			// start adc in interrupt mode

    cnt++;
    if (cnt == 4) cnt = 0;
    working = true;         // set working status as a true
  }

  Serial.print("Ch0: ");
  Serial.print(adc0Val);
  Serial.print("\t");
  
  Serial.print("Ch1: ");
  Serial.print(adc1Val);
  Serial.print("\t");
  
  Serial.print("Temp: ");
  Serial.print(tempVal);
  Serial.print("\t");
  
  Serial.print("Volt: ");
  Serial.print(voltVal);
  Serial.println();
  

  // make another process
  digitalWrite(13, !digitalRead(13));
  delay(30);

}
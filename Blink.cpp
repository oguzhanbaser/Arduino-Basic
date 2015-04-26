#define LED 13			//definition LED pin

void setup()
{
	pinMode(LED, OUTPUT);	//pin direction sets OUTPUT
}

void loop()
{
	digitalWrite(LED, HIGH);		//Logic - 1
	delay(1000);					//waiting 1 second
	digitalWrite(LED, LOW);			//Logic - 0
	delay(1000);					//waiting 1 second
}
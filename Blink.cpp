#define LED 13      //definition of led pin

void setup()      //software configuration
{
	pinMode(LED, OUTPUT);   //set led pin direction OUTPUT
}

void loop()       //infinite loof
{
	digitalWrite(LED, HIGH);    //set Logic-1
	delay(1000);                //wait 1 second
	digitalWrite(LED, LOW);     //set Lohic-2
	delay(1000);                //wait 1 second
}

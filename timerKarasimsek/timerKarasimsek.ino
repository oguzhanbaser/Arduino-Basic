int sayac = 0, sonSayac = 0;

int ledPin[9] = {2, 3, 4, 5, 6, 7, 8, 9, 10};

bool arttir = true;

void setup() {

  // put your setup code here, to run once:
  cli();
  TCCR1A = 0;// TCCR1A register 0'lanıyor
  TCCR1B = 0;// TCCR1B register 0'lanıyor
  TCNT1  = 0;//sayac değeri  0'la
  // OCRxA karşılaştırma registeri 1Hz değer için ayarlanıyor
  //16 MHz osilatör,10Hz timer1 ın çalışma frekansı,1024 prescalar
  OCR1A = 1562;// = (16*10^6) / (1*1024) - 1 (değer 65536 dan küçük)
  //   CTC mod açılıyor.
  TCCR1B |= (1 << WGM12);
  //   CS10 ve CS12 bitleri 1024 prescaler için ayarlanıyor
  TCCR1B |= (1 << CS12) | (1 << CS10);
  // timer karşılaştırma interruptı aktifleştiriliyor
  TIMSK1 |= (1 << OCIE1A);
  sei();

  for (int i = 0; i < 9; i++)
  {
    pinMode(ledPin[i], OUTPUT);
  }
}

ISR(TIMER1_COMPA_vect) {  //timer1 interrupt ı 10Hz de tetikleniyor.
  if (arttir)
  {
    sayac++;
  } else {
    sayac--;
  }

  if (sayac == 9) arttir = false;
  if (sayac == 0) arttir = true;
}

void loop() {
  // put your main code here, to run repeatedly:
  if (sayac != sonSayac)
  {
    //tüm ledleri söndür
    for (int i = 0; i < 9; i++)
    {
      digitalWrite(ledPin[i], LOW);
    }

    //sırası gelen ledi yak
    digitalWrite(ledPin[sayac], HIGH);

    //son sayac değerini bir sonraki değişime kadar hafızada tut
    sonSayac = sayac;
  }
}

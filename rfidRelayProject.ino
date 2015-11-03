long lastTime = 0, lastTime2 = 0;

bool bayrakGiris = false;

#define ROLE A0

#define LED1 A4
#define LED2 A5
#define LED3 4
#define LED4 5

bool durum1 = true, durum2 = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A0, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  digitalWrite(ROLE, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available())
  {
    String cardId = dataOku();

    
    if (girisYap(cardId) && !bayrakGiris)
    {
      bayrakGiris = true;
      digitalWrite(ROLE, LOW);
      ledDegis(true);
      lastTime2 = millis();
    } else {
      bayrakGiris = true;
      lastTime2 = millis();
    }

  }

  ledAnimasyon();
}

//yeni kişilerin Card Id leri buraya eklenecek
bool girisYap(String cardId)
{
  if(cardId == "0002865760") return true;
  else return false;
}

String dataOku()
{
  char _id[11];
  int j = 0;
  String s = Serial.readStringUntil('\n');

  for (int i = 0; i < s.length(); i++)
  {

    ledDondur();
    if (isdigit(s[i]))
    {
      _id[j++] = s[i];
    }
  }

  _id[10] = '\0';
  String cardId(_id);

  return cardId;
}

void ledAnimasyon()
{
  if (bayrakGiris && millis() - lastTime2 > 2000)
  {
    digitalWrite(ROLE, HIGH);
    bayrakGiris = false;
    ledDegis(false);
  } else if (!bayrakGiris) {
    ledDondur();
  }
}

void ledDegis(bool _durum)
{
  digitalWrite(LED1, _durum);
  digitalWrite(LED2, _durum);
  digitalWrite(LED3, _durum);
  digitalWrite(LED4, _durum);
}

void ledDondur()
{
  if (millis() - lastTime > 500)
  {
    durum1 = !durum1;
    durum2 = !durum2;
    lastTime = millis();
  }
  digitalWrite(LED1, durum1);
  digitalWrite(LED2, durum1);
  digitalWrite(LED3, durum2);
  digitalWrite(LED4, durum2);
}

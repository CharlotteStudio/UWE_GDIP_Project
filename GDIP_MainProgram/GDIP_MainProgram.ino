
const int distance = 10;
const int TrigPin = 4;
const int EchoPin = 5;
const int relayPin = 6;

float cm;
bool relayOn = false;

void setup()
{
  Serial.begin(115200);
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);
  pinMode(relayPin, OUTPUT);

  digitalWrite(relayPin, HIGH);
  relayOn = false;
}

void loop()
{
  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);

  // 將回波時間換算成 cm
  cm = pulseIn(EchoPin, HIGH) * 0.0170145;
  //This code also working
  //cm = pulseIn(EchoPin, HIGH) / 58.0;

  if(cm < 1000){
    Serial.print("The Distance :");
    Serial.print(cm);
    Serial.println("cm");
  } else {
    Serial.println("Error Detect...");
  }

  if (cm < distance){
    if(relayOn){
      digitalWrite(relayPin, HIGH);
      relayOn = false;
      Serial.println("off");
    }
  } else{
    if(!relayOn){
      digitalWrite(relayPin, LOW);
      relayOn = true;
      Serial.println("on");
    }
  }
  
  delay(100);
}

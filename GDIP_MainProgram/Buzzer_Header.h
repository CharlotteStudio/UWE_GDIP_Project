int buzzerPin    = 0;
int buzzerFreq   = 0;	// Freq 愈大會愈大聲
int activeTime   = 0;
int intervalTime = 0;
int loopTime     = 0;
int currentLoop  = 0;

unsigned long startTime = 0;
unsigned long endTime   = 0;

bool isOn = false;

void InitBuzzer(int pin){
  isOn = false;
  buzzerPin = pin;
  pinMode(pin, OUTPUT);
}

void StopBuzzerLoop(){
  isOn        = false;
  startTime   = 0;
  endTime     = 0;
  loopTime    = 0;
  currentLoop = 0;
  analogWrite(buzzerPin, 0);
}

void UpdateBuzzer(){

  if (!isOn) return;

  if (loopTime != 0 && currentLoop >= loopTime){
    StopBuzzerLoop();
    return;
  }

  if (millis() > endTime)
  {    
    analogWrite(buzzerPin, 0);
    endTime   = millis() + activeTime + intervalTime;

    if (loopTime != 0) currentLoop++;
  }

  if (millis() > startTime)
  {
    analogWrite(buzzerPin, buzzerFreq);
    startTime = millis() + activeTime + intervalTime;

    if (loopTime != 0) currentLoop++;
  }
}

// freq is 0 ~ 255
void StartBuzzerLoop(int freq, int time, int interval){
  isOn = true;
  buzzerFreq   = freq;
  activeTime   = time;
  intervalTime = interval;
  analogWrite(buzzerPin, buzzerFreq);

  startTime = millis() + activeTime + intervalTime;
  endTime   = millis() + activeTime;
}

void StartBuzzerLoop(int freq, int time, int interval, int loop){
  StartBuzzerLoop(freq, time, interval);
  loopTime = loop * 2;
  currentLoop = 0;
}
const int digitalPin = 2;
const int interval = 1000;

void initLight(){
  pinMode(digitalPin, INPUT);
}

float watcherLight(){
  unsigned long startTime = millis();
  int highCount = 0;
  int totalCount = 0;

  while (millis() - startTime < interval) {
    int val = digitalRead(D0);
    if (val == HIGH) highCount++;
    totalCount++;
    delay(1); // короткая пауза, чтобы не перегружать цикл
  }

  int val = digitalRead(D0);
  float lightPercent = (highCount / (float)totalCount) * 100.0;
  float res = 100 - lightPercent;

 /* Serial.print("Light (digital approx): ");
  Serial.print(lightPercent);
  Serial.println("%");*/


 return res; 
}

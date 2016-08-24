void setup() {
  // put your setup code here, to run once:
  pinMode(31, OUTPUT);           // set pin to input
  Serial.begin(115200);
  digitalWrite(31, LOW);
}

void loop() {
  char ch = 0;
  // put your main code here, to run repeatedly:
  Serial.println("Type the number and press enter");

  Serial.flush(); //flush all previous received and transmitted data 
  while(!Serial.available()) ;
  
  ch = Serial.read();
  
  
  switch(ch)
  {
    case '1':
    Serial.println("Pin ON");
    digitalWrite(31, HIGH);
    delay(150);
    digitalWrite(31, LOW);
    Serial.println("Pin OFF");
    //delay(10000);
    Serial.println("-------------");
  }

}

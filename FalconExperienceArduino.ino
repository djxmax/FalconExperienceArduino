/*
  DigitalReadSerial
  Reads a digital input on pin 2, prints the result to the serial monitor

  This example code is in the public domain.
*/

String serialResponse = "";
void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    serialResponse = Serial.readStringUntil('\r\n');

    // Convert from String Object to String.
    char buf[500];
    serialResponse.toCharArray(buf, sizeof(buf));
    char *p = buf;
    char *str;
    while ((str = strtok_r(p, "#", &p)) != NULL) {
      processData(str);
    }
      Serial.println(str);
  }
  delay(1);
}

void processData(char *str) {
  
  String myString = String(str);
  myString.replace(" ","");
  int ind1 = myString.indexOf('-');  //finds location of first ,
  String item = myString.substring(0, ind1);
  String action = "";
  String freq = "";
  int ind2 = myString.indexOf('$');
  if (ind2 == -1) {
    action = myString.substring(ind1+1);
  } else {
    action = myString.substring(ind1+1, ind2);
    freq = myString.substring(ind2+1);
  }
  Serial.println(item);
  Serial.println(action);
  Serial.println(freq);
}

//#L1-OF #L2-ON$500




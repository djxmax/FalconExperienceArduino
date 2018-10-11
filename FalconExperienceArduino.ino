/*
  DigitalReadSerial
  Reads a digital input on pin 2, prints the result to the serial monitor

  This example code is in the public domain.
*/

String inputName[] = {"B1", "B2", "B3", "B4", "B5", "B6"};
int inputPin[] = {1, 2, 3, 4, 5, 6};
int inputState[] = {0, 0, 0, 0, 0, 0};
int outputSize = 6;
String outputName[] = {"L1", "L2", "L3", "L4", "L5", "L6"};
int outputPin[] = {7, 8, 9, 10, 11, 13};
int outputState[] = {0, 0, 0, 0, 0, 0};
String blinkName = "";
unsigned long blinkTime = 0;
int blinkFreq = 0;



String serialResponse = "";
void setup() {
  Serial.begin(9600);
  Serial.println("Init...");

  /*for (int thisPin = 0; thisPin < sizeof(inputPin); thisPin++) {
    pinMode(inputPin[thisPin], INPUT);
    }*/

  for (int thisPin = 0; thisPin < outputSize; thisPin++) {
    pinMode(outputPin[thisPin], OUTPUT);
  }
  pinMode(13, OUTPUT);

  Serial.println("Ready !!");
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
  }

  processBlink();

  delay(1);
}

void processData(char *str) {

  String myString = String(str);
  myString.trim();
  int ind1 = myString.indexOf('-');  //finds location of first ,
  String item = myString.substring(0, ind1);
  String action = "";
  String freq = "";
  int ind2 = myString.indexOf('$');
  if (ind2 == -1) {
    action = myString.substring(ind1 + 1);
  } else {
    action = myString.substring(ind1 + 1, ind2);
    freq = myString.substring(ind2 + 1);
  }
  
  Serial.println("Receive item : "+item+", action : "+action+", freq : "+freq);

  if (action == "ON") {
    for (int theItem = 0; theItem < sizeof(outputName); theItem++) {
      if (outputName[theItem] == item) {
        digitalWrite(outputPin[theItem], HIGH);
        outputState[theItem] = 1;
        break;
      }
    }

    if (freq != "") {
      blinkFreq = freq.toInt();
      blinkName = item;
      blinkTime = millis();
    }
  } else if (action == "OF") {
    for (int theItem = 0; theItem < sizeof(outputName); theItem++) {
      if (outputName[theItem] == item) {
        digitalWrite(outputPin[theItem], LOW);
        outputState[theItem] = 0;
        break;
      }
    }

    if (item == blinkName) {
      blinkName = "";
      blinkTime = 0;
      blinkFreq = 0;
    }
  }
}

//#L1-OF #L2-ON$500
//#L6-ON$500

void processBlink() {
  if (blinkName != "") {
    unsigned long current = millis();
    if (current > blinkTime + blinkFreq) {
      for (int theItem = 0; theItem < sizeof(outputName); theItem++) {
        if (outputName[theItem] == blinkName) {
          if (outputState[theItem] == 0) {
            digitalWrite(outputPin[theItem], HIGH);
            outputState[theItem] = 1;
          } else {
            digitalWrite(outputPin[theItem], LOW);
            outputState[theItem] = 0;
          }
          break;
        }
      }
      blinkTime = current;
    }


  }
}




#include <require_cpp11.h>
#include <MFRC522.h>
#include <deprecated.h>
#include <MFRC522Extended.h>

// ringID 3404590208;

#include <SPI.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;
unsigned long uidDec, uidDecTemp; //for key IDs in 10syst

//Motion
#define PIN_ONE_PIR 4
#define PIN_TWO_PIR 5
//IRs
#define PIN_IR 2
#define PIN_LEFT_IR 7
#define PIN_RIGHT_IR 6
//LEDs
#define PIN_SINGLE_LED A0
#define PIN_BOTH_LED A1
#define PIN_MOTION_LED A2

int pirValOne;
int pirValTwo;

int sensorvalue;
int leftvalue;
int rightvalue;

void setup() {
  Serial.begin(9600);

  SPI.begin();
  mfrc522.PCD_Init();

  pinMode(PIN_ONE_PIR, INPUT);
  pinMode(PIN_TWO_PIR, INPUT);
  pinMode(PIN_IR, INPUT);
  pinMode(PIN_LEFT_IR, INPUT);
  pinMode(PIN_RIGHT_IR, INPUT);
  pinMode(PIN_SINGLE_LED, OUTPUT);
  pinMode(PIN_BOTH_LED, OUTPUT);
  pinMode(PIN_MOTION_LED, OUTPUT);

}

void loop() {

  //sensor values
  pirValOne = digitalRead(PIN_ONE_PIR);
  pirValTwo = digitalRead(PIN_TWO_PIR);
  sensorvalue = digitalRead(PIN_IR);
  leftvalue = digitalRead(PIN_LEFT_IR);
  rightvalue = digitalRead(PIN_RIGHT_IR);
  int players;

  //number of players
  if (Serial.available() > 0) {
    players = Serial.read();
    if (players == '1') {
      oneLed();
    } else if (players == '2') {
      twoLeds();
    } else if (players == '3') {
      threeLeds();
    }
  }

  //TURNS
  if (players == '4') {
    digitalWrite(PIN_SINGLE_LED, HIGH);
    digitalWrite(PIN_BOTH_LED, LOW);
    digitalWrite(PIN_MOTION_LED, LOW);
    if (pirValOne) {
      oneLed();
      if (!leftvalue) {
        Serial.println("<10>");
        irs();
      }
    }
  } else if (players == '5') {
    digitalWrite(PIN_SINGLE_LED, LOW);
    digitalWrite(PIN_BOTH_LED, HIGH);
    digitalWrite(PIN_MOTION_LED, LOW);
    if (pirValOne) {
      oneLed();

      //If on the second the movment is detected after first
      if (pirValTwo) {
        twoLeds();

        //if there is an object at the bottom
        if (!sensorvalue) {
          Serial.println("<10>");
          irs();
        }
      }
    }
  } else if (players == '6') {
    digitalWrite(PIN_SINGLE_LED, LOW);
    digitalWrite(PIN_BOTH_LED, LOW);
    digitalWrite(PIN_MOTION_LED, HIGH);
    if (pirValTwo) {
      oneLed();

      //if there is an object at the bottom
      if (!rightvalue) {
        Serial.println("<10>");
        irs();
      }
    }
  }
}

void oneLed() {
  digitalWrite(PIN_SINGLE_LED, HIGH);
  digitalWrite(PIN_BOTH_LED, LOW);
  digitalWrite(PIN_MOTION_LED, LOW);
  delay(500);
  ledsOff();
}

void twoLeds() {
  digitalWrite(PIN_SINGLE_LED, HIGH);
  digitalWrite(PIN_BOTH_LED, HIGH);
  digitalWrite(PIN_MOTION_LED, LOW);
  delay(500);
  ledsOff();
}

void threeLeds() {
  digitalWrite(PIN_SINGLE_LED, HIGH);
  digitalWrite(PIN_BOTH_LED, HIGH);
  digitalWrite(PIN_MOTION_LED, HIGH);
  delay(500);
  ledsOff();
}

void ledsOff() {
  digitalWrite(PIN_SINGLE_LED, LOW);
  digitalWrite(PIN_BOTH_LED, LOW);
  digitalWrite(PIN_MOTION_LED, LOW);
  delay(500);
}

void irs() {
  threeLeds();
  rfid();
}

void rfid() {
  if ( ! mfrc522.PICC_IsNewCardPresent())
    return;


  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
    return;

  // tag ID
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uidDecTemp = mfrc522.uid.uidByte[i];
    uidDec = uidDec * 256 + uidDecTemp;
  }

  Serial.println("Tag ID: ");
  Serial.println(uidDec);

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  if (uidDec == 3404590208) {
    congrats(3);
    Serial.println("<15>");
  } else  {
    wrongRing();
  }
}

void congrats (int times) {
  for ( int i = 0; i < times; i++) {
    digitalWrite(PIN_SINGLE_LED, LOW);
    digitalWrite(PIN_BOTH_LED, LOW);
    digitalWrite(PIN_MOTION_LED, LOW);
    delay(500);
    digitalWrite(PIN_SINGLE_LED, HIGH);
    digitalWrite(PIN_BOTH_LED, HIGH);
    digitalWrite(PIN_MOTION_LED, HIGH);
    delay (500);
  }

}

void wrongRing () {
  for ( int i = 0; i < 1; i++) {
    digitalWrite(PIN_SINGLE_LED, LOW);
    digitalWrite(PIN_BOTH_LED, LOW);
    digitalWrite(PIN_MOTION_LED, HIGH);
    delay(500);
    digitalWrite(PIN_SINGLE_LED, LOW);
    digitalWrite(PIN_BOTH_LED, LOW);
    digitalWrite(PIN_MOTION_LED, LOW);
    delay (500);
  }
}

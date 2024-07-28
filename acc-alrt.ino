//I didn't write this code from scratch. So it contains some unnecessary functions too.
#include <SoftwareSerial.h>
#include <TinyGPS++.h>


TinyGPSPlus gps;
SoftwareSerial mygps(4, 3); // 4 tx 3 rx
SoftwareSerial SIM900A(5, 6); // tx rx

#define BUZZER_PIN 11
#define button 9
float latitude = 0.0;
float longitude = 0.0;



byte updateflag;
int xaxis, yaxis, zaxis;
int deltx, delty, deltz;
int vibration = 0, magnitude = 0, sensitivity = 100, devibrate = 75;
double angle;
unsigned long time1;

void setup() {
  SIM900A.begin(9600);
  Serial.begin(9600);
  mygps.begin(9600);
  delay(100);
  Serial.println("SIM900A Ready");
  delay(100);
  Serial.println("You can use now");
  time1 = micros();
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(button, INPUT);
  digitalWrite(button, HIGH);

}

void loop() {
  if (checkCrash()) {
    Serial.print("Crash detected ");
    unsigned long startTime = millis();
    
    // Run the loop for 10 seconds
    while (millis() - startTime < 10000) {

      beep();
      // Check if switch is pressed
      if (digitalRead(button)==LOW){
        // If D9 is high, stop the buzzer and exit the loop
        digitalWrite(BUZZER_PIN, LOW);
        Serial.println("Button is pressed");
        return;
      }
    }
    SendMessage();
  }

  while (mygps.available() > 0) {
    gps.encode(mygps.read());
    if (gps.location.isUpdated() && gps.location.lat() != 0) {
      latitude = gps.location.lat();
      longitude = gps.location.lng();
    }
  }

  if (Serial.available() > 0)
    switch (Serial.read()) {
      case 's':
        SendMessage();
        break;
      case 'r':
        RecieveMessage();
        break;
    }

  if (SIM900A.available() > 0)
    Serial.write(SIM900A.read());
}

boolean checkCrash() {
  time1 = micros();
  int oldx = xaxis;
  int oldy = yaxis;
  int oldz = zaxis;

  vibration--;
  if (vibration < 0) vibration = 0;

  xaxis = analogRead(A0);
  yaxis = analogRead(A1);
  zaxis = analogRead(A2);

  if (vibration > 0) return false;

  deltx = xaxis - oldx;
  delty = yaxis - oldy;
  deltz = zaxis - oldz;

  magnitude = sqrt(sq(deltx) + sq(delty) + sq(deltz));

  if (magnitude >= sensitivity) {
    updateflag = 1;

    double X = acos((double) deltx / magnitude);
    double Y = acos((double) delty / magnitude);

    angle = (atan2(Y, X) * 180) / PI;
    angle += 180;

    vibration = devibrate;
      Serial.println(magnitude);

    return true;
  } else {
    magnitude = 0;
    return false;
  }
}

void SendMessage() {
  Serial.println("Sending Message");
  SIM900A.println("AT+CMGF=1");
  delay(1000);
  SIM900A.println("AT+CMGS=\"NUMBER_HERE\"\r"); // Emergeny contact number here (with country code) eg: +97798********
  delay(1000);
  String googleMapsUrl = "Crash detected Location: \n https://www.google.com/maps/?q=";
  googleMapsUrl += String(latitude, 6) + "+" + String(longitude, 6);
  SIM900A.println(googleMapsUrl);
  Serial.println(googleMapsUrl);
  delay(100);
  SIM900A.println((char)26);
  delay(1000);
  Serial.println("Location has been sent");
}

void beep() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(300);
  digitalWrite(BUZZER_PIN, LOW); 
  delay(100);
}

void RecieveMessage() {
  Serial.println("SIM900A Membaca SMS");
  delay(1000);
  SIM900A.println("AT+CNMI=2,2,0,0,0");
  delay(1000);
  Serial.write("Unread Message done");
}

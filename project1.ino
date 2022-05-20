/*
* Project 1
* CSC 460: Real Time Systems.
* 
*/

//initial values and import library
#include <LiquidCrystal.h>
#define TRIGGER_PIN 51 // trig pin on the ultrasonic sensor attach to pin 51.
#define ECHO_PIN_INT0 21 // echo pin on the ultrasonic sensor attach to pin 20 and 21.
#define ECHO_PIN_INT1 20 // and will be used latter for interrupts.
#define MAX_DISTANCE 400 // Maximum distance we want to ping for (in centimeters). Maximum
#define RED_LED 52 // Red LED is connected to pin 52
#define GREEN_LED 53 // Green LED is connected to pin 53
#define BUZZER 50 // Buzzer is connected to pin 50 

// sensor distance is rated at 400-500cm.
int distance_cm, distance_inches;
volatile long duration;
volatile unsigned long highTime;

LiquidCrystal lcd(4, 6, 10, 11, 12, 13);//lcd(RS,E,D4,D5,D6,D7)
unsigned int seconds = 0;


// setup the chip
void setup()
{
  Serial.begin(9600); // Initialize serial port
  lcd.begin(16, 2); //// set the position of the characters on the LCD as Line 2, Column 16

  pinMode(RED_LED, OUTPUT);// LED outputs
  pinMode(GREEN_LED, OUTPUT);

  // Initialize the digital pin as an output.
  pinMode(BUZZER, OUTPUT);

  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN_INT0, INPUT); // This pin will be used to watch for rising echo signal
  pinMode(ECHO_PIN_INT1, INPUT); // This pin will be used to watch for faling echo signal
  
  // Attach interrupt subroutines
  attachInterrupt(digitalPinToInterrupt(ECHO_PIN_INT0), echo_high, RISING);
  attachInterrupt(digitalPinToInterrupt(ECHO_PIN_INT1), echo_low, FALLING);
}
void loop() {// every iteration will get the distance and check the states, and wait 0.5s
  lcd.clear();
  lcd.setCursor(0, 0);
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  // Convert the time into a distance
  distance_cm = (duration/2) / 29.1; // Divide by 29.1 or multiply by 0.0343
  distance_inches = (duration/2) / 74; // Divide by 74 or multiply by 0.0135
  // Display the distance value on the terminal connected to serial port
  // Go to Tools -> Serial Monitor and make sure it is set to 9600 baud

  if (distance_cm >= 30){// check state greater than 30cm
    digitalWrite(GREEN_LED, HIGH);// Green LED ON
    digitalWrite(RED_LED, LOW); // Red LED OFF
    digitalWrite(BUZZER, LOW); // Buzzer OFF

    lcd.print("distance:");
    lcd.setCursor(0, 1); // Move the cursor to Line 1, Column 0
    lcd.print(" "); // and clear any leftover text.

    
    }
  else if (distance_cm < 30 && distance_cm > 10){// between 10 and 30
    digitalWrite(RED_LED, HIGH); // Red LED ON
    digitalWrite(GREEN_LED, LOW);// Green LED OFF
    digitalWrite(BUZZER, LOW); // Buzzer OFF

    lcd.print("distance:");
    lcd.setCursor(0, 1); // Move the cursor to Line 1, Column 0
    lcd.print(" "); // and clear any leftover text.
  } else{// less than 10
    digitalWrite(BUZZER, HIGH); // Buzzer ON
    digitalWrite(RED_LED, HIGH); // Red LED ON
    
    lcd.print("Danger!!!!");
    lcd.setCursor(0, 1); // Move the cursor to Line 1, Column 0
    lcd.print(" "); // and clear any leftover text.
  }
  
  lcd.setCursor(9, 1); // Move the cursor to Line 1, Column 9
  lcd.print(distance_cm); // and print the time in seconds
  lcd.setCursor(12, 1); // Move the cursor to Line 1, Column 12
  lcd.print("cm"); // and print sec

  Serial.print("Distance = ");// output for debug
  Serial.print(distance_cm);
  Serial.println(" cm"); // println gives a new line afterword

  
  delay(500); 
}
// sensor distance function
void echo_high() {
  highTime = micros();
}
void echo_low() {
  duration = micros()-highTime;
  highTime = 0;
}

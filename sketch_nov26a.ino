#include <LiquidCrystal.h>

// Initialize the LCD (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// Define MQ-2 sensor pin
const int gasSensorPin = 2;

void setup() {
  pinMode(gasSensorPin, INPUT); // Set MQ-2 pin as input
  lcd.begin(16, 2); // Initialize a 16x2 LCD
  lcd.print("Gas Detector"); // Initial message
  delay(2000); // Wait for 2 seconds
  lcd.clear();
}

void loop() {
  int gasDetected = digitalRead(gasSensorPin); // Read the MQ-2 sensor output

  lcd.setCursor(0, 0); // Set cursor to the first line
  if (gasDetected == LOW) { // Gas detected
    lcd.print("Gas Detected!");
  } else { // No gas detected
    lcd.print("No Gas Found ");
  }
  
  delay(500); // Wait 500ms before updating again
}

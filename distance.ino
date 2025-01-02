// Segment pins
int a = 26; // top
int b = 2; // top right
int c = 3; // bottom right
int d = 4; // bottom
int e = 5; // bottom left
int f = 6; // top left
int g = 7; // middle
int p = 8; // dot (decimal point)

// Digit pins
int d4 = 9;  // Digit 4 (rightmost)
int d3 = 10; // Digit 3
int d2 = 11; // Digit 2
int d1 = 12; // Digit 1 (leftmost)

int buzzerPin = 52;

const int analogPin = A0;
int sensorValue = 0;

int distance = 0;

// Segment mapping for numbers 0-9 (common cathode)
const int numbers[10][7] = {
  {0, 0, 0, 0, 0, 0, 1}, // 0
  {1, 0, 0, 1, 1, 1, 1}, // 1
  {0, 0, 1, 0, 0, 1, 0}, // 2
  {0, 0, 0, 0, 1, 1, 0}, // 3
  {1, 0, 0, 1, 1, 0, 0}, // 4
  {0, 1, 0, 0, 1, 0, 0}, // 5
  {0, 1, 0, 0, 0, 0, 0}, // 6
  {0, 0, 0, 1, 1, 1, 1}, // 7
  {0, 0, 0, 0, 0, 0, 0}, // 8
  {0, 0, 0, 0, 1, 0, 0}  // 9
};

// Initialize pins
void setup() {
  Serial.begin(9600);
  // Segment pins
  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(f, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(p, OUTPUT);

  // Digit select pins
  pinMode(d1, OUTPUT);
  pinMode(d2, OUTPUT);
  pinMode(d3, OUTPUT);
  pinMode(d4, OUTPUT);

  //buzzer
  pinMode(buzzerPin, OUTPUT);

  // Sensor
  pinMode(22, OUTPUT);
  pinMode(24, INPUT);

  // Turn off all digits and segments initially
  clearDigits();
  clearSegments();
}

// Turn off all segments
void clearSegments() {
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
  digitalWrite(p, HIGH); 
}

// Turn off all digits
void clearDigits() {
  digitalWrite(d1, LOW);
  digitalWrite(d2, LOW);
  digitalWrite(d3, LOW);
  digitalWrite(d4, LOW);
}

// Display a number on a specific digit
void displayNumber(int digitPin, int number, bool activateDecimal = false) {
  clearDigits();    // Turn off all digits
  clearSegments();  // Turn off all segments

  // Activate the specified digit
  digitalWrite(digitPin, HIGH);

  // Light up the segments for the given number
  digitalWrite(a, numbers[number][0]);
  digitalWrite(b, numbers[number][1]);
  digitalWrite(c, numbers[number][2]);
  digitalWrite(d, numbers[number][3]);
  digitalWrite(e, numbers[number][4]);
  digitalWrite(f, numbers[number][5]);
  digitalWrite(g, numbers[number][6]);

  // Turn on the decimal point if specified
  if (activateDecimal) {
    digitalWrite(p, LOW); // LOW to turn on for common cathode
  } else {
    digitalWrite(p, HIGH); // HIGH to turn off for common cathode
  }
}

// Update display
void updateDisplay() {
  distance = checkdistance();
  //Serial.println(distance);

  int digit1 = (distance / 1000) % 10; // Extract the 1000's place
  int digit2 = (distance / 100) % 10;  // Extract the 100's place
  int digit3 = (distance / 10) % 10;   // Extract the 10's place
  int digit4 = distance % 10;          // Extract the 1's place

  if (digit1 > 0){
    displayNumber(d1, digit1);
    delay(5);
  }
  
  if (digit2 > 0){
    displayNumber(d2, digit2);
    delay(5);
  }
  
  if (digit3 > 0) {
    displayNumber(d3, digit3);
  delay(5);
  }

  displayNumber(d4, digit4);
  delay(5);

  buzzerSound(distance);
}

void buzzerSound(int x){
  if (x < 10){
    tone(buzzerPin, 659);
  } else if (x < 20){
    tone(buzzerPin, 587);
  } else if (x < 35) {
    tone(buzzerPin, 523);
  } else if (x < 50) {
    tone(buzzerPin, 494);
  } else if (x < 60) {
    tone(buzzerPin, 440);
  } else {
    noTone(buzzerPin);
  }
}

float checkdistance() {
  digitalWrite(22, LOW);
  delayMicroseconds(2);
  digitalWrite(22, HIGH);
  delayMicroseconds(10);
  digitalWrite(22, LOW);
  float distance = pulseIn(24, HIGH) / 58.00;
  delay(10);
  //Serial.println(distance);
  return distance;
}

void loop() {
  updateDisplay();
}

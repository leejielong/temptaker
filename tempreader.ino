#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SparkFunMLX90614.h> // SparkFunMLX90614 Arduino library

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define TEMP_MEASUREMENT_RANGE 15
#define DETECTION_RANGE 40
#define COMP 2
#define MEASUREMENTS 10

const int trigPin = 4; 
const int echoPin = 2;
const int buzzerPin = 12;
int count = 0;
float duration, distance; 
float window[MEASUREMENTS];
float sum=0;
float avg;
bool updated_flag = false;
bool updated_flag2 = false;

IRTherm therm; // Create an IRTherm object to interact with throughout


// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void print_oled(String text, int sz, int x, int y){
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
    }
    //display.setRotation(2);
    display.clearDisplay();
    display.setTextSize(sz);
    display.setTextColor(WHITE);
    display.setCursor(x, y);
    // Display static text
    display.println(text);
    display.display(); 
}

float read_distance(){
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW); 
  duration = pulseIn(echoPin, HIGH); 
  distance = (duration*.0343)/2; 
  return distance;
}

void setup() {
  Serial.begin(9600); // Initialize Serial to log output

  //mlx90614 settings
  therm.begin(); // Initialize thermal IR sensor
  therm.setUnit(TEMP_C); // Set the library's units to Celsius
  // Alternatively, TEMP_F can be replaced with TEMP_C for Celsius or
  // TEMP_K for Kelvin.

  //hrsc04 settings
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);

  //buzzer settings
  pinMode(buzzerPin, OUTPUT); 

}

float calibrate(float temp){
  temp += COMP;
  Serial.println("test: "+String(temp,1));
  return temp;
//  if(temp<=35.0){
//    return 35.0;
//  }else{
//    return temp;
//  }
}

void loop() {
    
    distance = read_distance();
    Serial.println("distance: "+String(distance));
    if (distance < DETECTION_RANGE){ //user is found
      if (distance > TEMP_MEASUREMENT_RANGE){
        if(updated_flag == false){
          print_oled("Too Far", 3, 0, 24);
          digitalWrite(buzzerPin, HIGH);  //turn on buzzer if too far
          updated_flag = true;
          updated_flag2 = false;
        }
      }else{ // On success, read() will return 1, on fail 0.
        digitalWrite(buzzerPin, LOW);  //turn off buzzer if within measurement range
        Serial.println("Measuring Temp");
        print_oled(" Measuring", 2, 0, 24);
        for (int i=0; i<MEASUREMENTS; i++){
          if (therm.read()){ //if temp read is successful
            float temp = calibrate(therm.object());
            sum += temp;
            Serial.println("t"+String(i+1)+": "+String(temp,1));
          }
          delay(100);
        }

        avg = sum/MEASUREMENTS;
        Serial.println("avg: "+String(avg,1));
        String temp;
        if(avg<35.0){
          temp = String("Too Low");
          Serial.println("temp: "+temp);
          print_oled(temp, 3, 0,24);
        }else{
          temp = String(avg, 1)+"C";
          Serial.println("temp: "+temp);
          print_oled(temp, 3, 12,24);        
        }

        updated_flag = false;          
        updated_flag2 = false;
        sum = 0;

        digitalWrite(buzzerPin, HIGH);
        delay(100);
        digitalWrite(buzzerPin, LOW);
        delay(20);
        digitalWrite(buzzerPin, HIGH);
        delay(100);
        digitalWrite(buzzerPin, LOW);
        delay(2000);
      }      
    }else {
      if(updated_flag2 == false){
        print_oled(" Hello", 3, 0, 24);
        updated_flag = false;
        updated_flag2 = true;
      }
      digitalWrite(buzzerPin, LOW);  //turn on buzzer if too far
    }
    delay(250);
}

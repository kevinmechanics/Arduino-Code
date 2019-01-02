#include <dht.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DS1302.h>


  // Set the LCD 
LiquidCrystal_I2C lcd(0x3f, 16, 2);

  // Init the DS1302
DS1302 rtc(2, 3, 4);

  // Init a Time-data structure
Time t;

dht DHT;//create a variable type of dht

const int DHT11_PIN = 7; //Humiture sensor attach to pin7

// mq135
int sensorValue; 

//KY016 3-color LED module
int redpin = 11; // select the pin for the red LED
int bluepin = 10; // select the pin for the blue LED
int greenpin = 9 ;// select the pin for the green LED

int val;

void setup() {

  // put your setup code here, to run once:
  Serial.begin(9600);//initialize the serial monitor
   
  // initialize the LCD
  // Set the clock to run-mode, and disable the write protection
  lcd.init();  //initialize the lcd
  lcd.backlight();  //open the backlight
  lcd.begin(16, 2);
  rtc.halt(false);
  rtc.writeProtect(false);

  // The following lines can be commented out to use the values already stored in the DS1302
  rtc.setDOW(THURSDAY); // Set Day-of-Week
  rtc.setTime(12, 58, 30); // Set the time to 24hr format it will automatically take time according to AM/PM hr/min/sec
  rtc.setDate(25, 10, 2018); // Set the date to month/day/yr

  //rgb
  pinMode (redpin, OUTPUT);
  pinMode (bluepin, OUTPUT);
  pinMode (greenpin, OUTPUT);
 
}

void loop() {
  // put your main code here, to run repeatedly:
  render_clock();
  delay (1000);
  lcd.clear();
  render_dht();
  delay (1000);
  lcd.clear();
  render_mq135();
  delay (1000);
  lcd.clear();
}

void render_mq135(){
  
 sensorValue = analogRead(0);       // read analog input pin 0
 
 Serial.print("AirQua=");
 Serial.print(sensorValue, DEC);  // prints the value read
 Serial.println(" PPM");
 lcd.setCursor(0,0);
 lcd.print("ArQ=");
 lcd.print(sensorValue,DEC);
 lcd.print(" PPM");
 lcd.println("       "); 
 lcd.println("       "); 
 delay(100);     // wait 100ms for next reading

// if-else statement for air quality levels
lcd.setCursor(0,1);
if(sensorValue <= 50)
{
   
   Serial.print("Level:");
   Serial.print(" ");
   lcd.print("Level:");
   lcd.print("Good");
   Serial.print("Good");
   Serial.println(" ");  
   Serial.println("       ");
   digitalWrite(greenpin, HIGH);
   delay(1000);
}


else if(sensorValue > 51 && sensorValue <= 100)
{
 
   Serial.print("Level:");
   Serial.print(" ");
   lcd.print("Level:");
   lcd.print("Moderate");
   Serial.print("Moderate");
   Serial.println("");
   Serial.println("       ");
   analogWrite(redpin, 255);
   analogWrite(greenpin, 255);
   analogWrite(bluepin, 0);
   delay(1000);
}

else if(sensorValue > 101 && sensorValue <= 150)
{
   
   Serial.print("Level:");
   Serial.print(" ");
   lcd.print("Level:");
   lcd.print("Unhealthy For Sensitive Groups");
   Serial.print("Unhealthy For Sensitive Groups");
   Serial.println(" ");
   Serial.println("       ");
   analogWrite(redpin, 255);
   analogWrite(greenpin, 50);
   analogWrite(bluepin, 0);
   delay(1000);
}

else if(sensorValue > 151 && sensorValue <= 200)
{
   
   Serial.print("Level:");
   Serial.print(" ");
   lcd.print("Level:");
   lcd.print("Unhealthy");
   Serial.print("Unhealthy");
   Serial.println(" ");
   Serial.println("       ");
   analogWrite(redpin, 255);
   analogWrite(greenpin, 0);
   analogWrite(bluepin, 0);
   delay(1000);
   
}

else if(sensorValue > 201 && sensorValue <= 300)
{
  
   Serial.print("Level:");
   Serial.print(" ");
   lcd.print("Level:");
   lcd.print("Very Unhealthy");
   Serial.print("Very Unhealthy");
   Serial.println(" ");
   Serial.println("       ");
   analogWrite(redpin, 80);
   analogWrite(greenpin, 0);
   analogWrite(bluepin, 80);
   delay(1000);
  
}

else if(sensorValue > 301)
{
  
   Serial.print("Level:");
   Serial.print(" ");
   lcd.print("Level:");
   lcd.print("Hazardous!");
   Serial.print("Hazardous");
   Serial.println(" ");
   Serial.println("       ");
   analogWrite(redpin, 128);
   analogWrite(greenpin, 0);
   analogWrite(bluepin, 0);
   delay(1000);  
}
}

void render_dht() {

  

  // READ DATA
  //Serial.println("DHT11:");

  int chk = DHT.read11(DHT11_PIN);//read the value returned from sensor
  
  switch (chk)
  {
    case DHTLIB_OK:
      //Serial.println("OK!");
      break;
    case DHTLIB_ERROR_CHECKSUM:
      //Serial.print("Checksum error,\t");
      break;
    case DHTLIB_ERROR_TIMEOUT:
      //Serial.print("Time out error,\t");
      break;
    default:
      //Serial.print("Unknown error,\t");
      break;
  }
  // DISPLAY DATA
  lcd.setCursor(0, 0);
  lcd.print("Tem:");
  Serial.print("Tem:");
  lcd.print(DHT.temperature, 1); //print the temperature on lcd
  Serial.print(DHT.temperature, 1);
  lcd.print(char(223));//print the unit" ℃ "
  lcd.print("C");
  Serial.println(" C");
  lcd.setCursor(0, 1);
  lcd.print("Hum:");
  Serial.print("Hum:");
  lcd.print(DHT.humidity, 1); //print the humidity on lcd
  Serial.print(DHT.humidity, 1);
  lcd.print(" %");
  Serial.println(" %");

}

void render_clock() {
  
  // Get data from the DS1302
  t = rtc.getTime();

  lcd.setCursor(0, 0);
  if (t.date <= 9) {
    lcd.setCursor(0, 0);
    lcd.print("0");

    Serial.print("0");
    lcd.setCursor(1, 0);
    lcd.print(t.date, DEC);
    Serial.print(t.date, DEC);
  }
  else {
    lcd.print(t.date, DEC);
    Serial.print(t.date, DEC);
  }

  lcd.setCursor(3, 0);
  lcd.print(rtc.getMonthStr());
  Serial.print(" ");
  Serial.print(rtc.getMonthStr());
  lcd.setCursor(12, 0);
  lcd.print(t.year, DEC);
  Serial.print(" ");
  Serial.print(t.year, DEC);
  lcd.setCursor(4, 1);

{ if (t.hour >= 12) {
      lcd.setCursor(13, 1);
      lcd.print("PM");
      Serial.print(" ");
      Serial.print("PM");
      lcd.setCursor(4, 1);
      t.hour = t.hour - 12;
      if (t.hour == 0) {
        t.hour = 12; // Day 12 PM
      }
      if (t.hour <= 9) {
        lcd.setCursor(4, 1);
        lcd.print("0");

        Serial.print("0");
        lcd.setCursor(5, 1);
        lcd.print(t.hour, DEC);

        Serial.print(t.hour, DEC);
      }
      else {
        lcd.print(t.hour, DEC);

        Serial.print(t.hour, DEC);
      }
    }
    else {
      if (t.hour == 0) {
        t.hour = 12; // Night 12 AM
      }
      lcd.setCursor(13, 1);
      lcd.print("AM");
      Serial.print(" ");
      Serial.print("AM");
      lcd.setCursor(4, 1);
      if (t.hour <= 9) {
        lcd.setCursor(4, 1);
        lcd.print("0");

        Serial.print("0");
        lcd.setCursor(5, 1);
        lcd.print(t.hour, DEC);
  
        Serial.print(t.hour, DEC);
      }
      else {
        lcd.print(t.hour, DEC);
        Serial.print(t.hour, DEC);
      }
    }
  }
  lcd.setCursor(6, 1);
  lcd.print(":");
  Serial.print(":");
  lcd.setCursor(7, 1);
  if (t.min <= 9) {
    lcd.setCursor(7, 1);
    lcd.print("0");
    Serial.print("0");
    lcd.setCursor(8, 1);
    lcd.print(t.min, DEC);
    Serial.print(t.min, DEC);
  }
  else {
    lcd.print(t.min, DEC);
    Serial.print(t.min, DEC);
  }

  lcd.setCursor(9, 1);
  lcd.print(":");
  Serial.print(":");
  lcd.setCursor(10, 1);
  if (t.sec <= 9) {
    lcd.setCursor(10, 1);
    lcd.print("0");
    Serial.print("0");
    lcd.setCursor(11, 1);
    lcd.print(t.sec, DEC);
    Serial.print(t.sec, DEC);
  }
  else {
    lcd.print(t.sec, DEC);
    Serial.print(t.sec, DEC);
  }

  Serial.println("");
}

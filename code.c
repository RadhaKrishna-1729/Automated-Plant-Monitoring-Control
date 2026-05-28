#include <Keypad.h>
#include "Arduino_SensorKit.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
const int ROWS=4,COLS=3;
char keys[ROWS][COLS]={
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
  };
   
byte rowPins[ROWS]={2,3,4,5};
byte colPins[COLS]={6,10,7};
char plant[2]={'0','0'};
char height[5]={'0','0','0','0','0'};
int idx=0;
int index=0;
int BUZZ=8;
int plant_height; 
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
int moisture_value;
float moisture_percentage;
float moisture;
int LED=A2;
int flag=0;
int ENA = 11;
int IN1 = A0;
int IN2 = 9;
const int sensor_pin = A0;  //change value here
int sensor;
int threshold = 10;  //change value here
int pin_temp=12;
float temp;
float h;
float light;
float min_light=20;
float max_light=90;
int lightsensor_pin=11;

void beep(int on_ms, int off_ms) {
  digitalWrite(BUZZ, HIGH);
  delay(on_ms);
  digitalWrite(BUZZ, LOW);
  delay(off_ms);
}

void setup()
{
  Environment.setPin(12);
  pinMode(A0, INPUT);//moisture
  Serial.begin(9600);
  Environment.begin();
  lcd.init(); 
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(1,1);
  lcd.print("Welcome");
 pinMode(BUZZ, OUTPUT);
  digitalWrite(BUZZ, LOW);     // start silent

  // Welcome pattern: short, short, long
  beep(120, 120);
  beep(120, 150);
  beep(350, 0);
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("We are...");
  lcd.setCursor(0,1);
  lcd.print("Circuit Breakers");
  delay(2500);
  pinMode(13, INPUT); //smoke sensor
  pinMode(BUZZ, OUTPUT);//for buzzer
  digitalWrite(BUZZ, LOW);
  pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
  pinMode (ENA, OUTPUT);
  pinMode (LED, OUTPUT);// led
}
void LED_ON(){
    analogWrite(LED, 1023);
  delay(2000);
  LED_OFF();
}
void LED_OFF(){
  analogWrite(LED, 0);
  delay(2000);
}

int moisture_sensor(){
  int sensorValue = analogRead(A1);
  int moisturePercent = map(sensorValue, 0, 1023, 100, 0); 
  Serial.print("Soil Moisture: ");
  Serial.print(moisturePercent);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Moisture percent:");
  lcd.setCursor(0,1);
  lcd.print(moisturePercent);
  digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence);
  delay(2000);
  return moisturePercent;
}

void gas_sensor()
{
  float smoke= analogRead(A3);
  if(smoke>1000)
  {
  flag=1;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Fire! Fire!");
  digitalWrite(BUZZ, HIGH); delay(2000);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(400);  // silence
   digitalWrite(LED_BUILTIN, HIGH); // ON
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  }
}
void left_forward (){
digitalWrite (IN1, HIGH);
digitalWrite (IN2, LOW);
analogWrite (ENA, 220);
}

void stop_left (){
  digitalWrite (IN1, LOW);
digitalWrite (IN2, LOW);
analogWrite (ENA, 0);
}


void left_mode_1 (){
  left_forward ();
  delay(2000);
  stop_left() ;
}
void left_mode_2 (){
  left_forward ();
  delay(5000);
  stop_left() ;
}
void left_mode_3 (){
  left_forward ();
  delay(10000);
  stop_left() ;
}

float photoresistor() {
sensor = analogRead (lightsensor_pin);
float photo_value=map(sensor, 0, 1023, 0, 100);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Light:      %");
lcd.setCursor(8, 0);
lcd.print(photo_value);
delay(2000);
return photo_value;
}

float temp_sensor() {
  float tempC=Environment.readTemperature(); 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temp:      C");
  lcd.setCursor(6, 0);
  lcd.print(tempC);
  float humid=Environment.readHumidity(); 
  lcd.setCursor(0,1);
  lcd.print("Humidity");
  lcd.setCursor(9, 1);
  lcd.print(humid);
  lcd.setCursor(14, 1);
  lcd.print("%");
  digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
  delay(4000);
  return tempC;
}

void tomato()
{
  moisture= moisture_sensor();
  temp= temp_sensor();
  light= photoresistor();
  h=height_plant();
  //Seedlings (0–15 cm): Soil 60–70% (ON < 65%, OFF ~80–85%); Temp ~20–25°C; Sun 6–8+ h.
  if(h>=0 && h<=15)
    {
      if(moisture<=65)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Remove water");
    digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
    delay(3000);
  }
      if(moisture>82)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Switch pump on!");
    digitalWrite(BUZZ, HIGH); delay(200); // beep briefly
    left_mode_3(); 
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
    delay(3000);
  }
      if(temp<20)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Increase temp");
    digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
    delay(3000);
  }
      if(temp>25)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Decrease temp");
    digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
    delay(3000);
  }
      if(light<20)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Increasing light");
    digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
    LED_ON();
    delay(3000);
  }
      if(light>90)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Decreasing light");
    digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
    delay(3000);
    LED_OFF();
  }
    }
//Vegetative (15–60 cm): Soil 70–80% (ON < 70%, OFF ~85%); Temp ~22–29°C; Sun 8+ h.
  if(h>=15 && h<=60)
    {
      if(moisture<=70)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Switch pump on!");
    digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
    left_mode_2();
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
    delay(3000);
  }
      if(moisture>85)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Remove Water!");
    digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
    delay(3000);
  }
      if(temp<22)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Increase temp");
    digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
    delay(3000);
  }
      if(temp>29)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Decrease temp");
    digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
    delay(3000);
  }
      if(light<min_light)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Increasing light");
    digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
    delay(3000);
    LED_ON();
  }
      if(light>max_light)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Decreasing light");
    digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
    delay(3000);
    LED_OFF();
  }
    }
//Flowering/Fruiting (60+ cm): Soil 70–85% (ON < 70–75%, OFF ~85–90%); Best set ~21–24°C, avoid >32°C; Sun 6–8+ h with heat protection.
  if(h>60)
    {
      if(moisture<=73)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Switch pump on!");
    digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
    left_mode_1();
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
    delay(3000);
  }
      if(moisture>87)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Remove Water!");
    digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
    delay(3000);
  }
      if(temp<21)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Increase temp");
    digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
    delay(3000);
  }
      if(temp>24 && temp<30)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Decrease temp");
    digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
    delay(3000);
  }
      if(temp>=30)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Danger");
    lcd.setCursor(0,1);
    lcd.print("Too Hot!!");
    delay(3000);
    digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
  }
      if(light<min_light)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Increasing light");
    digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
    delay(3000);
    LED_ON();
  }
      if(light>max_light)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Decreasing light");
    digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
    delay(3000);
    LED_OFF();
  }
    }
}
void strawberry()
{
  moisture = moisture_sensor();
  temp = temp_sensor();
  light= photoresistor();
  h=height_plant();
 // ----- Seedling Stage (0–15 cm) -----
  if (h >= 0 && h <= 15)
  {
    if (moisture <= 65)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Switch pump on!");
        digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
    left_mode_3();
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
      delay(3000);
    }
    if (moisture > 82)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Remove water!");
      digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
      delay(3000);
    }
    if (temp < 18)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Too Cold");
      delay(3000);
    }
    if (temp > 24)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Too Hot");
      delay(3000);
    }
    if (light < min_light)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Add Light");
      delay(3000);
      LED_ON();
    }
    if (light > max_light)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Reduce Light");
      delay(3000);
      LED_OFF();
    }
  }
  // ----- Vegetative Stage (15–25 cm) -----
  if (h > 15 && h <= 25)
  {
    if (moisture <= 70)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Switch pump on!");
      digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
    left_mode_2();
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
      delay(3000);
    }
    if (moisture > 85)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Remove water!");
      digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
      delay(3000);
    }
    if (temp < 20)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Too Cold");
      digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
      delay(3000);
    }
    if (temp > 26)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Too Hot");
      digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
      delay(3000);
    }
    if (light < min_light)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Add Light");
      digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
      delay(3000);
      LED_ON();
    }
    if (light > max_light)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Reduce Light");
      digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
      delay(3000);
      LED_OFF();
    }
  }
  // ----- Flowering/Fruiting Stage (25+ cm) -----
  if (h > 25)
  {
    if (moisture <= 73)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Switch pump on!");
      digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
      left_mode_1();
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
      delay(3000);
    }
    if (moisture > 87)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Remove water!");
      digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
      delay(3000);
    }
    if (temp < 18)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Too Cold");
      digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
      delay(3000);
    }
    if (temp > 24 && temp < 30)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("High Temp");
      digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
      delay(3000);
    }
    if (temp >= 30)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Danger!");
      lcd.setCursor(0,1);
      lcd.print("Too Hot!!");
      delay(3000);
    }
    if (light < min_light)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Add Light");
      digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
      delay(3000);
      LED_ON();
    }
    if (light > max_light)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Reduce Light");
      digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
      delay(3000);
      LED_OFF();
    }
  }
}
void cucumber()
{
  moisture = moisture_sensor();
  temp = temp_sensor();
  light= photoresistor();
  h=height_plant();
  // Seedlings (0–10 cm): Soil 65–75% (ON < 65%, OFF ~80%); Temp ~22–28°C; Sun 6–8+ h.
  if(h>=0 && h<=10)
    {
      if(moisture<=65)
        {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Switch pump on!");
          digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
          left_mode_3();
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
          delay(3000);
        }
      if(moisture>80)
        {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Remove water!");
          digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
          delay(3000);
        }
      if(temp<22)
        {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Increase temp");
          digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
          delay(3000);
        }
      if(temp>28)
        {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Decrease temp");
          digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
          delay(3000);
        }
      if(light<min_light)
        {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Increasing light");
          digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
          delay(3000);
          LED_ON();
        }
      if(light>max_light)
        {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Decreasing light");
          digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
          delay(3000);
          LED_OFF();
        }
    }
  // Vegetative (10–60 cm): Soil 70–80% (ON < 70%, OFF ~85%); Temp ~24–30°C; Sun 8+ h.
  if(h>10 && h<=60)
    {
      if(moisture<=70)
        {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Switch pump on!");
          digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
          left_mode_2();
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
          delay(3000);
        }
      if(moisture>85)
        {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Remove water!");
          digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
          delay(3000);
        }
      if(temp<24)
        {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Increase temp");
          digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
          delay(3000);
        }
      if(temp>30)
        {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Decrease temp");
          digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
          delay(3000);
        }
      if(light<min_light)
        {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Increasing light");
          digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
          delay(3000);
          LED_ON();
        }
      if(light>max_light)
        {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Decreasing light");
          digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
          delay(3000);
          LED_OFF();
        }
    }
  // Flowering/Fruiting (60+ cm): Soil 70–85% (ON < 70–75%, OFF ~85–90%);
  // Best set ~22–26°C, avoid >30°C; Sun 6–8+ h with shading.
  if(h>60)
    {
      if(moisture<=72)
        {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Switch pump on!");
          digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
          left_mode_1();
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
          delay(3000);
        }
      if(moisture>87)
        {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Remove water!");
          digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
          delay(3000);
        }
      if(temp<22)
        {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Increase temp");
          digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
          delay(3000);
        }
      if(temp>26 && temp<30)
        {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Decrease temp");
          digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
          delay(3000);
        }
      if(temp>=30)
        {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Danger");
          lcd.setCursor(0,1);
          lcd.print("Too Hot!!");
          delay(3000);
          digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
        }
      if(light<min_light)
        {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Increasing light");
          digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
          delay(3000);
          LED_ON();
        }
      if(light>max_light)
        {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Decreasing light");
          digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
          delay(3000);
          LED_OFF();
        }
    }
}
void no_soil()
{
  moisture= moisture_sensor();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(moisture);
  delay(2000);
  temp=temp_sensor();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(temp);
  delay(2000);
  light= photoresistor();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(light);
  delay(2000);
}
void spinach ()
{
  moisture= moisture_sensor();
  temp= temp_sensor();
light= photoresistor();
h=height_plant();
  //Seedlings (0–5 cm): Soil 70–85% (ON < 70%, OFF >85%); Temp ~10–15°C; Sun 4–6 h.
  if(h>=0 && h<=5)
    {
      if(moisture<=70)
  {
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Switch pump on!");
   digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
   left_mode_3();
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
   delay(3000);
  }
      if(moisture>85)
  {
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Remove water!");
   digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
   delay(3000);
  }
      if(temp<10)
  {
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Increase temp");
   digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
   delay(3000);
  }
      if(temp>20)
  {
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Decrease temp");
   digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
   delay(3000);
  }
      if(light<min_light)
  {
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Increasing light");
   digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
   delay(3000);
   LED_ON();
  }
      if(light>max_light)
  {
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Decreasing light");
   digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
   delay(3000);
   LED_OFF();
  }
    }
//Vegetative (5–15 cm): Soil 65–75% (ON < 65%, OFF >75%); Temp ~10–20°C; Sun 4-6 h.
  if(h>=5 && h<=15)
    {
      if(moisture<=65)
  {
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Switch pump on!");
   digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
   left_mode_2();
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
   delay(3000);
  }
      if(moisture>75)
  {
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Remove water!");
   digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
   delay(3000);
  }
      if(temp<10)
  {
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Increase temp");
   digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
   delay(3000);
  }
      if(temp>20)
  {
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Decrease temp");
   digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
   delay(3000);
  }
      if(light<min_light)
  {
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Increasing light");
   digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
   delay(3000);
   LED_ON();
  }
      if(light>max_light)
  {
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Decreasing light");
   digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
   delay(3000);
   LED_OFF();
  }
    }
//Mature (15+ cm): Soil 60–70% (ON < 60%, OFF >70%); Temp ~8-20°C; Sun 4-6 h with heat protection.
  if(h>15)
    {
      if(moisture<=60)
  {
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Switch pump on!");
   digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
   left_mode_1();
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
   delay(3000);
  }
      if(moisture>70)
  {
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Remove water!");
   digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
   delay(3000);
  }
      if(temp<10)
  {
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Increase temp");
   digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
   delay(3000);
  }
      if(temp>20)
  {
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Decrease temp");
   digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
   delay(3000);
  }
      if(temp>=25)
  {
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Danger");
   lcd.setCursor(0,1);
   lcd.print("Too Hot!!");
   delay(3000);
   digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
  }
      if(light<min_light)
  {
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Increasing light");
   digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
   delay(3000);
   LED_ON();
  }
      if(light>max_light)
  {
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Decreasing light");
   digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
   delay(3000);
   LED_OFF();
  }
    }
}
void rose ()
{
  moisture = moisture_sensor();
  temp = temp_sensor();
  light = photoresistor();
  h=height_plant();
  // Seedlings (0–10 cm): Soil 65–75% (ON <65%, OFF >75%); Temp ~18–24°C; Sun 4–6 h.
  if(h >= 0 && h <= 10)
  {
    if(moisture <= 65)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Switch pump on!");
      digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
      left_mode_3();
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
      delay(3000);
    }
    if(moisture > 75)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Remove water");
      digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
      delay(3000);
    }
    if(temp < 18)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Increase temp");
      digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
      delay(3000);
    }
    if(temp > 26)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Decrease temp");
      digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
      delay(3000);
    }
    if(light < min_light)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Increasing light");
      digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
      delay(3000);
      LED_ON();
    }
    if(light > max_light)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Decreasing light");
      digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
      delay(3000);
      LED_OFF();
    }
  }
  // Vegetative (10–30 cm): Soil 60–70% (ON <60%, OFF >70%); Temp ~18–28°C; Sun 5–7 h.
  if(h > 10 && h <= 30)
  {
    if(moisture <= 60)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Switch pump on!");
      digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
      left_mode_2();
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
      delay(3000);
    }
    if(moisture > 70)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Remove water!");
      digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
      delay(3000);
    }
    if(temp < 18)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Increase temp");
      digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
      delay(3000);
    }
    if(temp > 28)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Decrease temp");
      digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
      delay(3000);
    }
    if(light < min_light)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Increasing light");
      digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
      delay(3000);
      LED_ON();
    }
    if(light > max_light)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Decreasing light");
      digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
      delay(3000);
      LED_OFF();
    }
  }
  // Flowering / Mature (30+ cm): Soil 55–65% (ON <55%, OFF >65%); Temp ~15–25°C; Sun 6+ h with heat protection.
  if(h > 30)
  {
    if(moisture <= 55)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Switch pump on!");
      digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
      left_mode_1();
      digitalWrite(BUZZ, LOW);  delay(800);  // silence
      delay(3000);
    }
    if(moisture > 65)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Remove water!");
      digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
      delay(3000);
    }
    if(temp < 15)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Increase temp");
      digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
      delay(3000);
    }
    if(temp > 25)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Decrease temp");
      digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
      delay(3000);
    }
    if(temp >= 32)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Danger");
      lcd.setCursor(0,1);
      lcd.print("Too Hot!!");
      delay(3000);
      digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
    }
    if(light < min_light)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Increase light");
      digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
      delay(3000);
      LED_ON();
    }
    if(light > max_light)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Reduce light");
      digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
      delay(3000);
      LED_OFF();
    }
  }
}
int height_plant(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Enter the value");
  lcd.setCursor(0,1);
  lcd.print("of height in cm");
  digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
  while(index<3)
  {
    char key=keypad.getKey();
    if(key)
    {
      height[index]=key;
      index++;
      Serial.println(key);
    }
  }
  plant_height=(height[0]-'0')*100+(height[1]-'0')*10+(height[2]-'0');
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Plant height:");
  lcd.setCursor(0,1);
  lcd.print(plant_height);
  digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence
  delay(3000);
  return plant_height;
}
void loop()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Select plant-");
  lcd.setCursor(0,1);
  lcd.print("Check the sheet");
  delay(200);
    while(idx<1)
  {
    char key=keypad.getKey();
    if(key)
    {
      plant[idx]=key;
      idx++;
      Serial.println(key);
      lcd.clear();
      lcd.setCursor(1,1);
      lcd.print(plant);
    }
  }
if(plant[0]=='1')
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Your selection:");
  lcd.setCursor(0,1);
  lcd.print("No plant");
  digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence);
  delay(2000);
  no_soil();
}
else if(plant[0]=='2')
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Your selection:");
  lcd.setCursor(0,1);
  lcd.print("Tomato");
  digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence);
  delay(2000);
  tomato();
}
else if(plant[0]=='3')
{lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Your selection:");
  lcd.setCursor(0,1);
  lcd.print("Cucumber");
  digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence);
  delay(2000);
  cucumber();
}
else if(plant[0]=='4')
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Your selection:");
  lcd.setCursor(0,1);
  lcd.print("Rose");
  digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence);
  delay(2000);
  rose();
}
else if(plant[0]=='5')
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Your selection:");
  lcd.setCursor(0,1);
  lcd.print("Spinach");
  digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence);
  delay(2000);
  spinach();
}
else if(plant[0]=='6')
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Your selection:");
  lcd.setCursor(0,1);
  lcd.print("Strawberry");
  digitalWrite(BUZZ, HIGH); delay(200);  // beep briefly
  digitalWrite(BUZZ, LOW);  delay(800);  // silence);
  delay(2000);
  strawberry();
}
gas_sensor();
index=0;
idx=0;
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Thank you");
delay(10000);
}

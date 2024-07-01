unsigned long startMillis,startMillis1,currentMillis1,startMillis2,currentMillis2;
unsigned long currentMillis;

#include <FastLED.h>
#include <Keypad.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4

#define switch1 2
#define fan     3
#define relay   4
#define buzzer  5
#define led1    6
#define LED     7
#define led2    9
#define led3   10
#define led4   11
#define ldr    A0

#define NUM_LEDS  1

CRGB leds[NUM_LEDS];
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);
char key1;
const int ROW_NUM = 2; //four rows
const int COLUMN_NUM = 3; //three columns
int fan_val11,switch_in = 0,soldering=1,time_interval,i=0,page =0,page1 = 0 ;
int relay_1_in,change,total;
int c,total_val,led1_val=0,led2_val=0,led3_val=0,led4_val=0,fan_val=0;
float relay_time_in,fan_v,led1_v,led2_v,led3_v,led4_v;
int led_1_disp,led_2_disp,led_3_disp,led_4_disp,led_5_disp;
char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3'},
  {'4','5','6'}
};

byte pin_rows[ROW_NUM] = {12,13}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {A1,A2,A3}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

void setup(){
  pinMode(12,OUTPUT);
    pinMode(13,OUTPUT);
  digitalWrite(12,1);digitalWrite(13,1);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);
startMillis=startMillis1=startMillis2 = c =0;
Serial.begin(9600);
pinMode(led1,OUTPUT);
pinMode(led2,OUTPUT);
pinMode(led3,OUTPUT);
pinMode(led4,OUTPUT);
pinMode(buzzer,OUTPUT);
pinMode(switch1,INPUT);
pinMode(fan,OUTPUT);
pinMode(relay,OUTPUT);
pinMode(ldr,INPUT);

FastLED.addLeds<WS2812B, LED, GRB>(leds, NUM_LEDS);
FastLED.setBrightness(50);

leds[0] = CRGB::White;
FastLED.show();
display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //or 0x3C
display.setTextColor(SSD1306_WHITE);
//display.clearDisplay(); //for Clearing the display
//display.drawBitmap(0, 0, NSIdeas, 128, 64, WHITE); // display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
//display.display();
//delay(3000);
//display.clearDisplay();
//display.setCursor( 15, 18); display.setTextSize(2);
//display.print(F("NS Ideas"));
//display.setCursor( 21, 43); display.setTextSize(1);
//display.print(F("...Presents..."));
//display.display();
//delay(2000);
}

void display_oled_disp( int mode,int led_1,int led_2,int led_3,int led_4,int relay_1,float relay_time, int fan_speed,int led_5){
display.clearDisplay();
display.setTextSize(1);
if(mode == 1){display.setCursor( 3, 20); display.print(">");}
else if(mode == 2){display.setCursor( 3, 31); display.print(">");}
else if(mode == 3){display.setCursor( 3, 41); display.print(">");}
else if(mode == 4){display.setCursor( 3, 51); display.print(">");}
//if(led_1 == 1){ display.setCursor( 60, 29); display.print("1");}
//else { display.setCursor( 60, 29); display.print("0");}
//if(led_2 == 1){ display.setCursor( 77, 29); display.print("2");}
//else { display.setCursor( 77, 29); display.print("0");}
//if(led_3 == 1){ display.setCursor( 95, 29); display.print("3");}
//else { display.setCursor( 95, 29); display.print("0");}
//if(led_4 == 1){ display.setCursor(113, 29); display.print("4");}
//else { display.setCursor(113, 29); display.print("0");}

display.setCursor( 60, 29); display.print(led_1);
display.setCursor( 77, 29); display.print(led_2);
display.setCursor( 95, 29); display.print(led_3);
display.setCursor( 113, 29); display.print(led_4);
if(relay_1==1){display.setCursor( 60, 18); display.print("on");}
else {display.setCursor( 60, 18); display.print("off");}
display.setCursor( 52, 0); display.print("Main");
display.setCursor( 13, 20); display.print("Relay");
display.setCursor( 50, 20); display.print(":");
display.setCursor( 83, 18); display.print(relay_time/60);
display.setCursor( 110, 18); display.print("Min");
display.setCursor( 13, 31); display.print("LED");
display.setCursor( 50, 31); display.print(":");
display.setCursor( 13, 41); display.print("Fan");
display.setCursor( 50, 41); display.print(":");
display.setCursor( 65, 40); display.print("<");
display.setCursor( 78, 40); display.print(fan_speed);
display.setCursor( 103, 40); display.print(">");
display.setCursor( 13, 51); display.print("Top LED");
display.setCursor( 50, 51); display.print(":");
display.setCursor( 65, 51); display.print("<");
display.setCursor( 78, 51); display.print(led_5);
display.setCursor( 103, 51); display.print(">");
}
void display_oled_disp1( float select_relay_delay ){
display.clearDisplay();
display.setTextSize(1);
display.setCursor( 15, 18); display.print("Delay in Seconds");
display.setTextSize(2);
display.setCursor( 33, 30); display.print("Relay");
display.setCursor( 23, 49); display.print("<");
display.setCursor( 39, 49); display.print(select_relay_delay/60);
display.setCursor( 90, 49); display.print(">");
}
void display_oled_disp4( float select_relay_delay ){
display.clearDisplay();
display.setTextSize(1);
display.setCursor( 15, 18); display.print("Buzzer Volume");
display.setTextSize(2);
display.setCursor( 33, 30); display.print("Buzzer");
display.setCursor( 23, 49); display.print("<");
display.setCursor( 39, 49); display.print(select_relay_delay/60);
display.setCursor( 90, 49); display.print(">");
}
void display_oled_disp3( float select_relay_delay, int mode1 ){
display.clearDisplay();
display.setTextSize(1);
display.setCursor( 18, 18); display.print("Speed     Delay");
display.setTextSize(2);
if(mode1 == 1){display.setCursor( 3, 30); display.print(">");}
else if(mode1 == 2){display.setCursor( 3, 49); display.print(">");}
display.setCursor( 46, 1); display.print("FAN");
display.setCursor( 23, 30); display.print("<");
display.setCursor( 39, 30); display.print(select_relay_delay);
display.setCursor( 90, 30); display.print(">");
display.setCursor( 23, 49); display.print("<");
display.setCursor( 39, 49); display.print(select_relay_delay);
display.setCursor( 90, 49); display.print(">");
}
void display_oled_disp2(int led1_in,int led2_in,int led3_in,int led4_in,int fan_in,int fan_speed){
display.clearDisplay();
display.setTextSize(1);
//if(mode2 == 1){display.setCursor( 3, 20); display.print(">");}
//else if(mode2 == 2){display.setCursor( 3, 31); display.print(">");}
//else if(mode2 == 3){display.setCursor( 3, 41); display.print(">");}
//else if(mode2 == 4){display.setCursor( 3, 51); display.print(">");}
//display.setCursor( 55, 0); display.print("LED");
display.setCursor( 15,  0); display.print("Fan");
display.setCursor( 50,  0); display.print(":");
display.setCursor( 65,  0); display.print("<");
display.setCursor( 78,  0); display.print(fan_speed);
display.setCursor(103,  0); display.print(">");
display.setCursor( 15, 20); display.print("LED 1");
display.setCursor( 50, 20); display.print(":");
display.setCursor( 65, 20); display.print("<");
display.setCursor( 78, 20); display.print(led1_in);
display.setCursor(103, 20); display.print(">");
display.setCursor( 15, 31); display.print("LED 2");
display.setCursor( 50, 31); display.print(":");
display.setCursor( 65, 31); display.print("<");
display.setCursor( 78, 31); display.print(led1_in);
display.setCursor( 103, 31); display.print(">");
display.setCursor( 15, 41); display.print("LED 3");
display.setCursor( 50, 41); display.print(":");
display.setCursor( 65, 41); display.print("<");
display.setCursor( 78, 41); display.print(led1_in);
display.setCursor( 103, 41); display.print(">");
display.setCursor( 15, 51); display.print("LED 4");
display.setCursor( 50, 51); display.print(":");
display.setCursor( 65, 51); display.print("<");
display.setCursor( 78, 51); display.print(led1_in);
display.setCursor( 103, 51); display.print(">");
}
void on_and_off(int led1_in, int led2_in, int led3_in, int led4_in, int fan_in){
    analogWrite(led1,led1_in);
    analogWrite(led2,led2_in);
    analogWrite(led3,led3_in);
    analogWrite(led4,led4_in);
    analogWrite(fan,fan_in);
    

   
}
void loop(){

   display.clearDisplay();
   currentMillis = millis()/1000;
//   key1 = keypad.getKey();
   switch_in = digitalRead(switch1);
   if(digitalRead(A2)==1){led1_val=int(led1_v+18);
       // if(led1_v>80.00)led1_v==0;
        //delay(200);
   }

   if(digitalRead(A3)==1){
        led3_val=int(led3_v+18);
       // if(led3_v>80)led3_v==0;
        //delay(200);
   }

   if(digitalRead(A1)==1){
        fan_val=int(fan_v+50);
        delay(200);
        if(fan_v > 100)fan_v==0;
   }
        
   if(switch_in == 1){
      analogWrite(buzzer,255);
      delay(200);
      analogWrite(buzzer,0);
      if(c==0||(currentMillis - startMillis >= 180)){
          time_interval = relay_time_in = 180;   c=1;  delay(1000);
      }
      else if(currentMillis - startMillis >= 120){
          time_interval = relay_time_in = 120;         delay(1000);
      }
      else if(currentMillis - startMillis >= 60){
          time_interval = relay_time_in =  60;         delay(1000);
      }
      else if(currentMillis - startMillis < 60){time_interval = relay_time_in = 30;delay(1000);}
          relay_1_in=1;   soldering = i = 0;   digitalWrite(relay,0);
          startMillis = startMillis1 = startMillis2 = currentMillis;
   }
   if(soldering == 0){
      if (currentMillis - startMillis1 >= 1){
          leds[0] = CRGB::Red; FastLED.show();   i++;   startMillis1 = currentMillis;
          analogWrite(buzzer,10);
      }
      if (currentMillis - startMillis2 >= 2){
          leds[0] = CRGB::Black; FastLED.show();   startMillis2 = currentMillis;
          analogWrite(buzzer,0);
      }
      if (currentMillis - startMillis >= time_interval){
          digitalWrite(relay,1);
          soldering = 1;relay_1_in =0;   startMillis = currentMillis;
          leds[0] = CRGB::White; FastLED.show();
          analogWrite(buzzer,255);   delay(200);   analogWrite(buzzer,0);
      }
   }
////   if(page == 0){
////     display_oled_disp(1,0,0,0,0,relay_1_in,relay_time_in, 100,1);
////     if(page1==1){
////        display_oled_disp3( 1,2 );
////     }
////     else if(page1==1){
////       display_oled_disp2( 1,255 );
////     }
////     else if(page1==1){
////      display_oled_disp1( 60);
////     }
////   }
////   total_val =led1_val+led2_val+led3_val+led4_val+fan_val; 
////   if(total_val!=total||change==0){
////    display_oled_disp2(led1_val,led2_val,led3_val,led4_val,fan_val);
////    total=total_val;
////    }
////   else{

    if(led1_val>=1){led_1_disp==1;}
    else led_1_disp==0;
    if(led2_val>=1){led_2_disp==1;}
    else led_2_disp==0;
    if(led3_val>=1){led_3_disp==1;}
    else led_3_disp==0;
    if(led4_val>=1){led_4_disp==1;}
    else led_4_disp==0;
    if(fan_val>=1){led_5_disp==1;}
    else led_5_disp==0;
    display_oled_disp(1,led1_val,led2_val,led3_val,led4_val,relay_1_in,relay_time_in,fan_val,fan_val);
////    }
   on_and_off(led1_val,led2_val,led3_val,led4_val,fan_val);
   //on_and_off(10,10,10,10,40);
   display.setTextSize(2);
   display.setCursor(0 , 0);
   display.print(currentMillis);
   display.setCursor(92 , 0);
   display.print(time_interval-i);
   display
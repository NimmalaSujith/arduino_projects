#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <SPI.h>
#include <SD.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

#define fan 18 
#define WIFILEDRed 21
#define WIFILEDGreen 25
#define green 32
#define yellow 33
#define R 12
#define G 14
#define B 27
#define s 35
#define NUM_LEDS  1
#define BOTtoken "5230029267:AAGKQKco7xEHuT1C0M1LIgn-Jaqjlnp-0Xw"

char ssid[] = "Balu"; char password[] = "9494004099";
long Bot_lasttime;   //last time messages' scan has been done
bool Start = false;
int a,change,c,led_count,r_value=0,g_value=0,b_value=0,len_text,message_count;
const int chipSelect = 10;
char charRead;
String link1,data_ID,text,from_name,chat_id,buffer1,file_text,comment,buffer2,start_comment,start_link1,temp_text;

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
File chatIDData,startFileData,FileData;
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);



void handleNewMessages(int numNewMessages){
    Serial.println("handleNewMessages");
    Serial.println(String(numNewMessages));
    for (int i = 0; i < numNewMessages; i++){
        chat_id = String(bot.messages[i].chat_id);
        text = bot.messages[i].text;
        from_name = bot.messages[i].from_name;
        if (from_name == "") from_name = "Guest";
        if(text[0]=='/'){
            len_text = text.length();
            for(int j =1;j<=len_text;j++){
                temp_text = temp_text+text[j];
            }
            text = temp_text;
            temp_text = "";
        }
       
        if(text =="start"){
            bot.sendMessage(chat_id,chat_id, "");
        }
        else {
            bot.sendMessage(chat_id, "Sorry "+ from_name +",\n" + text + ",Data is not available", "");
        }

    message_count++;
    display.clearDisplay(); display.setTextSize(1);
    display.setCursor( 7,  0); display.print("IP :");
    display.setCursor(37,  0); display.print(WiFi.localIP());
    display.setCursor( 2, 14); display.print("Message Count : "+ String(message_count));
    display.setCursor( 2, 25); display.print("Chat ID:");
    display.setCursor(52, 25); display.print(String(chat_id));
    display.setCursor( 5, 35); display.print("> " + String(from_name));
    display.setCursor( 5, 45); display.print("> " + String(text));display.display();
    bot.sendMessage("1314922309",String(chat_id) + "  :  " + String(from_name)+ "  :  " + String(text), "");  
  }
}
    

//void deleteFile()
//{
// //delete a file:
//  if (SD.exists(fileName)) 
//    {
//    Serial.println("Removing simple.txt");
//    SD.remove(fileName);
//    Serial.println("Done");
//   } 
//}


void setup() {
    Serial.begin(9600);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.setTextColor(SSD1306_WHITE);
//    display.clearDisplay();
//    display.setCursor( 15, 18); display.setTextSize(2);
//    display.print(F("NS Ideas"));
//    display.setCursor( 21, 43); display.setTextSize(1);
//    display.print(F("...Presents..."));
//    display.display();
//    delay(3000);
    display.clearDisplay();
    ledcAttachPin(WIFILEDRed,0);  ledcSetup(0,5000,8);
    ledcAttachPin(WIFILEDGreen,1);  ledcSetup(1,5000,8);
    ledcAttachPin(green,2);  ledcSetup(2,5000,8);
    ledcAttachPin(yellow,3);  ledcSetup(3,5000,8);
    ledcAttachPin(R,4);  ledcSetup(4,5000,8);
    ledcAttachPin(G,5);  ledcSetup(5,5000,8);
    ledcAttachPin(B,6);  ledcSetup(6,5000,8);
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    Serial.print("Connecting Wifi: ");
    display.clearDisplay();
    display.setCursor( 0, 5); display.setTextSize(2);
    display.print("Connecting \n   Wifi: ");display.display();
    delay(1000);
    Serial.println(ssid);
    display.clearDisplay();
    display.setCursor( 40, 25); display.setTextSize(2);
    display.print(ssid);display.display();
    delay(1000);
    WiFi.begin(ssid, password);
//    ledcWrite(0,100);
//    delay(200);
//    ledcWrite(0,0);
//    delay(100);
//    ledcWrite(0,100);
//    delay(200);
    ledcWrite(0,0);
    while (WiFi.status() != WL_CONNECTED) {
        display.clearDisplay();
        display.setCursor( 30, 27); display.setTextSize(1);
        display.print(F("connecting..."));display.display();
        ledcWrite(0,100);
        delay(300);
        ledcWrite(0,0);
        delay(200);
    }
    delay(1000);
    if(WiFi.status() == WL_CONNECTED){ledcWrite(1,150);display.print(F("...connected..."));}
    Serial.println("");
    Serial.println("WiFi connected");
    display.clearDisplay();
    display.setCursor( 20, 27); display.setTextSize(1);
    display.print("WiFi connected");display.display();
    delay(1000);
    client.setInsecure(); // if yr core cannot handle secure connections
    pinMode(fan,OUTPUT);
    pinMode(s,INPUT);
    
    while (!Serial) {
     ; // wait for serial port to connect. Needed for native USB port only
     }
   
     display.display();
}

void loop() {
    if (millis() > Bot_lasttime + 100)  {
        int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
        while (numNewMessages) {
            Serial.println();
            display.clearDisplay();display.setCursor( 15, 10); 
            display.setTextSize(2);display.print("  Got\n Response");
            display.display();delay(100);
            handleNewMessages(numNewMessages);
            numNewMessages = bot.getUpdates(bot.last_message_received + 1);
            
        }
        Bot_lasttime = millis();
        
    }
    else if(WiFi.status() != WL_CONNECTED){display.clearDisplay();display.setCursor( 15, 27); display.setTextSize(1);display.print("WiFi Not Connected");display.display();}
}

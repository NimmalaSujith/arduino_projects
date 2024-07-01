#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#define BOTtoken "5159467417:AAHDLTWAUJ9JmCKkYslmhpVmrThfMiuDIOM"
char ssid[] = "Balu"; char password[] = "9494004099";
int mod=0,i,members = 3;
char data,array[][50]= {"1314922309","1525418086","870288444"};
long Bot_lasttime;   //last time messages' scan has been done
bool Start = false;
int a,change,c,led_count,r_value=0,g_value=0,b_value=0,len_text,message_count;
const int chipSelect = 10;
char charRead;
String link1,data_ID,text,from_name,chat_id,buffer1,file_text,comment,buffer2,start_comment,start_link1,temp_text;

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
//0 pin output

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
        if(text =="stop"&&(chat_id=="1314922309" ||chat_id == "1525418086" ||chat_id == "870288444")){
            mod =5;
            bot.sendMessage(chat_id,"stopping the warning", "");
        }
        else if (chat_id!="1314922309" && chat_id != "1525418086" && chat_id != "870288444"){bot.sendMessage(chat_id,"You are not User", "");}
        if(text !="stop") {
            bot.sendMessage(chat_id, "Sorry "+ from_name +"sir,\n" + text + ",Data is not available \n To stop the warning text as /stop", "");
        }    
    }
}
void setup() {
    Serial.begin(9600);
    pinMode(2,INPUT);
    pinMode(0,OUTPUT);
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    Serial.print("Connecting Wifi: ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {Serial.println("");}
    Serial.println("WiFi connected");
    client.setInsecure();
    data = 'n';    mod =0; 
}

void loop() {
  //data = Serial.read();
  //if(digitalRead(2) == 0){ mod =0; }
  Serial.print(data);
  Serial.print(mod);
  if(mod == 0 && data == 'n'){
      for(i = 0; i<members;i++){
          bot.sendMessage(array[i],"Check the robot", "");
      }
      
  }
  else if(mod == 0 && data == 'y'){
      for(i = 0; i<members;i++){
          bot.sendMessage(array[i],"Security Breache", "");
      }
      }
  else Serial.print("Scanning");
  int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
        while (numNewMessages) {
            handleNewMessages(numNewMessages);
            numNewMessages = bot.getUpdates(bot.last_message_received + 1);      
        }
}

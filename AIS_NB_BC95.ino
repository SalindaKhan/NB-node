#include "AIS_NB_BC95.h"
#include <Thread.h>
String apnName = "devkit.nb";

String serverIP = "202.139.192.99"; // Your Server IP
String serverPort = "41234"; // Your Server Port
Thread myThread = Thread();

String udpData = "N01";

AIS_NB_BC95 AISnb;

const long interval = 1000;  //millisecond
unsigned long previousMillis = 0;

long cnt = 0;

void udpSent(){
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval)
    {
      cnt++;     
           
      // Send data in String 
      UDPSend udp = AISnb.sendUDPmsgStr(serverIP, serverPort, udpData+String(cnt));
   
      //Send data in HexString     
      //udpDataHEX = AISnb.str2HexStr(udpData);
      //UDPSend udp = AISnb.sendUDPmsg(serverIP, serverPort, udpDataHEX);
      previousMillis = currentMillis;
    }
}

void setup()
{ 
  AISnb.debug = true;
  
  Serial.begin(9600);
  pinMode(6,OUTPUT);
 
  AISnb.setupDevice(serverPort);

  String ip1 = AISnb.getDeviceIP();  
  delay(1000);
  
  pingRESP pingR = AISnb.pingIP(serverIP);
  previousMillis = millis();

  myThread.onRun(udpSent);
  myThread.setInterval(10000);  

}
void loop()
{ 
  if(myThread.shouldRun())
    myThread.run();
    
  UDPReceive resp = AISnb.waitResponse();
  if(AISnb.toString(resp.data)!=""){
    Serial.println("Data print : " + AISnb.toString(resp.data).substring(3));
    if(AISnb.toString(resp.data)=="CLI00"){ 
      digitalWrite(6,LOW);
    }else if(AISnb.toString(resp.data)=="CLI11"){ 
      digitalWrite(6,HIGH);
    }
  }
  
}

/*
  This program waits for serial commands and sends data to a server 

//=======================================================================
           _   __                  _                  
          | | / /                 | |                 
          | |/ /   __ _  ___  ___ | | __  ___   _ __  
          |    \  / _` |/ __|/ __|| |/ / / _ \ | '_ \ 
          | |\  \| (_| |\__ \\__ \|   < | (_) || | | |
          \_| \_/ \__,_||___/|___/|_|\_\ \___/ |_| |_|
                                                        
//=======================================================================

*/
#include <Arduino.h>     //add arduino framework
#include <ESP8266WiFi.h> //add esp8266wifi library

const char *ssid = "---------";    //Enter your wifi SSID
const char *password = "--------"; //Enter your wifi Password

const char *host = "---------"; //enter the host url
const uint16_t port = 80;       //enter the port

WiFiClient client; // Use WiFiClient class to create TCP connections

String data_from_rx; //string to put usefull incoming data
String in_string;    //string to put all incoming data
//=======================================================================
//                    Power on setup
//=======================================================================
void setup()
{
  Serial.begin(19200);        //create a serial connection
  WiFi.mode(WIFI_STA);        //open wifi in sta mode
  WiFi.begin(ssid, password); //Connect to wifi

  Serial.println("Connecting to Wifi"); //print the present state of the code
  while (WiFi.status() != WL_CONNECTED) //Wait for connection
  {
    //delay(500);               //print for debuging
    //Serial.print(".");        //print for debuging
    //delay(500);               //print for debuging
  }

  //Serial.println("");                   //print for debuging
  //Serial.print("Connected to ");        //print for debuging
  //Serial.println(ssid);                 //print for debuging
  //Serial.print("ESP IP address: ");     //print for debuging
  //Serial.println(WiFi.localIP());       //print for debuging
  //Serial.print(" on port ");            //print for debuging
  //Serial.println(port);                 //print for debuging
  //Serial.println("");                   //print for debuging

  //Serial.print("connecting to ");       //print for debuging
  //Serial.print(host);                   //print for debuging
  //Serial.print(':');                    //print for debuging
  //Serial.println(port);                 //print for debuging

  if (!client.connect(host, port)) // create a TCP connection
  {
    Serial.println("connection failed"); //inform if the connection fails
    delay(5000);                         //delay for 5 sec
    return;                              //return to the code
  }
}
//=======================================================================
//                    Loop
//=======================================================================

void loop()
{
  if (Serial.available() > 0) //check for serial data
  {
    in_string = Serial.readStringUntil('\n');               //putting the data util new line
    int until_command = in_string.indexOf(':');             //finds where is located the end of the command
    String command = in_string.substring(0, until_command); //put the command to string command
    if (command == "send")                                  //check if the command is equal to send
    {
      data_from_rx = in_string.substring(until_command + 1); //put the data received after the command in data_from_rx
      if (!client.connect(host, port))                       // create a TCP connection
      {
        Serial.println("connection failed"); //inform if the connection fails
        delay(5000);                         //delay for 5 sec
        return;                              //return to the code
      }
      if (client.connected()) //check if the client is connected
      {
        client.println(data_from_rx); //send via tcp the data
      }
      unsigned long timeout = millis(); //check the time and put it in the timeout
      while (client.available() == 0)   // wait for data to be available
      {
        if (millis() - timeout > 5000) //if no data available for 5 sec
        {
          Serial.println(">>> Client Timeout !"); //inform for timeout
          client.stop();                          //Disconnect from the server
          return;                                 //return to the code
        }
      }
    }
    if (command == "connection") //if the command is connection
    {
      data_from_rx = in_string.substring(until_command + 1); //put the subcommand received after the command in data_from_rx
      if (data_from_rx == "connect")                         //if the subcommand is connect
      {
        WiFi.mode(WIFI_STA);                  //open wifi in sta mode
        delay(1);                             //delay for stability
        WiFi.begin(ssid, password);           //Connect to wifi
        Serial.println("Connecting to Wifi"); //print the present state of the code
        while (WiFi.status() != WL_CONNECTED) // Wait for connection
        {
        }
        if (!client.connect(host, port)) // create a TCP connection
        {
          Serial.println("connection failed"); //inform if the connection fails
          delay(5000);                         //delay for 5 sec
          return;                              //return to the code
        }
      }
      if (data_from_rx == "reconnect") //if the subcommand is connect
      {
        client.stop();                   //Disconnect from the server
        delay(1);                        //delay for stability
        if (!client.connect(host, port)) // create a TCP connection
        {
          Serial.println("connection failed"); //inform if the connection fails
          delay(5000);                         //delay for 5 sec
          return;                              //return to the code
        }
      }
      if (data_from_rx == "terminate") //if the subcommand is terminate
      {
        client.stop();       //Disconnect from the server
        delay(10);           //delay for stability
        WiFi.mode(WIFI_OFF); //close the wifi
      }
    }
    in_string = ""; //clean the data for the next use
  }
  while (client.connected() && client.available()) //while data are available to read
  {
    String line = client.readStringUntil('\n'); //putting the data from the server util new line
    Serial.println(line);                       //print the data
  }
}

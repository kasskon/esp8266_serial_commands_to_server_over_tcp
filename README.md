# code for esp8266 take serial commands and trasmit data to a server over tcp
```
//=======================================================================
           _   __                  _                  
          | | / /                 | |                 
          | |/ /   __ _  ___  ___ | | __  ___   _ __  
          |    \  / _` |/ __|/ __|| |/ / / _ \ | '_ \ 
          | |\  \| (_| |\__ \\__ \|   < | (_) || | | |
          \_| \_/ \__,_||___/|___/|_|\_\ \___/ |_| |_|
                                                        
//=======================================================================
```

### Upload the main.cpp to an ESP8266.
You can use serial commands. 
The commands are :
```
send, connection
```
the connection is using sub commands, the send command can be implemented like: 
```
send:data 	//this command takes the data and sends the over tcp
```

The connection command can be implemented like: 
```
connection:subcommand 
```
The subcommand can be : 
>connect, reconnect, terminate 

## serial commands
```
send:123 	//this command takes the 123 and sends the over tcp to the server

connection:connect  // opens this wi-fi and creates a connection to the server 

connection:reconnect // closes the connection with the server and open the connection again 

connection:terminate // closes the connection with the server and closes the Wi-Fi
```


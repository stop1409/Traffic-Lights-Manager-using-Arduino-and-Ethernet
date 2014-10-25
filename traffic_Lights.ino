

#include <SPI.h>      //import spi library
#include <Ethernet.h>       //import ethernet library


int ledR1 = 2 ,ledY1 = 3,ledG1 = 4;
int ledR2 = 5 ,ledY2 = 6,ledG2 = 7;
int ledR3 = 8 ,ledY3 = 9,ledG3 = A2;
int ledR4 = A3 ,ledY4 = A4,ledG4 = A5;

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:

byte mac[] = { 0x54, 0x55, 0x58, 0x10, 0x00, 0x24 };  // entspricht einer MAC von 84.85.88.16.0.36
byte ip[]  = { 192, 168, 1, 100 };                  // IP-Adresse
byte gateway[] = { 192, 168, 1, 1 };                // Gateway
byte subnet[]  = { 255, 255, 255, 0 };                // Subnet

int x =0;
// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);



String readString = String(100);      // string for fetching data from address
boolean Street1N = false;                  // Status flag
boolean Street2N = false;
boolean Street3N = false;
boolean Street4N = false;



void setup()
{
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
  //for setup all pin mode as output
  pinMode(ledR1, OUTPUT);
  pinMode(ledY1, OUTPUT);
  pinMode(ledG1, OUTPUT);
  pinMode(ledR2, OUTPUT);
  pinMode(ledY2, OUTPUT);
  pinMode(ledG2, OUTPUT);
  pinMode(ledR3, OUTPUT);
  pinMode(ledY3, OUTPUT);
  pinMode(ledG3, OUTPUT);
  pinMode(ledR4, OUTPUT);
  pinMode(ledY4, OUTPUT);
  pinMode(ledG4, OUTPUT);
  
  //fot frist run all light red
  digitalWrite(ledR1, HIGH);
  digitalWrite(ledR2, HIGH);
  digitalWrite(ledR3, HIGH);
  digitalWrite(ledR4, HIGH);
   
}

//main function
void loop()
{
  if(!x==1)
    login();
    else
    html();
}


//life cyrcl of traffic signal 
void led(int red , int yallow ,int green)
{
   digitalWrite(red, LOW);
   digitalWrite(green, HIGH);
   delay(5000);
   digitalWrite(yallow, HIGH);
   delay(3000);
   digitalWrite(green, LOW);
   digitalWrite(yallow, LOW);
   digitalWrite(red, HIGH);
}
  
//for atomatic control light
void defaultMode(){
  led(ledR1,ledY1,ledG1); 
  led(ledR2,ledY2,ledG2);
  led(ledR3,ledY3,ledG3);
  led(ledR4,ledY4,ledG4);
}

   
//for static open street control light 
void openS (int x){
  if(x == 1){
    digitalWrite(ledR1, LOW);
    digitalWrite(ledY1, LOW);
    digitalWrite(ledG1, HIGH);
  }
  else if(x == 2){
    digitalWrite(ledR2, LOW);
    digitalWrite(ledY2, LOW);
    digitalWrite(ledG2, HIGH);
  }
  else if(x == 3){
    digitalWrite(ledR3, LOW);
    digitalWrite(ledY3, LOW);
    digitalWrite(ledG3, HIGH);
  }
  else{
    digitalWrite(ledR4, LOW);
    digitalWrite(ledY4, LOW);
    digitalWrite(ledG4, HIGH); 
  }
}
   
//for static close street control light
void closeS(int x){
  if(x == 1){
    digitalWrite(ledR1, HIGH);
    digitalWrite(ledY1, LOW);
    digitalWrite(ledG1, LOW);
  }
  else if(x == 2){
    digitalWrite(ledR2, HIGH);
    digitalWrite(ledY2, LOW);
    digitalWrite(ledG2, LOW);
  }
  else if(x == 3)
  {
    digitalWrite(ledR3, HIGH);
    digitalWrite(ledY3, LOW);
    digitalWrite(ledG3, LOW);
  }
  else{
    digitalWrite(ledR4, HIGH);
    digitalWrite(ledY4, LOW);
    digitalWrite(ledG4, LOW);
  }
}
   

//html page function
void html(){
  // Create a client connection
  EthernetClient client = server.available();
  if(client) {
  while (client.connected()) {
  if(client.available()) {
  char c = client.read();

  //read char by char HTTP request
  if (readString.length() < 100) {

  //store characters to string
  readString = readString + c; 

  // very simple but it works...
  }

  Serial.print(c);  //output chars to serial port

  if(c == '\n') {  //if HTTP request has ended


  // indexOf locates a character or String within another String.
  // Returns the index of val within the String, or -1 if not found.
  
  /***********************************
  ************ Street One ************
  ***********************************/
  //if street 1 open
  if(readString.indexOf("1=ON") > -1) {  
    openS(1);
    Serial.println(" street 1 open!");
    Street1N = true;
   }
   //if street 1 Close
  if(readString.indexOf("1=OFF") > -1){
    closeS(1);
    Serial.println(" street 1 Close!");
    Street1N = false;
  }


  /***********************************
  ************ Street Two ************
  ***********************************/
  //if street 2 open
  if(readString.indexOf("2=ON") > -1) {
    openS(2);
    Serial.println(" street 2 Open!");
    Street2N = true;
  }
   //if street 2 Close
  if(readString.indexOf("2=OFF") > -1){
    closeS(2);
    Serial.println(" street 2 Close!");
    Street2N = false;
  }

  /***********************************
  ************ Street Three **********
  ***********************************/
  //if street 3 open
  if(readString.indexOf("3=ON") > -1){
    openS(3);
    Serial.println(" street 3 Open!");
    Street3N = true;
  }
  //if street 3 Close
  if(readString.indexOf("3=OFF") > -1){
    closeS(3);
    Serial.println("street 3 Close!");
    Street3N = false;
  }


  /***********************************
  ************ Street Four ***********
  ***********************************/
  //if street 4 open
 if(readString.indexOf("4=ON") > -1){
   openS(4);
   Serial.println(" street 4 Open!");
   Street4N = true;
 }
 //if street 3 Close
 if(readString.indexOf("4=OFF") > -1){
   closeS(4);
   Serial.println("street 4 Close!");
   Street4N = false;
  
  }

  /***********************************
  ************ Default Mode **********
  ***********************************/
  if(readString.indexOf("state=Automatic+Mode") > -1) {
    defaultMode();
    Serial.println("Automatic Mode");
    Street1N = false;
    Street2N = false;
    Street3N = false;
    Street4N = false;
  }
  
  /***********************************
  ************ Logout ****************
  ***********************************/
  if(readString.indexOf("state=logout") > -1) {
    x=0;
  }
  
  //--------------------------HTML------------------------
  client.println("HTTP/1.1 200 OK");

  client.println("Content-Type: text/html");

  client.println();

  client.print("<html>");
  client.println("<meta http-equiv=\"refresh\" content=\"5\">");

  client.print("<head>");

  client.print("<title>Trafic Signal control Application </title>");

  client.println("</head>");

  client.print("<body bgcolor='#444444'>");

  //---Body---
  client.println("<br><hr />");

  client.println("<h1><div align='center'><font color='#2076CD'>Wellcome in Trafic Signal Control Application</font color></div></h1>");
 
  client.println("<h1><div align='center'><font color='#2076CD'>Hassan Zalah & Yasser Zalah</font color></div></h1>");

  client.println("<hr /><br>");


  //---Control Panel---
  client.println("<div align='center'><font face='Verdana' color='#FFFFFF'>Control Panel:</font></div>");

  client.println("<br>");

  client.println("<table align='center' border='1' width='500' cellpadding='5'>");

  client.println("<tr bgColor='#222222'>");

  client.println("<td bgcolor='#222222'><font face='Verdana' color='#CFCFCF' size='2'>Street Number One <br></font></td>");
 
  client.println("<td align='center' bgcolor='#222222'><form method=get><input type=submit name=1 value='ON'></form></td>");
 
  client.println("<td align='center' bgcolor='#222222'><form method=get><input type=submit name=1 value='OFF'></form></td>");
 
  if(Street1N){
    client.println("<td align='center'><font color='green' size='5'>ON");
  }
  else {
    client.println("<td align='center'><font color='red' size='5'>OFF");
  }
  client.println("</tr>");
  //*****************************************************************
  client.println("<tr bgColor='#222222'>");

  client.println("<td bgcolor='#222222'><font face='Verdana' color='#CFCFCF' size='2'>Street Number Two<br></font></td>");
 
  client.println("<td align='center' bgcolor='#222222'><form method=get><input type=submit name=2 value='ON'></form></td>");
 
  client.println("<td align='center' bgcolor='#222222'><form method=get><input type=submit name=2 value='OFF'></form></td>");
 
  if(Street2N){
    client.println("<td align='center'><font color='green' size='5'>ON");
  }
  else{
    client.println("<td align='center'><font color='red' size='5'>OFF");
  }
  client.println("</tr>");
  //*****************************************************************

  client.println("<tr bgColor='#222222'>");
  
  client.println("<td bgcolor='#222222'><font face='Verdana' color='#CFCFCF' size='2'>Street Number Three<br></font></td>");
 
  client.println("<td align='center' bgcolor='#222222'><form method=get><input type=submit name=3 value='ON'></form></td>");
 
  client.println("<td align='center' bgcolor='#222222'><form method=get><input type=submit name=3 value='OFF'></form></td>");
 
  if(Street3N){
    client.println("<td align='center'><font color='green' size='5'>ON");
  }
  else{
    client.println("<td align='center'><font color='red' size='5'>OFF");
  }
  client.println("</tr>");
  //*****************************************************************

  client.println("<tr bgColor='#222222'>");

  client.println("<td bgcolor='#222222'><font face='Verdana' color='#CFCFCF' size='2'>Street Number Four<br></font></td>");
 
  client.println("<td align='center' bgcolor='#222222'><form method=get><input type=submit name=4 value='ON'></form></td>");
 
  client.println("<td align='center' bgcolor='#222222'><form method=get><input type=submit name=4 value='OFF'></form></td>");
 
  if(Street4N){
    client.println("<td align='center'><font color='green' size='5'>ON");
  }
  else{
    client.println("<td align='center'><font color='red' size='5'>OFF");
  }
  client.println("</tr>");
  
  client.println("<tr bgColor='#222222'>");
  client.println("<td align='center' colspan='4'><form method=get><input type=submit name=state value='Automatic Mode'></form>");
  client.println("</td>");  
  client.println("</tr>");
  
  //*******************************************************************
  client.println("</table>");
  client.println("<br>");
  
  client.println("<div align='center'><form align='center' method=get><input type=submit name=state value='logout'></form></div>");
  
  client.println("</body></html>");
  
  //clearing string for next read
  readString="";

  //stopping client
  client.stop();
}
}
}
}
}
   
   
   
   
   
   
   
   
   
   
//html page function
void login(){
  // Create a client connection
  EthernetClient client = server.available();
  if(client) {
  while (client.connected()) {
  if(client.available()) {
  char c = client.read();

  //read char by char HTTP request
  if (readString.length() < 100) {

  //store characters to string
  readString = readString + c; 

  // very simple but it works...
  }

  Serial.print(c);  //output chars to serial port

  if(c == '\n') {  //if HTTP request has ended


  // indexOf locates a character or String within another String.
  // Returns the index of val within the String, or -1 if not found.
 

  /***********************************
  ************ Default Mode **********
  ***********************************/
  if(readString.indexOf("state?user=Admin&password=Admin") > -1) {
    html();
    x=1;
  }
  
  //--------------------------HTML------------------------
  client.println("HTTP/1.1 200 OK");

  client.println("Content-Type: text/html");

  client.println();

  client.print("<html>");
  

  client.print("<head>");

  client.print("<title>Trafic Signal control Application </title>");

  client.println("</head>");

  client.print("<body bgcolor='#444444'>");

  //---Body---
  client.println("<br><hr />");

  client.println("<h1><div align='center'><font color='#2076CD'>Wellcome in Trafic Signal Control Application</font color></div></h1>");
 
  client.println("<h1><div align='center'><font color='#2076CD'>Hassan Zalah  &amp; Yasser Zalah</font color></div></h1>");

  client.println("<hr /><br>");
  
  client.println("<div align='center'><form action='state'>");
  
  client.println("<font face='Verdana' color='#FFFFFF'> User:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");
  
  client.println("<input type='text' name='user'><br>");
  
  client.println("<font face='Verdana' color='#FFFFFF'> Password:");
  
  client.println("<input type='password' name='password'><br>");
  
  client.println("<input type='submit' value='Login'>");
  
  client.println("</body></html>");
  
  //clearing string for next read
  readString="";

  //stopping client
  client.stop();
}
}
}
}
}
   

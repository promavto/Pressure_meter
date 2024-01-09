/* ***********************************************************************************************
 * 
 * oldcurmudgeon November 6 2021
 * 
 * This sketch is for an ESP8266 to provide a web interface for blinking an LED
 *
 * The onboard LED is used so no additional hardware is needed
 *
 * It is implemented as 2 state machines. The first has the state defined by the pattern
 * being run. The state is defined by currentPattern. The state transition is done via 
 * a browser request changing newState to define the next state.
 *
 * The second state machine defines the step in the pattern of blinks. The state transition
 * occurs when the current time for the LED state expires. When the time is up the LED toggles
 * to the other state and the time for that state starts. At the end of the pattern it is
 * started over from the beginning. 
 * 
 *************************************************************************************************/ 


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "ASUS";
const char* password = "panasonic";

ESP8266WebServer server(80);

#include <string.h>
#include <Streaming.h>   //  from "Streaming by Mikal Hart" in library manager

// Pattern selection state machine variables
volatile uint16_t newPattern = 0;  //default to first pattern volatile because it is set asynchronously
uint16_t currentPattern = 0; // default first pattern

// Pattern step state machine variables
byte step = 0;                // current step in pattern default to start
unsigned long currentTime;   // current time for test
unsigned long startTime;     // start time of this step

// led on state - the onboard led is active low (when the pin is low the led
// is on). If you are using some other pin to drive the led so that the led is on when
// the pin is high this should be changed to one.
const byte LED_ON = 0; 
byte ledState = LED_ON;     // current led state default on

// led patterns - this is a series of numbers defining the led on and off times
// This example is 4 patterns of 6 changes. A zero before the end of the pattern
// indicates a short pattern that restarts when it detects a zero.
const byte NUM_PATTERNS = 4;    // number of patterns 
const byte PATTERN_LEN = 6;    // pattern length
const unsigned long pattern[ NUM_PATTERNS][PATTERN_LEN] = 
        {{200,200,200,200,200,200},
        {1500,1500,1500,1500,1500,01500},
        {1500,1500,200,200,0,0},           // short pattern example 
        {1500,1500,1500,1500,200,200}};
/* ***************************************************************************
*
* Handle root web page
*
******************************************************************************/
void handleRoot() {
  if (server.hasArg("pattern")){ // the browser has sent a response
    // Save new value from web page browser response  
    newPattern = server.arg("pattern").toInt();
  }
   /* Build html page
    * 
    * The new lines (\n) are not required by the browser - they are formatting for a human 
    * looking at the page source on the browser
    * 
    * The "String((newPattern == x) ? " checked" : " ")"  construct is used to set the
    * pattern selected as checked when the web page is refreshed. If newPage is equal to
    * the value "checked" is added to the attributes, otherwise a blank is added. You can
    * see the result on the web page by right-click then select "View Page Source".
    * This will show the actual HTML sent to the browser.
   */ 
  String htmlPage =
    String("<!DOCTYPE HTML>\n") 
    + "<html lang='en'>\n"
    + "<head>\n"
    + "<!-- Ignore request for nonexistent favicon -->\n"
    + "<link rel='icon' href='data:,'>\n"
    + " <meta name='viewport' content='width=device-width, initial-scale=1.0'>"
    +"</head>\n"
    + "<body>\n\n"
    + "<h1 style='text-align: center;'>LED Pattern Blinker</h1>\n"
    + "<div>\n"
    + "<form style='font-size: 20px'>\n"
    +   " <input type='radio' name='pattern' value='0' required"
    +  String((newPattern == 0) ? " checked" : " ")        
    +   " > Short blink<br>\n"
    +   " <input type='radio' name='pattern' value='1' required"
    +  String((newPattern == 1) ? " checked" : " ") 
    +   "> Long blink<br>\n"
    +   " <input type='radio' name='pattern' value='2' required"
    +  String((newPattern == 2) ? " checked" : " ") 
    +   "> Long Short blink <br>\n"
    +   " <input type='radio' name='pattern' value='3' required"
    +  String((newPattern == 3) ? " checked" : " ") 
    +   "> Long Long Short blink<br>\n"
    +   "<input type='submit' value='Set Pattern'>\n"
    +   "</div>\n"
    +"</body>\n</html>\n";

  server.send(200, "text/html",htmlPage);
    
}

void handleNotFound() {
    // no clue what they sent
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";

    for (uint8_t i = 0; i < server.args(); i++) {
      message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }

    server.send(404, "text/plain", message);

}

void setup() {
  pinMode(LED_BUILTIN,OUTPUT);

  Serial.begin(19200);
   

  // Set up WiFi network
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Set up mDNS responder:
  // - first argument is the domain name, in this example
  //   the fully-qualified domain name is "esp8266.local"
  // - second argument is the IP address to advertise
  //   we send our IP address on the WiFi network
  if (!MDNS.begin("esp8266")) {
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  
  // Start the server
  server.begin();
  Serial.println("TCP server started");

  // Add service to MDNS-SD
  MDNS.addService("http", "tcp", 80);
  
  startTime = millis();      // get current time for start of pattern
}

void checkPattern() {
    if (newPattern != currentPattern){     // browser has sent a new pattern
        // switch to new pattern
        ledState = LED_ON;  // start with led on
        digitalWrite(LED_BUILTIN,ledState);  // start with led on
        step = 0;   // start at beginning of pattern
        currentPattern = newPattern;  // switch to new pattern
        startTime = millis();      // get current time for start of pattern
    } 
};    

void loop() {
    
  MDNS.update();
     
  server.handleClient();      // check for connection request
  
  // Check to see if there is a new pattern request. If the check is done here
  // the new pattern starts immediately. If you want the new pattern to start 
  // after the current pattern has completed, comment out the line below and uncomment
  // the one in the end of pattern processing
  checkPattern();
  
  currentTime = millis();      // get current time for compare
  if (currentTime  - startTime > pattern[currentPattern][step]) {
     // done with this step 
     ledState ^= 1;  // toggle led state
     digitalWrite(LED_BUILTIN,ledState);  // change led state       
     step++;             // next step
     startTime = currentTime;  // start next led pattern step time
     if ((step >= PATTERN_LEN)                    // end of pattern
         || (pattern[currentPattern][step]==0)){  // or short pattern
        // end of pattern processing     
        step = 0;   //restart pattern
        
        // Check to see if there is a new pattern request. If the check 
        // is done here the new pattern starts after the current pattern
        // has completed. (Remember to comment out the call above.)
       // checkPattern();        
     }
  }     
}

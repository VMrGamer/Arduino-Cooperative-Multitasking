#include <Scheduler.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

uint8_t Relay1pin = 2;
bool Relay1status = LOW;

uint8_t Relay2pin = 14;
bool Relay2status = LOW;

uint8_t Relay3pin = 12;
bool Relay3status = LOW;

uint8_t Relay4pin = 13;
bool Relay4status = LOW;

ESP8266WebServer server(80);

class TouchTask: public Task {
  private:
    uint8_t pinIN;
    uint8_t* pinOUT;
    bool* out;
  protected:
    void setup(){
      Serial.print("Init GPIO");  Serial.println(pinIN);
      pinMode(pinIN, INPUT);
    }
    void loop(){
      if(digitalRead(pinIN)){
        *out = *out == HIGH ? LOW : HIGH;
        digitalWrite(*pinOUT, *out);
        //Serial.print("GPIO");  Serial.print(*pinOUT);  Serial.print(": ");  Serial.println(*out);
        delay(500);
        while(digitalRead(pinIN));
      }
    }
  public:
    TouchTask(uint8_t pinIN, uint8_t* pinOUT, bool* out){
      this->pinIN = pinIN;
      this->pinOUT = pinOUT;
      this->out = out;
    }
} tt4(16, &Relay1pin,&Relay1status), tt5(5, &Relay2pin,&Relay2status), tt6(4, &Relay3pin,&Relay3status), tt7(0, &Relay4pin,&Relay4status);

class WiFiServerTask: public Task {
  private:
    const char* ssid = "node_work";
    const char* password = "node_work";
  protected:
    void setup() {
      Serial.print("Connecting to ");  Serial.println(ssid);
      WiFi.begin(ssid, password);
      while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
      }
      Serial.println("");
      Serial.println("WiFi connected..!");
      Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

      server.on("/", handle_OnConnect);
      
      server.on("/relay1on", handle_relay1on);
      server.on("/relay1off", handle_relay1off);
      
      server.on("/relay2on", handle_relay2on);
      server.on("/relay2off", handle_relay2off);
      
      server.on("/relay3on", handle_relay3on);
      server.on("/relay3off", handle_relay3off);
      
      server.on("/relay4on", handle_relay4on);
      server.on("/relay4off", handle_relay4off);
      
      server.onNotFound(handle_NotFound);

      server.begin();
      Serial.println("HTTP server started");
    }
    
    void loop() {
      server.handleClient();
      digitalWrite(Relay1pin, Relay1status);
      digitalWrite(Relay2pin, Relay2status);
      digitalWrite(Relay3pin, Relay3status);
      digitalWrite(Relay4pin, Relay4status);
    }
public:
    static void handle_OnConnect() {
      Serial.print("R1/GPIO16 Status: ");  Serial.print(Relay1status == HIGH ? "ON" : "OFF");
      Serial.print(" | R2/GPIO5 Status: ");  Serial.print(Relay2status == HIGH ? "ON" : "OFF");
      Serial.print(" | R3/GPIO4 Status: ");  Serial.print(Relay3status == HIGH ? "ON" : "OFF");
      Serial.print(" | R4/GPIO0 Status: ");  Serial.println(Relay4status == HIGH ? "ON" : "OFF");
      server.send(200, "text/html", SendHTML(Relay1pin, Relay2pin, Relay3pin, Relay4pin)); 
    }

    static void handle_relay1on() {
      Relay1status = HIGH;
      Serial.println("R1/GPIO16 Status: ON");
      server.send(200, "text/html", SendHTML(true, Relay2status, Relay3status, Relay4status)); 
    }
    
    static void handle_relay1off() {
      Relay1status = LOW;
      Serial.println("R1/GPIO16 Status: OFF");
      server.send(200, "text/html", SendHTML(false, Relay2status, Relay3status, Relay4status)); 
    }
    
    static void handle_relay2on() {
      Relay2status = HIGH;
      Serial.println("R2/GPIO5 Status: ON");
      server.send(200, "text/html", SendHTML(Relay1status, true, Relay3status, Relay4status)); 
    }
    
    static void handle_relay2off() {
      Relay2status = LOW;
      Serial.println("R2/GPIO5 Status: OFF");
      server.send(200, "text/html", SendHTML(Relay1status, false, Relay3status, Relay4status)); 
    }

    static void handle_relay3on() {
      Relay3status = HIGH;
      Serial.println("R3/GPIO4 Status: ON");
      server.send(200, "text/html", SendHTML(Relay1status, Relay2status, true, Relay4status)); 
    }
    
    static void handle_relay3off() {
      Relay3status = LOW;
      Serial.println("R3/GPIO4 Status: OFF");
      server.send(200, "text/html", SendHTML(Relay1status, Relay2status, false, Relay4status)); 
    }
    
    static void handle_relay4on() {
      Relay4status = HIGH;
      Serial.println("R4/GPIO0 Status: ON");
      server.send(200, "text/html", SendHTML(Relay1status, Relay2status, Relay3status, true)); 
    }
    
    static void handle_relay4off() {
      Relay4status = LOW;
      Serial.println("R4/GPIO0 Status: OFF");
      server.send(200, "text/html", SendHTML(Relay1status, Relay2status, Relay3status, false)); 
    }

    static void handle_NotFound(){
      server.send(404, "text/plain", "Not found");
    }
    
    static String SendHTML(uint8_t relay1stat, uint8_t relay2stat, uint8_t relay3stat, uint8_t relay4stat){
      String ptr = "<!DOCTYPE html> <html>\n";
      ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
      ptr +="<title>LED Control</title>\n";
      ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
      ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
      ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
      ptr +=".button-on {background-color: #1abc9c;}\n";
      ptr +=".button-on:active {background-color: #16a085;}\n";
      ptr +=".button-off {background-color: #34495e;}\n";
      ptr +=".button-off:active {background-color: #2c3e50;}\n";
      ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
      ptr +="</style>\n";
      ptr +="</head>\n";
      ptr +="<body>\n";
      ptr +="<h1>ESP8266 Web Server</h1>\n";
      ptr +="<h3>Using Station(STA) Mode</h3>\n";
      
      if(relay1stat) {
        ptr +="<p>LED1 Status: ON</p><a class=\"button button-off\" href=\"/relay1off\">OFF</a>\n";
      }
      else {
        ptr +="<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/relay1on\">ON</a>\n";
      }
    
      if(relay2stat) {
        ptr +="<p>LED2 Status: ON</p><a class=\"button button-off\" href=\"/relay2off\">OFF</a>\n";
        }
      else {
        ptr +="<p>LED2 Status: OFF</p><a class=\"button button-on\" href=\"/relay2on\">ON</a>\n";
      }

      if(relay3stat) {
        ptr +="<p>LED3 Status: ON</p><a class=\"button button-off\" href=\"/relay3off\">OFF</a>\n";
      }
      else {
        ptr +="<p>LED3 Status: OFF</p><a class=\"button button-on\" href=\"/relay3on\">ON</a>\n";
      }
    
      if(relay4stat) {
        ptr +="<p>LED4 Status: ON</p><a class=\"button button-off\" href=\"/relay4off\">OFF</a>\n";
        }
      else {
        ptr +="<p>LED4 Status: OFF</p><a class=\"button button-on\" href=\"/relay4on\">ON</a>\n";
      }
    
      ptr +="</body>\n";
      ptr +="</html>\n";
      return ptr;
    }
} server_task;


void setup() {
  Serial.begin(9600);
  Serial.println("starting");
  
  pinMode(Relay1pin, OUTPUT);
  pinMode(Relay2pin, OUTPUT);
  pinMode(Relay3pin, OUTPUT);
  pinMode(Relay4pin, OUTPUT);
  
  delay(1000);

  Scheduler.start(&tt4);
  Scheduler.start(&tt5);
  Scheduler.start(&tt6);
  Scheduler.start(&tt7);

  Scheduler.start(&server_task);

  Scheduler.begin();
}

void loop() {}

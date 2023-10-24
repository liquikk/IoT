#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

void handle_root(){
  String page_code = "<form action=\"/LED\" method=\"POST\">";
  page_code += "<input type=\"submit\" value=\"Switch LED\">";
  page_code += "</form>";
  server.send(200, "text/html", page_code);
}

void handle_led(){
  bool current = digitalRead (led_pin);
  digitalWrite(led_pin, !current);
  server.sendHeader("Location", "/");
  server.send(303);
  
}

void server_init(){
  server.on("/", HTTP_GET, handle_root);
  server.on("/LED", HTTP_POST, handle_led);

  server.begin();
  Serial.println("HTTP Server is on, port " + String(web_port));
}

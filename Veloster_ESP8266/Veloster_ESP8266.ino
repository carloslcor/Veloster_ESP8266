#include <EEPROMFREEDOM.h>
#include <EEPROM.h>
#include <Carro.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#define MEM_ALOC_SIZE 512

#include <FS.h>
const char* imagefile = "/logoSemFundo.png";
//###### Config EEPROM ######
EEPROMFREEDOM eepromFreedom;
int ENDERECO_ACC_EEPROM = 0;
int ENDERECO_FAROIS_EEPROM = 2;
int ENDERECO_RETROVISORES_EEPROM = 4;
int ENDERECO_ARCONDICIONADO = 6;

Carro carro("Veloster", ENDERECO_ACC_EEPROM, ENDERECO_FAROIS_EEPROM, ENDERECO_RETROVISORES_EEPROM, ENDERECO_ARCONDICIONADO);

/* Set these to your desired credentials. */
const char *ssid = "Veloster";
const char *password = "20192019";

ESP8266WebServer server(80);

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
 * connected to this access point to see it.
 */
void handleRoot() {
String html ="<!DOCTYPE html> <html> <head> <title>Veloster</title> <meta charset=\"utf-8\" name=\"viewport\" content=\"width=device-width, minimumscale=1.0, maximum-scale=1.0, initial-scale=1\" /> <style> button:focus {outline:0;} body {color:rgba(255,255,255); background-color: black; text-align: center; max-width: 400px; margin: 10px auto;} #updateView { max-width: 400px; display: none; margin-top: 30px; } #dataVals { max-width: 400px; display: block; margin-top: 30px; } .displayInline {display: inline;} /* Estilo iOS */ .switch__container { margin: 15px auto; width: 60px; } .switch { visibility: hidden; position: absolute; margin-left: -9999px; } .switch + label { display: block; position: relative; cursor: pointer; outline: none; user-select: none; } .switch--shadow + label { padding: 2px; width: 60px; height: 30px; background-color: #dddddd; border-radius: 60px; } .switch--shadow + label:before, .switch--shadow + label:after { display: block; position: absolute; top: 1px; left: 1px; bottom: 1px; content: \"\"; } .switch--shadow + label:before { right: 1px; background-color: #f1f1f1; border-radius: 30px; transition: background 0.4s; } .switch--shadow + label:after { width: 30px; background-color: #fff; border-radius: 100%; box-shadow: 0 2px 5px rgba(0, 0, 0, 0.3); transition: all 0.4s; } .switch--shadow:checked + label:before { background-color: #8ce196; } .switch--shadow:checked + label:after { transform: translateX(30px); } .tableConfig { text-align: center; max-width: 400px; margin: 10px auto; } .onOff { display: inline-flex; margin-right: 15px; font-size: 15px; } .botaoStartStop { border-radius: 50%; height: 140px; width: 140px; border: 16px solid #000000; box-shadow: inset -10px -10px 10px #111, inset 10px 3px 5px #777; margin: 1.5em; margin-bottom: 9px; border: 5; padding: 0.5em 1em; color: #FFF; background: #444444; text-shadow: 1px 1px 1px rgba(0,0,0,0.5); box-shadow: 0px 0px 0 7px #ffffff, inset 2px 2px 25px #000000; transition: box-shadow 0.4s ease, transform 0.4s ease; } .botaoStartStop:active { box-shadow: 0px 0px 0 7px; /* adicionar #cc682b para mudar de cor ao apertar */ transform: translate(2px, 2px); border-color: #000000; } .seletorOnOffACC{ margin-right: 10px; margin-left: 10px; color: #444444; ; font-size: 15px; } .botaoOnStartStop{ box-shadow:inset -10px -10px 10px #111, inset 10px 3px 5px #777; box-shadow: 0px 0px 0 7px #0273fff0, inset 2px 2px 25px #000000; } .botaoOffStartStop{ box-shadow:inset -10px -10px 10px #111, inset 10px 3px 5px #777; box-shadow: 0px 0px 0 7px #ffffff, inset 2px 2px 25px #000000; } </style> </head> <body> <img src=\"\logoSemFundo.png\" alt=\"Image from ESP8266\" width=\"380\" height=\"110\"> <div style=\"margin: 20px\"> <h7>Última Alteração:</h7> <h7 id=\"retornoUpdate\"></h7> </div> <div> <div> <button onClick=\"ACCOnOff()\" class=\"botaoStartStop\" id=\"bss\"> <p style=\"margin: 0px; color:#0273ffbf; font-size: 12px\">ENGINE</p> <p style=\"margin: 0px; color:#0273ffbf; font-size: 15px\"><b>START</b></p> <p style=\"margin: 0px; color:#0273ffbf; font-size: 15px\"><b>STOP</b></p> </button> <div> <p> <span id=\"offBotaoSS\" class=\"seletorOnOffACC\">OFF</span> <span id=\"accBotaoSS\" class=\"seletorOnOffACC\">ACC</span> <span id=\"onBotaoSS\"class=\"seletorOnOffACC\">ON</span> </p> </div> </div> <table class=\"tableConfig\"> <tr> <td> <div class=\"onOff\"> <h2> Faróis </h2> </div> </td> <td> <div class=\"switch__container\" style=\"display: inline-flex;\"> <input onClick=\"FaroisOnOff(checked)\" id=\"switch-shadow-farois\" class=\"switch switch--shadow\" type=\"checkbox\"> <label for=\"switch-shadow-farois\"></label> </div> </td> </tr> <tr> <td> <div class=\"onOff\"> <h2> Retrovisores </h2> </div> </td> <td> <div class=\"switch__container\" style=\"display: inline-flex;\"> <input onClick=\"RetrovisoresOnOff(checked)\" id=\"switch-shadow-retrovisores\" class=\"switch switch--shadow\" type=\"checkbox\"> <label for=\"switch-shadow-retrovisores\"></label> </div> </td> </tr> <tr> <td> <div class=\"onOff\"> <h2> Ar Cond: <i><span id=\"temperatura\"></span> ºC <img src=\"\icons8-inverno-26.png\" alt=\"Image from ESP8266\"> </i> </h2> </div> </td> <td> <div class=\"switch__container\" style=\"display: inline-flex;\"> <input onClick=\"ArcondicionadoOnOff(checked)\" id=\"switch-shadow-arCondicionado\" class=\"switch switch--shadow\" type=\"checkbox\"> <label for=\"switch-shadow-arCondicionado\"></label> </div> </td> </tr> </table> </div> <script> var startStop; window.onload = function(){ getAllStatus() }; function ACCOnOff(){ if(startStop == 1){ setACCEstado(0); } if(startStop == 0){ setACCEstado(1); } } function FaroisOnOff(checked){ if(checked == true){ setFarois(1); } else{ setFarois(0); } } function RetrovisoresOnOff(checked){ if(checked == true){ setRetrovisores(1); } else{ setRetrovisores(0); } } function ArcondicionadoOnOff(checked){ if(checked == true){ setArcondicionado(1); } else{ setArcondicionado(0); } } function getAllStatus() { console.log(\"Entrou no getAll\"); var xhttp = new XMLHttpRequest(); xhttp.onreadystatechange = function() { if (this.readyState == 4 && this.status == 200) { var obj = JSON.parse(this.responseText); var acc = \"\" ; var farois = \"\"; var retrovisores = \"\"; var temperatura = 0; document.getElementById(\"temperatura\").innerHTML = obj.data[3].temperatura; if(obj.data[0].estadoACC == 0){ acc = \"Desligado\"; startStop = 0; }; if(obj.data[0].estadoACC == 1){ acc = \"Ligado\"; startStop = 1; }; if(obj.data[1].estadoFarois == 0){ farois = \"Desligado\"; document.getElementById(\"switch-shadow-farois\").checked = false; }; if(obj.data[1].estadoFarois == 1){ farois = \"Ligado\"; document.getElementById(\"switch-shadow-farois\").checked = true; }; if(obj.data[2].estadoRetrovisores == 1){ retrovisores = \"Ligado\"; document.getElementById(\"switch-shadow-retrovisores\").checked = true; }; if(obj.data[2].estadoRetrovisores == 0){ retrovisores = \"Desligado\"; document.getElementById(\"switch-shadow-retrovisores\").checked = false; }; if(obj.data[4].estadoArcondicionado == 1){ arCondicionado = \"Ligado\"; document.getElementById(\"switch-shadow-arCondicionado\").checked = true; }; if(obj.data[4].estadoArcondicionado == 0){ arCondicionado = \"Desligado\"; document.getElementById(\"switch-shadow-arCondicionado\").checked = false; }; if(acc == \"Ligado\"){ document.getElementById(\"onBotaoSS\").style = \"color: #0273fff0\"; document.getElementById(\"offBotaoSS\").style = \"color: #444444\"; document.getElementById(\"accBotaoSS\").style = \"color: #444444\"; document.getElementById(\"bss\").classList = \"botaoStartStop botaoOnStartStop\"; } if(acc == \"Desligado\"){ document.getElementById(\"onBotaoSS\").style = \"color: #444444\"; document.getElementById(\"offBotaoSS\").style = \"color: #ffffff\"; document.getElementById(\"accBotaoSS\").style = \"color: #444444\"; document.getElementById(\"bss\").classList = \"botaoStartStop botaoOffStartStop\"; } console.log(\"Objeto recebido: \"); console.log(obj.data[0], obj.data[1], obj.data[2], obj.data[3], obj.data[4]); }; }; xhttp.open(\"GET\", \"data\", true); xhttp.send(); }; function setACCEstado(status) { console.log(status); var xhttp = new XMLHttpRequest(); xhttp.onreadystatechange = function() { if (this.readyState == 4 && this.status == 200) { document.getElementById(\"retornoUpdate\").innerHTML = this.responseText; } }; xhttp.open(\"GET\", \"setACCEstado?valor=\"+status, true); xhttp.send(); getAllStatus(); window.setTimeout(getAllStatus, 1000); }; function setFarois(status) { var xhttp = new XMLHttpRequest(); xhttp.onreadystatechange = function() { if (this.readyState == 4 && this.status == 200) { document.getElementById(\"retornoUpdate\").innerHTML = this.responseText; } }; xhttp.open(\"GET\", \"setFarois?valor=\"+status, true); xhttp.send(); getAllStatus(); }; function setRetrovisores(status) { var xhttp = new XMLHttpRequest(); xhttp.onreadystatechange = function() { if (this.readyState == 4 && this.status == 200) { document.getElementById(\"retornoUpdate\").innerHTML = this.responseText; } }; xhttp.open(\"GET\", \"setRetrovisores?valor=\"+status, true); xhttp.send(); getAllStatus(); }; function setArcondicionado(status) { var xhttp = new XMLHttpRequest(); xhttp.onreadystatechange = function() { if (this.readyState == 4 && this.status == 200) { document.getElementById(\"retornoUpdate\").innerHTML = this.responseText; } }; xhttp.open(\"GET\", \"setArcondicionado?valor=\"+status, true); xhttp.send(); getAllStatus(); }; </script> </body> </html>";

  server.send(200, "text/html", html);
}

void handleWebRequests(){
  if(loadFromSpiffs(server.uri())) return;
  String message = "File Not Detected\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " NAME:"+server.argName(i) + "\n VALUE:" + server.arg(i) + "\n";
  }
  server.send(404, "text/html", message);
  Serial.println(message);
}

void getData() {   
   //This is a JSON formatted string that will be served. You can change the values to whatever like.
   // {"data":[{"dataValue":"1024"},{"dataValue":"23"}]} This is essentially what is will output you can add more if you like
  String text2 = "{\"data\":[";
  text2 += "{\"estadoACC\":\"";
  text2 += carro.GetEstadoACC();
  text2 += "\"},";
  text2 += "{\"estadoFarois\":\"";
  text2 += carro.GetEstadoFarois();
  text2 += "\"},";
  text2 += "{\"estadoRetrovisores\":\"";
  text2 += carro.GetEstadoRetrovisores();
  text2 += "\"},";  
  text2 += "{\"temperatura\":\"";
  text2 += 21;
  text2 += "\"},";    
  text2 += "{\"estadoArcondicionado\":\"";
  text2 += carro.GetEstadoArcondicionado();
  text2 += "\"}";   
  text2 += "]}";  
  Serial.println("Realizado GETALL: " + text2);
  server.send(200, "text/html", text2);
 
}


void setACCEstado(){
  int valor = server.arg("valor").toInt();
  
  if (valor == 1) {
    carro.ACCLiga();       
    server.send(200, "text/plane", carro.Nome + " ACC Ligado"); //Send web page
  }
  
  if (valor == 0) {    
    carro.ACCDesliga();
    server.send(200, "text/plane", carro.Nome + " ACC Desligado"); //Send web page
  }
  //if(valor == null ){
  //  server.send(200, "text/plane", "Nada foi feito"); //Send web page
  //  } 
     
}

void setFarois(){
  int valor = server.arg("valor").toInt();
  
  if (valor == 1) {       
    carro.LigaFarois();
    server.send(200, "text/plane", "Faróis Ligados"); //Send web page
  }
  
  if (valor == 0) {
    carro.DesligaFarois();
    server.send(200, "text/plane", "Faróis Desligados"); //Send web page
  }
  
  //if(valor == ""){
  //  server.send(200, "text/plane", "Nada foi feito"); //Send web page
  //  }   
     
}

void setRetrovisores(){
  int valor = server.arg("valor").toInt();
  
  if (valor == 1) {       
    carro.LigaRetrovisores();
    server.send(200, "text/plane", "Retrovisores Acionados"); //Send web page
  }
  
  if (valor == 0) {
    carro.DesligaRetrovisores();
    server.send(200, "text/plane", "Retrovisores Recolhidos"); //Send web page
  }
  
  //if(valor == ""){
  //  server.send(200, "text/plane", "Nada foi feito"); //Send web page
  //  }   
     
}

void setArcondicionado(){
  int valor = server.arg("valor").toInt();
  
  if (valor == 1) {       
    carro.LigaArcondicionado();
    server.send(200, "text/plane", "Ar condicionado Ligado"); //Send web page
  }
  
  if (valor == 0) {
    carro.DesligaArcondicionado();
    server.send(200, "text/plane", "Ar condicionadotrovisores Desligado"); //Send web page
  }
  
  //if(valor == ""){
  //  server.send(200, "text/plane", "Nada foi feito"); //Send web page
  //  }   
     
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  
  SPIFFS.begin();
  
  /* You can remove the password parameter if you want the AP to be open. */
  
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  
  server.on("/", handleRoot);
  server.onNotFound(handleWebRequests);
  server.on("/data", getData);
  server.on("/setACCEstado", setACCEstado);
  server.on("/setFarois", setFarois);
  server.on("/setRetrovisores", setRetrovisores);  
  server.on("/setArcondicionado", setArcondicionado);  
  server.begin();
  Serial.println("HTTP server started");
    
}

 
void loop() {
  server.handleClient();
}

bool loadFromSpiffs(String path){
  String dataType = "text/html";
  if(path.endsWith("/")) path += "index.htm";

  if(path.endsWith(".src")) path = path.substring(0, path.lastIndexOf("."));
  else if(path.endsWith(".html")) dataType = "text/html";
  else if(path.endsWith(".htm")) dataType = "text/html";
  else if(path.endsWith(".css")) dataType = "text/css";
  else if(path.endsWith(".js")) dataType = "application/javascript";
  else if(path.endsWith(".png")) dataType = "image/png";
  else if(path.endsWith(".gif")) dataType = "image/gif";
  else if(path.endsWith(".jpg")) dataType = "image/jpeg";
  else if(path.endsWith(".ico")) dataType = "image/x-icon";
  else if(path.endsWith(".xml")) dataType = "text/xml";
  else if(path.endsWith(".pdf")) dataType = "application/pdf";
  else if(path.endsWith(".zip")) dataType = "application/zip";
  File dataFile = SPIFFS.open(path.c_str(), "r");
  if (server.hasArg("download")) dataType = "application/octet-stream";
  if (server.streamFile(dataFile, dataType) != dataFile.size()) {
  }

  dataFile.close();
  return true;
}

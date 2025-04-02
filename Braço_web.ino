#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

const char* ssid = "Nome_rede";  // Substitua pelo nome da sua rede Wi-Fi
const char* password = "senha_rede";  // Substitua pela senha da sua rede Wi-Fi

ESP8266WebServer server(80);  // Cria um servidor web na porta 80

const int ledPin = 4;  // Define o pino do LED
Servo servos[5];  // Cria um array de objetos Servo
const int servoPins[5] = {2, 5, 12, 13, 15};  // Define os pinos dos Servos

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  for (int i = 0; i < 5; i++) {
    servos[i].attach(servoPins[i]);  // Conecta os servos aos pinos
    servos[i].write(90);  // Inicializa os servos na posição 90 graus
  }

  WiFi.begin(ssid, password);
  Serial.print("Conectando-se ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Conectado ao WiFi");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/led/on", handleLedOn);
  server.on("/led/off", handleLedOff);
  server.on("/servo", handleServo);  // Adiciona a rota para o controle dos servos
  server.begin();
  Serial.println("Servidor HTTP iniciado");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  String html = "<html><body><h1>Controle do LED e Servos</h1>"
                "<p><button onclick=\"sendRequest('/led/on')\">Ligar LED</button></p>"
                "<p><button onclick=\"sendRequest('/led/off')\">Desligar LED</button></p>";
  for (int i = 0; i < 5; i++) {
    html += "<p>Posição do Servo " + String(i+1) + ": <input type=\"range\" id=\"servoSlider" + String(i) + "\" min=\"0\" max=\"180\" oninput=\"updateServo(this.value, " + String(i) + ")\">"
            "<span id=\"servoValue" + String(i) + "\">90</span>°</p>";
  }
  html += "<script>"
          "function sendRequest(url) {"
          "  var xhr = new XMLHttpRequest();"
          "  xhr.open('GET', url, true);"
          "  xhr.send();"
          "}"
          "function updateServo(value, servoIndex) {"
          "  document.getElementById('servoValue' + servoIndex).innerText = value;"
          "  sendRequest('/servo?posicao=' + value + '&servo=' + servoIndex);"
          "}"
          "</script>"
          "</body></html>";
  server.send(200, "text/html", html);
}

void handleLedOn() {
  digitalWrite(ledPin, HIGH);
  server.send(200, "text/html", "");  // Resposta vazia para evitar recarregar a página
}

void handleLedOff() {
  digitalWrite(ledPin, LOW);
  server.send(200, "text/html", "");  // Resposta vazia para evitar recarregar a página
}

void handleServo() {
  if (server.hasArg("posicao") && server.hasArg("servo")) {
    int posicao = server.arg("posicao").toInt();
    int servoIndex = server.arg("servo").toInt();
    if (posicao >= 0 && posicao <= 180 && servoIndex >= 0 && servoIndex < 5) {
      servos[servoIndex].write(posicao);
      server.send(200, "text/html", "");  // Resposta vazia para evitar recarregar a página
    } else {
      server.send(400, "text/html", "Posição ou índice de servo inválido");
    }
  } else {
    server.send(400, "text/html", "Parâmetros 'posicao' ou 'servo' não encontrados");
  }
}

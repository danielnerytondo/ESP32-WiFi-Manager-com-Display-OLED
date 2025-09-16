#include <WiFiManager.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TinyGPSPlus.h>  // Nome correto da biblioteca
#include <HardwareSerial.h>

// DEFINIÇÕES DE PINOS
#define PIN_AP 23
#define LED_BUILTIN 2

// DEFINIÇÕES
#define APERTADO LOW

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C
#define OLED_RESET -1

// Objetos
Adafruit_SSD1306 tela(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
TinyGPSPlus gps;
HardwareSerial SerialGPS(2); // UART2 do ESP32

// DECLARAÇÃO DE VARIÁVEIS
char nomeWifi[15] = "ESP32_GPS";
char senhaWifi[15] = "12345678";

// Variáveis GPS
float latitude = 0;
float longitude = 0;
int satelites = 0;
bool gpsValido = false;

bool botaoPressionado() {
  if (digitalRead(PIN_AP) == APERTADO) {
    delay(50);
    if (digitalRead(PIN_AP) == APERTADO) {
      return true;
    }
  }
  return false;
}

void mostrarMensagem(String mensagem, int tamanho = 1) {
  tela.clearDisplay();
  tela.setTextSize(tamanho);
  tela.setTextColor(SSD1306_WHITE);
  tela.setCursor(0, 0);
  tela.println(mensagem);
  tela.display();
}

void atualizarGPS() {
  // Processar dados do GPS
  while (SerialGPS.available() > 0) {
    char c = SerialGPS.read();
    if (gps.encode(c)) {
      if (gps.location.isValid()) {
        latitude = gps.location.lat();
        longitude = gps.location.lng();
        satelites = gps.satellites.value();
        gpsValido = true;
        
        // Debug no Serial Monitor
        Serial.print("Lat: ");
        Serial.print(latitude, 6);
        Serial.print(" Lon: ");
        Serial.print(longitude, 6);
        Serial.print(" Sats: ");
        Serial.println(satelites);
      } else {
        gpsValido = false;
        Serial.println("GPS: Aguardando fixação...");
      }
    }
  }
}

void mostrarInfoCompleta() {
  tela.clearDisplay();
  tela.setTextSize(1);
  tela.setTextColor(SSD1306_WHITE);
  
  // Linha 1: Status WiFi
  tela.setCursor(0, 0);
  if (WiFi.status() == WL_CONNECTED) {
    tela.print("WiFi: ");
    tela.print(WiFi.SSID());
  } else {
    tela.print("Modo AP: ");
    tela.print(nomeWifi);
  }
  
  // Linha 2: IP Address
  tela.setCursor(0, 10);
  tela.print("IP: ");
  if (WiFi.status() == WL_CONNECTED) {
    tela.print(WiFi.localIP());
  } else {
    tela.print("192.168.4.1");
  }
  
  // Linha 3: Latitude
  tela.setCursor(0, 20);
  if (gpsValido) {
    tela.print("Lat: ");
    tela.print(latitude, 6);
  } else {
    tela.print("GPS: Buscando sats");
  }
  
  // Linha 4: Longitude
  tela.setCursor(0, 30);
  if (gpsValido) {
    tela.print("Lon: ");
    tela.print(longitude, 6);
  } else {
    tela.print("Sats: ");
    tela.print(satelites);
  }
  
  // Linha 5: Satélites
  tela.setCursor(0, 40);
  tela.print("Satelites: ");
  tela.print(satelites);
  
  // Linha 6: Status GPS
  tela.setCursor(0, 50);
  if (gpsValido) {
    tela.print("GPS: FIXO ");
    if (gps.time.isValid()) {
      tela.print("| ");
      if (gps.time.hour() < 10) tela.print("0");
      tela.print(gps.time.hour());
      tela.print(":");
      if (gps.time.minute() < 10) tela.print("0");
      tela.print(gps.time.minute());
    }
  } else {
    tela.print("GPS: BUSCANDO");
  }
  
  tela.display();
}

void setup() {
  pinMode(PIN_AP, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(115200);
  
  // Inicializar GPS - UART2 nos pinos 16(RX), 17(TX)
  SerialGPS.begin(9600, SERIAL_8N1, 16, 17);
  Serial.println("Inicializando GPS...");
  
  // Inicializar display
  if(!tela.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("Falha no display!");
    while(1);
  }
  
  mostrarMensagem("Iniciando GPS...", 1);
  delay(2000);

  // Verificar botão de reset
  if (botaoPressionado()) {
    mostrarMensagem("Reset WiFi?", 1);
    delay(2000);
    mostrarMensagem("3...", 2);
    delay(1000);
    mostrarMensagem("2...", 2);
    delay(1000);
    mostrarMensagem("1...", 2);
    delay(1000);
    
    WiFiManager wm;
    wm.resetSettings();
    
    mostrarMensagem("Reiniciando", 1);
    delay(2000);
    ESP.restart();
    while(1);
  }

  // Configurar WiFiManager
  WiFiManager wm;
  wm.setConfigPortalTimeout(180);

  mostrarMensagem("Conectando WiFi", 1);

  // Tentar conectar automaticamente
  bool res = wm.autoConnect(nomeWifi, senhaWifi);

  if (!res) {
    Serial.println("Modo AP ativo");
  }
  else {
    Serial.println("WiFi Conectado!");
  }
}

void loop() {
  atualizarGPS();
  mostrarInfoCompleta();
  
  // Piscar LED baseado no status
  if (gpsValido) {
    digitalWrite(LED_BUILTIN, millis() % 1000 < 100); // Pisca rápido com GPS válido
  } else {
    digitalWrite(LED_BUILTIN, millis() % 1000 < 500); // Pisca normal
  }
  
  delay(100);
}

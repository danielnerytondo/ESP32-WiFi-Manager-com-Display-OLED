#include <Arduino.h>
#include <WiFi.h>
#include <WiFiManager.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

// DEFINIÇÕES DE PINOS
#define PIN_AP 23
#define LED_BUILTIN 2  // LED interno do ESP32 (GPIO2)

// DEFINIÇÕES
#define APERTADO LOW

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C
#define OLED_RESET -1

Adafruit_SH1106G tela(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Variáveis para WiFiManager
char nomeWifi[40] = "ESP32_Config";
char senhaWifi[40] = "12345678";

void setupDisplay() {
  if (!tela.begin(SCREEN_ADDRESS, true)) {
    Serial.println("Display OLED não encontrado!");
    while (true);
  }
  tela.clearDisplay();
  tela.setTextSize(1);
  tela.setTextColor(SH110X_WHITE);
  tela.display();
}

void mostrarMensagem(String mensagem, int tamanhoTexto = 1) {
  tela.clearDisplay();
  tela.setTextSize(tamanhoTexto);
  tela.setCursor(0, 0);
  tela.println(mensagem);
  tela.display();
}

void configModoAP() {
  tela.clearDisplay();
  tela.setCursor(0, 0);
  tela.setTextSize(1);
  tela.println("Modo Configuracao");
  tela.println("Conecte-se ao:");
  tela.println(nomeWifi);
  tela.println("IP: 192.168.4.1");
  tela.display();
}

void configModoConectado() {
  tela.clearDisplay();
  tela.setCursor(0, 0);
  tela.setTextSize(1);
  tela.println("WiFi Conectado!");
  tela.print("IP: ");
  tela.println(WiFi.localIP());
  tela.display();
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_AP, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  // Inicializar display
  setupDisplay();
  mostrarMensagem("Iniciando...", 2);
  delay(2000);

  // WiFiManager
  WiFiManager wm;

  // Verificar se botão de reset está pressionado
  if (digitalRead(PIN_AP) == APERTADO) {
    mostrarMensagem("Resetando WiFi...", 1);
    Serial.println("Resetando configurações WiFi...");
    delay(3000);
    wm.resetSettings();
    mostrarMensagem("Reiniciando...", 1);
    delay(2000);
    ESP.restart();
  }

  // Configurar timeout do portal
  wm.setConfigPortalTimeout(180);

  mostrarMensagem("Conectando...", 1);

  // Tentar conectar automaticamente
  bool res;
  res = wm.autoConnect(nomeWifi, senhaWifi);

  if (!res) {
    Serial.println("Falha na conexão ou portal ativo");
    configModoAP();

    // Portal configuracao ativo
    while (true) {
      digitalWrite(LED_BUILTIN, millis() % 500 < 250); // LED piscando
      delay(10);
    }
  } else {
    // Conectado com sucesso
    Serial.println("Conectado!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());

    configModoConectado();
  }
}

void loop() {
  // Piscar LED para indicar funcionamento
  digitalWrite(LED_BUILTIN, millis() % 1000 < 500);
  delay(100);
}
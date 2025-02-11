#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Preferences.h>  // 🔹 Librería para guardar datos en la memoria
#include "frames.h"
#include <Adafruit_NeoPixel.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1 
#define TEMP_SENSOR_PIN A1
#define LED_PIN       20
#define BUTTON1_PIN   8
#define BUTTON2_PIN   3
#define NUMPIXELS     1

bool muerto = false;
bool basketball = false;

const char* ssid = "JOSECASA";
const char* password = "Elpatiodemicasa34";

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
WebServer server(80);
Preferences preferences;  // 🔹 Objeto para manejar la memoria
Adafruit_NeoPixel strip(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

String mascotaNombre = "Mascota";  // 🔹 Nombre predeterminado

float readTemperature() {
    int sensorValue = analogRead(TEMP_SENSOR_PIN);
    return (sensorValue / 4095.0) * 100; 
}

int foodBarWidth = 90; 
int foodDecrement = 1; 
unsigned long lastFoodUpdate = 0; 
const unsigned long foodUpdateInterval = 500; 
unsigned long lastLedToggle = 0;
const unsigned long ledToggleInterval = 1000; // Intervalo de cambio de color del LED

void showAnimation() {
    static uint8_t frameIndex = 0;
    display.clearDisplay();
    
    // 🔹 Dibujar el nombre de la mascota en la parte superior
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(mascotaNombre);

    // 🔹 Barra de comida (ajustar la posición para que no se superponga con el nombre)
    display.fillRect(0, 10, foodBarWidth, 5, SSD1306_WHITE);

    // 🔹 Animación de la mascota
    display.drawBitmap(0, 24, frames[frameIndex], 128, 40, SSD1306_WHITE);

    display.display();
    frameIndex = (frameIndex + 1) % FRAME_COUNT;
    if (basketball == false) {
        if (frameIndex == 18){
            frameIndex = 0;
        }
    }
    if (basketball == true) {
        if (frameIndex == 0){
            basketball = false;
        }
    }
    delay(100);
}

void updateFoodBar() {
    if (millis() - lastFoodUpdate >= foodUpdateInterval) {
        lastFoodUpdate = millis();
        if (foodBarWidth > 0) {
            foodBarWidth -= foodDecrement;
        } else {
            muerto = true;
        }
    }
}

void handleRevive() {
    foodBarWidth = 90; 
    muerto = false;
    server.send(200, "text/html", "<p>Mascota revivida. <a href='/'>Volver</a></p>");
}

void handleBasketball() {
    basketball = true;
    muerto = false;
    server.send(200, "text/html; charset=UTF-8", "<p>Mascota revivida. <a href='/'>Volver</a></p>");
}

// 🔹 Función para manejar la página web
void handleRoot() {
    String html = "<!DOCTYPE html><html><head><title>ESP32C3 Control</title></head><body>";
    html += "<h2>Control del LED RGB y Pantalla</h2>";
    html += "<p>Temperatura: " + String(readTemperature()) + " °C</p>";

    // 🔹 Campo para cambiar el nombre de la mascota
    html += "<form action='/set' method='GET'>";
    html += "<label>Nombre de la mascota: <input type='text' name='nombre' value='" + mascotaNombre + "'></label><br>";
    html += "<label>R: <input type='range' name='r' min='0' max='255'></label><br>";
    html += "<label>G: <input type='range' name='g' min='0' max='255'></label><br>";
    html += "<label>B: <input type='range' name='b' min='0' max='255'></label><br>";
    html += "<input type='submit' value='Actualizar'></form>";

    // 🔹 Botón de revivir
    html += "<form action='/revive' method='GET'>";
    html += "<input type='submit' value='Revivir Mascota' style='margin-top:20px; padding:10px; font-size:16px;'>";
    html += "</form>";

    // Boton de la animacion
    html += "<form action='/basketball' method='GET'>";
    html += "<input type='submit' value='🏀' style='margin-top:20px; padding:10px; font-size:16px;'>";
    html += "</form>";

    html += "</body></html>";
    server.send(200, "text/html; charset=UTF-8", html);
}

// 🔹 Función para manejar la configuración desde la web
void handleSet() {
    if (server.hasArg("r") && server.hasArg("g") && server.hasArg("b")) {
        int r = server.arg("r").toInt();
        int g = server.arg("g").toInt();
        int b = server.arg("b").toInt();
        strip.setPixelColor(0, strip.Color(r, g, b));
        strip.show();
    }

    // 🔹 Guardar el nombre en la memoria
    if (server.hasArg("nombre")) {
        mascotaNombre = server.arg("nombre");
        preferences.putString("nombreMascota", mascotaNombre);
    }

    handleRoot();
}

void setup() {
    Serial.begin(115200);
    pinMode(TEMP_SENSOR_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON1_PIN, INPUT_PULLUP);
    pinMode(BUTTON2_PIN, INPUT_PULLUP);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("No se encontró OLED"));
        while (1);
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 10);
    display.println("Conectando...");
    display.display();

    // Intentar conectarse a la red WiFi
    WiFi.begin(ssid, password);
    Serial.print("Conectando a WiFi...");
    unsigned long startAttemptTime = millis();

    // Esperar hasta 10 segundos para conectarse a la red WiFi
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
        Serial.print(".");
        delay(500);
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Conectado a WiFi");
        display.clearDisplay();
        display.setCursor(0, 10);
        display.println("IP: " + WiFi.localIP().toString());
        display.display();
        server.on("/", handleRoot);
        server.on("/set", handleSet);
        server.on("/revive", handleRevive);
        server.on("/basketball", handleBasketball);
        server.begin();
    } else {
        Serial.println("No se pudo conectar a WiFi, continuando sin funcionalidad web");
    }
    // 🔹 Iniciar memoria y recuperar el nombre de la mascota
    preferences.begin("gameData", false);
    mascotaNombre = preferences.getString("nombreMascota", "Mascota");
}

void loop() {
    if (digitalRead(BUTTON1_PIN) == LOW) {
        handleRevive();
    }
    if (WiFi.status() == WL_CONNECTED) {
        server.handleClient();
    }

    if (muerto) {
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 10);
        display.println("Tu mascota ha muerto");
        display.display();
        return;
    } else {
        showAnimation();
        updateFoodBar();
    }

    // Alternar el estado del LED RGB
    if (millis() - lastLedToggle >= ledToggleInterval) {
        lastLedToggle = millis();
        static int colorState = 0;
        colorState = (colorState + 1) % 3;
        switch (colorState) {
            case 0:
                strip.setPixelColor(0, strip.Color(255, 0, 0)); // Rojo
                break;
            case 1:
                strip.setPixelColor(0, strip.Color(0, 255, 0)); // Verde
                break;
            case 2:
                strip.setPixelColor(0, strip.Color(0, 0, 255)); // Azul
                break;
        }
        strip.show(); // Actualiza el LED
    }

    if (digitalRead(BUTTON2_PIN) == LOW) {
        handleBasketball();
    }
}

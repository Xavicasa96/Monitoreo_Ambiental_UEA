#include <Arduino.h>
#include <Wire.h>                  
#include <LiquidCrystal_I2C.h>     
#include <DHT.h>                   

#define DHTPIN 2                   
#define DHTTYPE DHT11              
#define LDRPIN A0                  

#define LED_VERDE 3                
#define LED_ROJO  4                
#define BUZZER    5                

const float TEMP_ADVERTENCIA = 27.0; 
const float TEMP_ALARMA      = 30.0; 
const float HUM_ADVERTENCIA  = 70.0; 
const float HUM_ALARMA       = 80.0; 

enum EstadoSistema {
  ESTADO_NORMAL,
  ESTADO_ADVERTENCIA,
  ESTADO_ALARMA
};

EstadoSistema estadoActual = ESTADO_NORMAL;

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2); 

void setup() {
  Serial.begin(9600);
  Serial.println(F("=================================================="));
  Serial.println(F(" UEA - SISTEMA DE MONITOREO AMBIENTAL (VS CODE)  "));
  Serial.println(F("=================================================="));

  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_ROJO, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_ROJO, LOW);
  digitalWrite(BUZZER, LOW);

  dht.begin();
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("SISTEMA AMBIENTAL");
  lcd.setCursor(0, 1);
  lcd.print("INICIALIZANDO...");
  delay(2000);
  lcd.clear();
}

void loop() {
  float humedad = dht.readHumidity();
  float temperatura = dht.readTemperature();
  int valorLDR = analogRead(LDRPIN); 
  int porcentajeLuz = map(valorLDR, 0, 1023, 0, 100); 

  if (isnan(humedad) || isnan(temperatura)) {
    Serial.println(F("ERROR: Fallo al leer datos del sensor DHT."));
    lcd.setCursor(0, 0);
    lcd.print("ERROR EN SENSOR ");
    lcd.setCursor(0, 1);
    lcd.print("VERIFICAR CABLES");
    delay(2000);
    return;
  }

  if (temperatura >= TEMP_ALARMA || humedad >= HUM_ALARMA) {
    estadoActual = ESTADO_ALARMA;
  } else if (temperatura >= TEMP_ADVERTENCIA || humedad >= HUM_ADVERTENCIA) {
    estadoActual = ESTADO_ADVERTENCIA;
  } else {
    estadoActual = ESTADO_NORMAL;
  }

  switch (estadoActual) {
    case ESTADO_NORMAL:
      digitalWrite(LED_VERDE, HIGH);
      digitalWrite(LED_ROJO, LOW);
      digitalWrite(BUZZER, LOW);
      
      lcd.setCursor(0, 0);
      lcd.print("T:"); lcd.print(temperatura, 1); lcd.print((char)223); lcd.print("C ");
      lcd.print("H:"); lcd.print(humedad, 1); lcd.print("% ");
      lcd.setCursor(0, 1);
      lcd.print("Luz:"); lcd.print(porcentajeLuz); lcd.print("% EST:OK  ");
      break;

    case ESTADO_ADVERTENCIA:
      digitalWrite(LED_VERDE, !digitalRead(LED_VERDE)); 
      digitalWrite(LED_ROJO, LOW);
      digitalWrite(BUZZER, LOW);

      lcd.setCursor(0, 0);
      lcd.print("T:"); lcd.print(temperatura, 1); lcd.print("C ");
      lcd.print("H:"); lcd.print(humedad, 1); lcd.print("%");
      lcd.setCursor(0, 1);
      lcd.print("EST: ADVERTENCIA");
      break;

    case ESTADO_ALARMA:
      digitalWrite(LED_VERDE, LOW);
      digitalWrite(LED_ROJO, HIGH);
      tone(BUZZER, 1000, 200); 

      lcd.setCursor(0, 0);
      lcd.print("!ALERTA CRITICA!");
      lcd.setCursor(0, 1);
      lcd.print("T:"); lcd.print(temperatura, 1); lcd.print("C H:"); lcd.print(humedad, 1); lcd.print("%");
      break;
  }

  Serial.print(F("Temperatura: ")); Serial.print(temperatura, 1); Serial.print(F(" °C | "));
  Serial.print(F("Humedad: "));     Serial.print(humedad, 1);     Serial.print(F(" % | "));
  Serial.print(F("Luminosidad: ")); Serial.print(valorLDR);       Serial.print(F(" ("));   
  Serial.print(porcentajeLuz);       Serial.println(F("% )"));

  Serial.print(F("Estado del Sistema: "));
  switch (estadoActual) {
    case ESTADO_NORMAL:      Serial.println(F("Monitoreo (Normal)")); break;
    case ESTADO_ADVERTENCIA: Serial.println(F("ADVERTENCIA")); break;
    case ESTADO_ALARMA:      Serial.println(F("ALARMA CRITICA")); break;
  }
  Serial.println(F("--------------------------------------------------"));

  delay(1500);
}
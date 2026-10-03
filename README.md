# 🌡️ Sistema Inteligente de Monitoreo Ambiental

**Universidad Estatal Amazónica**  
**Carrera:** Ingeniería en Tecnologías de la Información  
**Asignatura:** Sistemas Digitales (UEA-L-UFPTI-002)  
**Entorno de Desarrollo:** Visual Studio Code + PlatformIO IDE / Wokwi  
**Lenguaje:** C++ (Arduino Framework)

---

## 📋 Descripción del Proyecto

Este sistema realiza el monitoreo en tiempo real de variables ambientales (**Temperatura**, **Humedad Relativa** y **Luminosidad**) utilizando un microcontrolador **Arduino UNO** (ATmega328P). El control operacional se basa en una **Máquina de Estados Finitos (FSM) tipo Moore**, garantizando transiciones de estado deterministas y la activación segura de actuadores de señalización y alerta.

---

## 🚀 Simulación Interactiva en Tiempo Real

Puedes probar y manipular las variables del circuito simulado (ajustar temperatura, humedad e intensidad de luz) directamente desde tu navegador:

👉 **[Ejecutar Simulación en Wokwi](https://wokwi.com/projects/476903296096645121)**

---

## 🛠️️ Arquitectura de Hardware y Diagrama de Conexiones

| Componente | Pin del Componente | Pin en Arduino UNO | Descripción / Protocolo |
| :--- | :--- | :--- | :--- |
| **Pantalla LCD 16x2** | SDA / SCL | **A4** / **A5** | Bus Serial Síncrono I2C (Dirección `0x27`) |
| **Sensor DHT22 / DHT11** | DATA / SDA | Digital **D2** | Interfaz Digital Monobús (*One-Wire*) |
| **Sensor LDR (Fotocelda)** | Salida AO | Analógico **A0** | Canal de Conversión ADC (0 - 1023) |
| **LED Verde** | Ánodo (+) | Digital **D3** | Indicador de Estado Normal (vía 220 Ω) |
| **LED Rojo** | Ánodo (+) | Digital **D4** | Indicador de Estado de Alarma (vía 220 Ω) |
| **Buzzer Activo** | Positivo (+) | Digital **D5** | Alerta Sonora Frecuencia 1 kHz |

---

## ⚙️ Lógica de Control (Máquina de Estados Finitos - FSM)

El firmware evalúa las lecturas analógicas y digitales de forma continua y clasifica el estado del entorno en tres niveles operacionales:

* **ESTADO_NORMAL:** Temp < 27.0 °C y Hum < 70.0%  
  *Salida:* LED Verde Encendido, LED Rojo Apagado, Buzzer Apagado.
* **ESTADO_ADVERTENCIA:** 27.0 °C ≤ Temp < 30.0 °C o 70.0% ≤ Hum < 80.0%  
  *Salida:* LED Verde Parpadeante, LED Rojo Apagado, Buzzer Apagado.
* **ESTADO_ALARMA (Crítico):** Temp ≥ 30.0 °C o Hum ≥ 80.0%  
  *Salida:* LED Verde Apagado, LED Rojo Encendido, Buzzer Activo (1 kHz).

---

## 📁 Estructura del Repositorio

```text
Monitoreo_Ambiental_UEA/
├── .pio/                 # Archivos de compilación de PlatformIO (excluido en git)
├── include/              # Cabeceras y definiciones del proyecto
├── lib/                  # Librerías privadas
├── src/
│   └── main.cpp          # Código fuente principal en C++ (FSM y Drivers)
├── platformio.ini        # Archivo de configuración de dependencias y target
├── diagram.json          # Esquema y mapeo de hardware para Wokwi
└── README.md             # Documentación técnica del proyecto

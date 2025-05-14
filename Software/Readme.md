# Proyecto de IoT: Monitoreo de Salud de Cerdas

## Tecnologías Utilizadas
- **Lenguajes de Programación**:
  - Arduino: Para el desarrollo en Heltec LoRa 32 y Lilygo SIMXXX.
  
- **Frameworks**:
  - Node-RED: Para la gestión y visualización de datos.
  
- **Bibliotecas**:
  - MQTT.js: Para la comunicación MQTT en Node-RED.
  - Adafruit Sensor Library: Para la integración de sensores en el microcontrolador.
  
- **Herramientas**:
  - InfluxDB: Para el almacenamiento de datos históricos.
  - Grafana: Para la visualización de datos almacenados en InfluxDB.

## Funcionalidades
### Descripción Detallada
- **Monitoreo de Datos en Tiempo Real**: Recepción continua de datos de sensores, actualizando la información en el dashboard.
- **Visualización de Datos**: Dashboard interactivo en Node-RED con gráficos y alertas visuales.
- **Almacenamiento de Datos**: Uso de InfluxDB para guardar datos históricos y facilitar análisis a largo plazo.
- **Generación de Reportes**: Herramientas para crear reportes sobre la salud de los cerdos y análisis de tendencias.
- **Integración con Hardware**: Conexión directa con sensores y microcontroladores para recolección automática de datos.
- **Interacción con Otros Sistemas**: Comunicación mediante MQTT y acceso remoto al dashboard.

## Estructura del Código
### Descripción de la Estructura del Código Fuente
- **Heltec LoRa 32**:
  - **Módulo Sensor**: Código para la recolección de datos de temperatura y ritmo cardíaco.
  - **Módulo MQTT**: Publica datos en el broker MQTT.

- **Raspberry Pi**:
  - **Script de Node-RED**: Flujos que gestionan la recepción de datos y su visualización.
  - **Configuración de InfluxDB**: Scripts para crear bases de datos y gestionar datos.

- **PC**:
  - **Dashboard en Node-RED**: Código para la interfaz de usuario y visualización de datos.

- **Lilygo SIMXXX**:
  - **Módulo de Comunicación**: Código para la conexión a redes móviles y envío de datos via MQTT.

### Scripts de Bases de Datos, MQTT, Grafana y Zabbix
- **InfluxDB**: Scripts para la creación de la base de datos y la configuración de retención de datos.
- **MQTT**: Configuraciones del broker para gestionar la comunicación entre dispositivos.
- **Grafana**: Configuración de paneles para visualizar datos de InfluxDB.
- **Zabbix**: Scripts de monitorización para el estado del sistema.

## Instrucciones de Instalación
### Pasos Detallados
1. **Heltec LoRa 32**:
   - Instalar el IDE de Arduino.
   - Configurar el entorno con las bibliotecas necesarias (Adafruit Sensor Library, etc.).
   - Cargar el código del módulo sensor y MQTT.

2. **Raspberry Pi**:
   - Instalar Node.js y Node-RED:
     ```bash
     sudo apt update
     sudo apt install nodejs npm
     npm install -g --unsafe-perm node-red
     ```
   - Instalar InfluxDB:
     ```bash
     sudo apt install influxdb
     ```
   - Configurar Node-RED y cargar los flujos necesarios.

3. **Modem WiFi (Lilygo SIMXXX)**:
   - Configurar el modem para la conexión a la red móvil.
   - Cargar el código para la comunicación MQTT.

4. **Configuración de Grafana**:
   - Instalar Grafana y conectarlo a InfluxDB.
   - Crear paneles de visualización.

### Dependencias y Requisitos Especiales
- Asegúrate de tener todas las bibliotecas y dependencias instaladas en cada entorno.
- Verifica la conectividad de red para MQTT y el acceso a la base de datos.
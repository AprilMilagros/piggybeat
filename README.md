# PIGGYBEAT

# Proyecto de IoT: Monitoreo de Salud de Cerdas Gestantes

## Descripción del Proyecto
El proyecto consiste en un sistema de monitoreo de la salud de los cerdos gestantes mediante el uso de tecnologías IoT. Su propósito es mejorar la identificación de complicaciones durante el parto, optimizando así la salud animal y reduciendo pérdidas económicas en la ganadería.

### Propósito
- **Monitoreo Continuo**: Recoger datos en tiempo real sobre la temperatura, ritmo cardíaco y condiciones ambientales.
- **Detección Temprana**: Identificar complicaciones durante el parto antes de que se conviertan en problemas graves.

### Alcance
- Implementación en granjas porcinas para el seguimiento de cerdos gestantes.
- Visualización de datos a través de un dashboard accesible desde dispositivos móviles y computadoras.

### Tecnologías Utilizadas
- **Sensores**: Sensores de temperatura, ritmo cardíaco y condiciones ambientales.
- **Microcontroladores**: Placas como Heltec LoRa 32 y ESP32 para la recolección de datos.
- **Comunicación**: Protocolos MQTT para la transmisión de datos.
- **Backend**: Node-RED para la gestión y visualización de datos.
- **Base de Datos**: InfluxDB para el almacenamiento de información.

## Diagrama de Topología Lógica
A continuación, se describe cómo debería estructurarse el diagrama:

1. **Sensores** (Temperatura, Ritmo Cardíaco, Condiciones Ambientales)
   - Recogen datos y envían información a la placa de microcontrolador.

2. **Microcontrolador** (Heltec LoRa 32 / ESP32)
   - Procesa los datos de los sensores.
   - Publica los datos en un broker MQTT.

3. **Broker MQTT**
   - Recibe datos de múltiples microcontroladores.
   - Distribuye los datos a los suscriptores (Node-RED y otros).

4. **Node-RED**
   - Recibe datos del broker MQTT.
   - Procesa y visualiza los datos en un dashboard.

5. **Base de Datos (InfluxDB)**
   - Almacena datos históricos para análisis futuros.

6. **Interfaz de Usuario**
   - Dashboard accesible a través de un navegador web para visualización de datos en tiempo real.

### Flujo de Información
- **Desde Sensores a Microcontrolador**: Los sensores envían datos al microcontrolador.
- **Desde Microcontrolador a Broker MQTT**: El microcontrolador publica los datos en el broker.
- **Desde Broker MQTT a Node-RED**: Node-RED suscribe a los datos del broker.
- **Desde Node-RED a Base de Datos**: Node-RED almacena datos en InfluxDB.
- **Desde Node-RED a Interfaz de Usuario**: Visualización de datos en tiempo real en el dashboard.
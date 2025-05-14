## Configuración de Node-RED

1. **Instalación de Node-RED**:
   - Asegúrate de tener Node.js instalado. Puedes descargarlo desde [nodejs.org].
   - Instala Node-RED ejecutando el siguiente comando en la terminal:
     ```bash
     npm install -g node-red
     ```

2. **Ejecutar Node-RED**:
   - Inicia Node-RED con el siguiente comando:
     ```bash
     node-red
     ```
   - Accede a la interfaz de usuario en tu navegador en `http://localhost:1880`.

3. **Configuración del MQTT**:
   - Asegúrate de que el broker MQTT (por ejemplo, `broker.emqx.io`) esté accesible.
   - Configura el nodo MQTT en Node-RED con el tema correspondiente (`nicamex/porcino`) para recibir datos de las placas.

4. **Base de Datos InfluxDB**:
   - Asegúrate de que InfluxDB esté instalado y corriendo en la dirección especificada (`192.168.137.67`).
   - Crea la base de datos `proyecto_nicamex` en InfluxDB, si aún no existe.

5. **Flujos de Node-RED**:
   - Importa el archivo `node-red.json` en la interfaz de Node-RED.
   - Conecta los nodos según sea necesario para recibir y visualizar los datos.
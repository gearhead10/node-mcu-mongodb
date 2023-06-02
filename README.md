# node-mcu-mongodb
Alpha proyect v0.1.2

## DESCRIPCIÓN ##
-- Proyecto para conectar broker + NodeMCU ESP8266 WiFi para envío de datos obtenidos desde sensores 
de humedad y temperatura hacia una base de datos NoSQL MongoDB utilizando clusteres y datos en JSON.

## DESCRIPTION ##

-- Project to connect broker + NodeMCU ESP8266 WiFi to send data obtained from humidity and temperature sensors 
to a NoSQL MongoDB database using clusters and JSON data.


## FUNCIONAMIENTO ##

- Instalar librerías de Arduino.
- Llenar los campos marcados de credenciales y conexiones.
- Instalar dependecias de NodeJs para correr el servicio del servidor de Express.
- La información se muestra en el "Index.ejs" del directorio "Views".


#### ## logs ## #####

1. 02/06/2023

--SPANISH--
✓ Conexión a Broker correcta.
✓ Conexión del ESP8266 a la red WiFi establecida.
✓ Envío de datos de sensores hacia Broker correcta.
✓ Creación de doc JSON con los datos de cada sensor.

--ENGLISH--
✓ Correct Broker connection.
✓ Connection of the ESP8266 to the established WiFi network.
✓ Sending sensor data to Broker correct.
✓ Creation of JSON doc with the data of each sensor.

-- TO BE FIXED --

- Conexión HTTPs hacia el cluster de MongoDB Atlas. | HTTPs connection to the MongoDB Atlas cluster.
- Actualización en tiempo real de los datos obtenidos desde el JSON del cluster hacia NodeJs. | - Real-time update of the data obtained from the
  cluster JSON to NodeJs.

const { ObjectId } = require('bson');
const express = require('express');
const mongoose = require('mongoose');
const path = require('path');

const app = express();
app.set('view engine', 'ejs');


// Conexión a MongoDB Atlas
const user = 'username';
const password = 'test123';
const dbName = 'mongodb-name';
const dbURI = "mongodb+srv://<username>:<password>@xxx-cluster.xxx.mongodb.net/xxx";

mongoose.connect(dbURI, { useNewUrlParser: true, useUnifiedTopology: true })
  .then(() => {
    console.log('Conexión exitosa a MongoDB Atlas');
  })
  .catch(error => {
    console.error('Error de conexión a MongoDB Atlas:', error);
  });


// Definicion del esquema y el modelo
const Schema = mongoose.Schema;
const datosSchema = new Schema({
  //_id: ObjectId,  
  humedad: Number,
  temp: Number
});
const Datos = mongoose.model('Datos', datosSchema, 'temp_hum');




// Ruta para obtener los datos de la coleccion
app.get('/', async (req, res) => {
  try {
    const datos = await Datos.find().lean();
    
    //res.setHeader('Content-Type', 'application/javascript');
    //res.sendFile(path.join(__dirname, 'index.html'), { datos: JSON.stringify(datos) });
    //res.render('index', {datos: datos});
    
    res.render('index', { datos: datos });

    console.log(datos)
  } catch (error) {
    console.error('Error al obtener los datos: ', error);
    res.status(500).send('Error al obtener los datos.');
  }
});

app.listen(3000, () => {
  console.log('Servidor Express en ejecución en el puerto 3000');
});

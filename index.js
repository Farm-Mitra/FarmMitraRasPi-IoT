var  express = require('express');
var app = express();
var path = require('path')
var mqtt = require('mqtt');
var http = require('http').Server(app);
var io = require('socket.io')(http);

var mitraPath = "10.244.25.18";
//var mitraPath = "10.244.25.68";


var mqttClient = mqtt.connect('mqtt://'+mitraPath+':1883'); 
console.log('mqtt://'+mitraPath+':1883');
app.get('/',function(req,res){
    res.sendFile(path.join(__dirname, '../FarmMitraRasPi-IoT', 'index.html'));
})

var soilMoisture = 20;
var soliMoistTopic = "farmer/node1/solimoist";

// action for listen on Soil Moisture
console.log('Tryin to connect');
mqttClient.on('connect',function(){
    console.log('connected to Pi');
    mqttClient.subscribe(soliMoistTopic)
    
})


// actions for water flow control

io.on('connection', function(socket){
   console.log('connected to FrontEnd');
    mqttClient.on('message',function(topic,message){
        console.log('Moisture Data:'+message.toString());
        io.emit('soilMoisture',message.toString());
    })
    
  
});

http.listen(3000, function(){
  console.log('listening on *:3000');
});
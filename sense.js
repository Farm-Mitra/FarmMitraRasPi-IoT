var mqtt    = require('mqtt');
var client  = mqtt.connect('mqtt://10.244.25.18');
 
client.on('connect', function () {
  console.log('Connection Successfull');
  client.subscribe('farmer/node2/humidity');
  client.subscribe('farmer/node2/temperature');
  client.subscribe('farmer/node1/soilmoist');
  client.publish('farmer/node1/valve',"0");
  
  //client.publish('presence', 'Hello mqtt');
});
 var flagMotorRun = 0;
client.on('message', function (topic, message) {
  // message is Buffer
    
    //switch(topic.splice.pop())

switch(topic.split('/').pop()){
    case 'humidity':
        break;
    case 'temperature':
        break;
    case 'soilmoist':
        console.log("Soil Moisutre: "+message);
        console.log("flagMotorRun: "+flagMotorRun);
        
        if(parseInt(message) < 38 && flagMotorRun == 0){
            flagMotorRun = 1;
            client.publish('farmer/node1/valve',"1");
        } else if(parseInt(message) >= 39 && flagMotorRun == 1){
            flagMotorRun = 0;
            client.publish('farmer/node1/valve',"0");
        }            
        break;
        
}
    

  console.log(topic+":"+message.toString());
  //client.end();
});


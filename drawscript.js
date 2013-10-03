//connect to the socket
var ws = new WebSocket("ws://<IP ADRESS>/websocket");
var heart_image = new Image();
//I hope they don't mind
heart_image.src = 'http://cdn.ttgtmedia.com/ITKE/uploads/blogs.dir/13/files/2009/01/pixelheart3.jpg';
 
var x_counter = 0;
var heart_beat_point = 0;
var connected = 0;
 
var myVar=setInterval(function(){check_coonection()},500);
 
function check_coonection()//this functions runs twice a second to check if we are still connected
{
    connected = connected - 1;
    if(connected < 1){
        document.getElementById("data").innerHTML = "ERROR, COULD NOT CONNECT. Either the server is down, or you are blocking port 8888";
        connected = 0;
    }
}
 
ws.onmessage = function(evt)//event on message from websocket server
{
    connected = 2;
 
    var raw_data = evt.data;
    document.getElementById("data").innerHTML = raw_data;
 
    var splitted_data = raw_data.split(",");//splits the data from the uC, look into the python server, to see how the data is formatted.
 
    //check for heart beat
    if(splitted_data[3] == 1)
        heart_beat_point = 40;
    else
        heart_beat_point = heart_beat_point - 1;
 
    //drawing the graph on canvas:
    var c=document.getElementById("graph");
    var ctx=c.getContext("2d");
    ctx.fillStyle="#FF0000";
    if(splitted_data[2] < 100 || splitted_data[2] > 250){
        ctx.clearRect(0,0,800,200);
        ctx.font="35px Arial";
        ctx.fillText("No finger connected",150,75);
    }
    else{
        ctx.fillRect(x_counter,(splitted_data[2])*(5)+(-800),1,1);
        ctx.clearRect(x_counter+2,0,1,500);//clean the area in front 
        ctx.clearRect(0,0,150,90);//clear heart area (and text area)
        ctx.strokeRect(0,0,151,91)
 
        //draw the hart - if just after a beat - make it bigger
        if(heart_beat_point > 0)
            ctx.drawImage(heart_image, 1, 1, 85, 85);
        else
            ctx.drawImage(heart_image, 5, 5, 70, 70);
 
        var bps = splitted_data[5].toString();
        ctx.font="30px Arial";
        ctx.fillText(bps,85,60);     

	x_counter = x_counter + 1;//move on the x axsis
 
        if(x_counter >= 800){//move pointer back
            x_counter = 0;  
            ctx.clearRect(0,0,1,500);
        }
    }
 
}

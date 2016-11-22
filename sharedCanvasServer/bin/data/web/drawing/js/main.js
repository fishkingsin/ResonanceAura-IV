var socket;

var messageDiv;
var statusDiv;

// drawing vars
var canvas;
var ctx = null;
var color 	 = {};
var id 		 = -1;
var sketches = {};
var wOffset,hOffset,drawSPt=0, drawSPtX=0, drawSPtY=0;
var strokeW = 1;
var mode = 1;
var lastIndex = 0;
var dMode = 1;
var drawIndex = 0;
//line
var lineArray = [];
function Line() {
	this.x = 0;
	this.y = 0;
	this.x2 = 0;
	this.y2 = 0; 
}

function addLine(x, y) {
	var l = new Line;
	l.x = x;
	l.y = y;
	l.x2 = x;
	l.y2 = y;
	lineArray.push(l);
}
//circle
var cirArray = [];
var circleRX = 0, circleRY = 0;
function Cir() {
	this.x = 0;
	this.y = 0;
	this.r = 10; 
}

function addCir(x, y) {
	var cir = new Cir;
	cir.x = x;
	cir.y = y;
	cirArray.push(cir);
}
//triangle
var triArray = [];
function Tri() {
	this.x = 0;
	this.y = 0;
	this.w = 10; 
	this.h = 10;
	this.a = 0;
}

function addTri(x, y) {
	var tri = new Tri;
	tri.x = x;
	tri.y = y;
	triArray.push(tri);
}

//rect
var sqArray = [];
function Sq() {
	this.x = 0;
	this.y = 0;
	this.w = 10; 
	this.h = 10;
	this.a = 0;
}

function addSq(x, y) {
	var sq = new Sq;
	sq.x = x;
	sq.y = y;
	sqArray.push(sq);
}

//hex
var hexArray = [];
function Hex() {
	this.x = 0;
	this.y = 0;
	this.w = 10; 
	this.h = 10;
	this.a = 0;
}

function addHex(x, y) {
	var hex = new Hex;
	hex.x = x;
	hex.y = y;
	hexArray.push(hex);
}

$(window).load(function() {
	socket = setupSocket();
	
	// document.getElementById("brow").textContent = " " + BrowserDetect.browser + " "
	// + BrowserDetect.version +" " + BrowserDetect.OS +" ";

	// messageDiv 	= document.getElementById("messages");
	// statusDiv	= document.getElementById("status");

	canvas 		= document.getElementById("sketchCanvas");
	if (canvas.getContext) {
		canvas.onmousedown 	= onMouseDown;
		canvas.onmouseup 	= onMouseUp;
		canvas.onmousemove 	= onMouseMoved;
		canvas.addEventListener("touchstart", onMouseDown, false);
		canvas.addEventListener("touchend", onMouseUp, false);
		canvas.addEventListener("touchmove", onMouseMoved, false);
		ctx			= canvas.getContext('2d');
		canvas.width  = window.innerWidth-150;
		canvas.height = window.innerHeight;
		// canvas.width  = 450;
		// canvas.height = 450;
		//get the screen resize offset
		wOffset = Math.floor(canvas.width/360);
		hOffset = Math.floor(canvas.height/144);
		window.addEventListener("resize", onresize);

		// canvas.addEventListener('touchmove', function(e) {
		//   e.preventDefault();
		// 	onMouseDown(e);
		// },false);
		//target the entire page, and listen for touch events
		$('html, body').on('touchstart touchmove', function(e){ 
		     //prevent native touch activity like scrolling
		     e.preventDefault(); 
		 });
		
	} else {
		alert("Sorry, your browser doesn't support canvas!");
	}

	$("#drawIcon").on("mouseup touchend", changeDraw);
	$("#circleIcon").on("mouseup touchend", changeCir);
	$("#triIcon").on("mouseup touchend", changeTri);
	$("#sqIcon").on("mouseup touchend", changeSq);
	$("#hexIcon").on("mouseup touchend", changeHex);
	$("#okIcon").on("mouseup touchend", saveCanvas);
	//ledSpeed.onFinishChange(function(value) {
  		// Fires when a controller loses focus.
  		
	//});

	//strokeW.onFinishChange(function(value) {
  		// Fires when a controller loses focus.
  	//	strokeW = parseInt(value);
	//});

	// circleUsing.onFinishChange(function(value) {
 //  		eraseAll();
	// });

	// circleR.onFinishChange(function(value) {
 //  		// Fires when a controller loses focus.
 //  		alert("The new value is " + value);
	// });



});



function onresize(){
	canvas 		= document.getElementById("sketchCanvas");
	
	canvas.width  = window.innerWidth;
	canvas.height = window.innerHeight;
	wOffset = Math.floor(canvas.width/360);
	hOffset = Math.floor(canvas.height/144);
}
// send value from text input
function sendMessageForm(){
	socket.send(message.value);
	message.value = "";
}

function changeDraw(e) {
	//dMode = 0;
	cirArray=[];
	hexArray=[];
	sqArray=[];
	triArray=[];
}

function changeCir(e) {
	dMode = 1;	
}

function changeTri(e) {
	dMode = 2;
}

function changeSq(e) {
	dMode = 3;
}

function changeHex(e) {
	dMode = 4;
}

var bMouseDown = false;
function onMouseDown( e ){
	mouseX = (e.changedTouches ? e.changedTouches[0].pageX : e.pageX);
	mouseY = (e.changedTouches ? e.changedTouches[0].pageY : e.pageY);
	
	
	//get the new draw point
	//
	//
	//if(!bMouseDown) {
		bMouseDown=true;
		if(dMode == 0) {
			
			addLine(mouseX, mouseY);
		} else if(dMode == 1) {

			addCir(mouseX, mouseY);
			drawIndex = cirArray.length-1;
		} else if(dMode == 2) {
			addTri(mouseX, mouseY);
		}  else if(dMode == 3) {
			addSq(mouseX, mouseY);
		}  else if(dMode == 4) {
			addHex(mouseX, mouseY);
		}
	//}
	renderCanvas();
	
}

function onMouseMoved( e ){
	mouseX = (e.changedTouches ? e.changedTouches[0].pageX : e.pageX);
	mouseY = (e.changedTouches ? e.changedTouches[0].pageY : e.pageY);
	if ( bMouseDown ){//for mouse drag draw function
		if(dMode == 0) {
			//onMouseDraw( mouseX, mouseY );
			var index = lineArray.length-1;
			lineArray[index].x2 = mouseX;
			lineArray[index].y2 = mouseY;
		} else if(dMode == 1) {
			var index = cirArray.length-1;
			cirArray[index].r = 10 + (Math.abs(mouseX-cirArray[index].x)>Math.abs(mouseY-cirArray[index].y)?Math.abs(mouseX-cirArray[index].x):Math.abs(mouseY-cirArray[index].y));
			addCir(cirArray[drawIndex].x, cirArray[drawIndex].y);
		} else if(dMode == 2) {
			var index = triArray.length-1;
			triArray[index].w = 10 + (Math.abs(mouseX-triArray[index].x)>Math.abs(mouseY-triArray[index].y)?Math.abs(mouseX-triArray[index].x):Math.abs(mouseY-triArray[index].y));
			triArray[index].h = Math.sqrt(Math.pow(triArray[index].w,2)-Math.pow(triArray[index].w*.5, 2));
			triArray[index].a = Math.atan2(mouseY-triArray[index].y, mouseX-triArray[index].x);
			triArray[index].a = triArray[index].a*180/Math.PI+90;
		}  else if(dMode == 3) {
			var index = sqArray.length-1;
			sqArray[index].w = 10 + (Math.abs(mouseX-sqArray[index].x)>Math.abs(mouseY-sqArray[index].y)?Math.abs(mouseX-sqArray[index].x):Math.abs(mouseY-sqArray[index].y));
			sqArray[index].a = Math.atan2(mouseY-sqArray[index].y, mouseX-sqArray[index].x);
			sqArray[index].a = sqArray[index].a*180/Math.PI+90;
		}  else if(dMode == 4) {
			var index = hexArray.length-1;
			hexArray[index].w = 10 + (Math.abs(mouseX-hexArray[index].x)>Math.abs(mouseY-hexArray[index].y)?Math.abs(mouseX-hexArray[index].x):Math.abs(mouseY-hexArray[index].y));
			hexArray[index].a = Math.atan2(mouseY-hexArray[index].y, mouseX-hexArray[index].x);
			hexArray[index].a = hexArray[index].a*180/Math.PI+90;
		}
	}
	renderCanvas();
}

function onMouseUp( e ){
	mouseX = (e.changedTouches ? e.changedTouches[0].pageX : e.pageX);
	mouseY = (e.changedTouches ? e.changedTouches[0].pageY : e.pageY);
	bMouseDown = false;
	//onMouseDraw(-1, -1);
	if(dMode == 0) {
	} else if(dMode == 1) {
		var index = cirArray.length-1;
		cirArray[index].r = 10 + (Math.abs(mouseX-cirArray[index].x)>Math.abs(mouseY-cirArray[index].y)?Math.abs(mouseX-cirArray[index].x):Math.abs(mouseY-cirArray[index].y));
	}
	//renderCanvas();
}

// catch mouse events on canvas
function onMouseDraw( x, y ){
	//rescale the point for server
	// x =Math.floor(x/wOffset);
	// y=Math.floor(y/hOffset);
	// var sw = parseInt(strokeW);
	// console.log(x+"::"+y);

	// var point = {point:{x:x,y:y},id:id,color:color,sw:sw};
	// if ( socket.readyState == 1 ){
	// 		// sketches[id].points.push( point.point );

	// 		// if ( sketches[id].points.length > 500 ){
	// 		// 	//sketches[id].points.shift();
	// 		// }
	// 		//socket.send(JSON.stringify(point));
	// 		renderCanvas();
	// 	}
	}

//save the start draw point
function saveDrawSpt(x,y) {
	x =-Math.floor(x/wOffset);
	y=-Math.floor(y/hOffset);
	sw = parseInt(strokeW);
	var point = {point:{x:x,y:y},id:id,color:color,sw:sw};
	if ( socket.readyState == 1 ){
		sketches[id].points.push( point.point );
		drawSPt = sketches[id].points.length-1;
		console.log(drawSPt+"sPt:"+sketches[id].points.length);
		socket.send(JSON.stringify(point));
	}
	
}

function renderCanvas(){
	if ( ctx == null ) return;
	ctx.clearRect(0, 0, canvas.width, canvas.height);
	drawLine();
	drawCircle();
	drawTri();
	drawSq();
	drawHex();
}

function drawLine() {
	for(var i=0;i<lineArray.length;i++) {
		ctx.beginPath();
		ctx.strokeStyle="#FFFFFF";
		ctx.lineWidth=8;
		ctx.moveTo(lineArray[i].x,lineArray[i].y);
		ctx.lineTo(lineArray[i].x2, lineArray[i].y2);
		ctx.stroke();
	}
}

function drawCircle() {

	// for (var i=0;i<=10;i++) {
	// 	onMouseDraw(x+Math.cos(Math.radians(i*36))*r, y+Math.sin(Math.radians(i*36))*r);

	// }
	// ctx.beginPath();
	// ctx.strokeStyle="#FF0000";
	// ctx.arc(circleRX,circleRY,50,0,2*Math.PI);
	// ctx.stroke();
	for(var i=0;i<cirArray.length;i++) {
		ctx.beginPath();
		ctx.strokeStyle="#FFFFFF";
		ctx.lineWidth=8;
		ctx.arc(cirArray[i].x, cirArray[i].y,cirArray[i].r, 0, 2*Math.PI);
		ctx.stroke();
	}
}

function drawTri() {
	for(var i=0;i<triArray.length;i++) {
		var obj = triArray[i];
		ctx.save();
		ctx.beginPath();
		ctx.strokeStyle="#FFFFFF";
		ctx.lineWidth=8;
		ctx.translate(obj.x, obj.y);
		ctx.rotate(obj.a*Math.PI/180);
		ctx.moveTo(-obj.w*.5, obj.h*.5);
		ctx.lineTo(obj.w*.5, obj.h*.5);
		ctx.lineTo(0, -obj.h*.5);
		ctx.closePath();
		ctx.stroke();
		ctx.restore();
	}
}

function drawSq() {
	for(var i=0;i<sqArray.length;i++) {
		var obj = sqArray[i];
		ctx.save();
		ctx.beginPath();
		ctx.strokeStyle="#FFFFFF";
		ctx.lineWidth=8;
		ctx.translate(obj.x, obj.y);
		ctx.rotate(obj.a*Math.PI/180);
		ctx.strokeRect(-obj.w*.5, -obj.w*.5, obj.w, obj.w);
		ctx.stroke();
		ctx.restore();
	}
}

function drawHex() {
	for(var i=0;i<hexArray.length;i++) {
		var obj = hexArray[i];
		ctx.save();
		ctx.beginPath();
		ctx.strokeStyle="#FFFFFF";
		ctx.lineWidth=8;
		ctx.translate(obj.x, obj.y);
		ctx.rotate(obj.a*Math.PI/180);
		ctx.moveTo(obj.w*.5,0);
		for(var j=0;j<6;j++) {
			var a = (j+1)*60*Math.PI/180;
			ctx.lineTo(obj.w*.5*Math.cos(a),obj.w*.5*Math.sin(a));
		}
		ctx.closePath();
		ctx.stroke();
		ctx.restore();
	}
}

function eraseAll() {
	if ( socket.readyState == 1 ){
		var point = {id:id,erase:-1};
		socket.send(JSON.stringify(point));
	}
	sketches[id].points = [];
	ctx.clearRect(0, 0, canvas.width, canvas.height);


}

function eraseLast() {
	if ( socket.readyState == 1 ){
		var point = {id:id,erase:lastIndex};
		socket.send(JSON.stringify(point));
	}
	sketches[id].points = sketches[id].points.slice(0, lastIndex);
	ctx.clearRect(0, 0, canvas.width, canvas.height);
	renderCanvas();
}

// Converts from degrees to radians.
Math.radians = function(degrees) {
	return degrees * Math.PI / 180;
};
// Converts from radians to degrees.
Math.degrees = function(radians) {
	return radians * 180 / Math.PI;
};

// catch incoming messages + render them to the canvas

// setup web socket
function setupSocket(){

	// setup websocket
	// get_appropriate_ws_url is a nifty function by the libwebsockets people
	// it decides what the websocket url is based on the broswer url
	// e.g. https://mygreathost:9099 = wss://mygreathost:9099
	var _socket = new WebSocket(get_appropriate_ws_url());
	
	// open
	try {
		_socket.onopen = function() {
			console.log("open");
			// statusDiv.style.backgroundColor = "#000000";
			// statusDiv.textContent = " websocket connection opened ";
		} 

		// received message
		_socket.onmessage =function got_packet(msg) {
			console.log(msg);
			var message = JSON.parse(msg.data);

			if ( message.setup ){
				// set up our drawing!
				color 	= message.setup.color;
				id 		= message.setup.id;

				sketches[id] = {color:color, points:[]};
			} else if ( message.point ){
				//console.log(message.point);
				var c = message.color;
				var _id = message.id;

				// if we don't know this one, add it to our list
				if ( !sketches[_id] ){
					sketches[_id] = {color:c, points:[]};
				}
				sketches[_id].points.push( message.point );
				if ( sketches[_id].points.length > 500 ){
					//sketches[_id].points.shift();
				}
			} else if ( message.erase ){

				var _id = message.erase;
				if ( sketches[_id] ){
					delete sketches[_id];
				}
			}
			renderCanvas();
		}

		_socket.onclose = function(){
			// statusDiv.style.backgroundColor = "#ff4040";	
			// statusDiv.textContent = " websocket connection CLOSED ";
		}
		return _socket;
	} catch(exception) {
		alert('<p>Error' + exception);  
	}
	return null;
}

function saveCanvas() {
	// console.log("saveCanvas");
	var canvasG = document.getElementById("sketchCanvas");
	//ctx.scale(0.5,0.5);
	if(canvasG.toBlob){
		canvasG.toBlob(
          function (newBlob) {
            socket.send(newBlob);
          },
          'image/jpeg'
        )
	}
}

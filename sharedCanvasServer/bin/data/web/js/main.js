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

$(window).load(function() {

	setupGui();
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
		canvas.width  = window.innerWidth;
		canvas.height = window.innerHeight;
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

	ledSpeed.onFinishChange(function(value) {
  		// Fires when a controller loses focus.
  		
  	});

	strokeW.onFinishChange(function(value) {
  		// Fires when a controller loses focus.
  		strokeW = parseInt(value);
  	});

	circleUsing.onFinishChange(function(value) {
		eraseAll();
	});

	circleR.onFinishChange(function(value) {
  		// Fires when a controller loses focus.
  		alert("The new value is " + value);
  	});

	

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

var bMouseDown = false;
function onMouseDown( e ){
	mouseX = (e.changedTouches ? e.changedTouches[0].pageX : e.pageX);
	mouseY = (e.changedTouches ? e.changedTouches[0].pageY : e.pageY);
	bMouseDown = true;
	lastIndex = sketches[id].points.length+1;
	//get the new draw point
	saveDrawSpt(mouseX, mouseY);
	onMouseDraw( mouseX, mouseY );
	//drawCircle(50, mouseX, mouseY);
}

function onMouseMoved( e ){
	mouseX = (e.changedTouches ? e.changedTouches[0].pageX : e.pageX);
	mouseY = (e.changedTouches ? e.changedTouches[0].pageY : e.pageY);
	if ( bMouseDown ){//for mouse drag draw function
		onMouseDraw( mouseX, mouseY );
	}
}

function onMouseUp( e ){
	bMouseDown = false;
	//onMouseDraw(-1, -1);

}

// catch mouse events on canvas
function onMouseDraw( x, y ){
	//rescale the point for server
	x =Math.floor(x/wOffset);
	y=Math.floor(y/hOffset);
	var sw = parseInt(strokeW);
	console.log(x+"::"+y);

	var point = {point:{x:x,y:y},id:id,color:color,sw:sw};
	if ( socket.readyState == 1 ){
		sketches[id].points.push( point.point );

		if ( sketches[id].points.length > 500 ){
				//sketches[id].points.shift();
			}
			socket.send(JSON.stringify(point));
			renderCanvas();
		}
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
	
	canvas.width = canvas.width;
	ctx.moveTo(0,0);
	for ( var _id in sketches ){
		var c = sketches[_id].color;
		var pts = sketches[_id].points;
		ctx.lineWidth=strokeW;
		ctx.strokeStyle = 'rgb('+c.r+','+c.g+','+c.b+')';
		ctx.beginPath();
		if ( pts.length > 0 ){			
			//ctx.moveTo(pts[drawSPt].x*wOffset, pts[drawSPt].y*hOffset);
			//console.log(pts[drawSPt].x*wOffset+"::"+drawSPt);
			for ( var i=0, len = pts.length; i<len; i++){
				// if(pts[i].x<0) {
				// 	drawSPt = i+1;
				// 	break;
				// }
				if(pts[i].x<0) {
					ctx.moveTo(-pts[i].x*wOffset, -pts[i].y*hOffset);
				} else {
					ctx.lineTo( pts[i].x*wOffset, pts[i].y*hOffset );
				}

			}
			ctx.stroke();
		}

	}
}

function drawCircle(r, x, y) {

	for (var i=0;i<=10;i++) {
		onMouseDraw(x+Math.cos(Math.radians(i*36))*r, y+Math.sin(Math.radians(i*36))*r);
		
	}
}

function drawRect() {

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
			//console.log(msg.data);
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


function setupGui(){
	var ledAttr = function() 
	{ 
		this.speed=0 
		this.stroke=1;
		this.clear = function() { eraseAll();};
		this.clearLast = function() {eraseLast();};

		this.capture = function() {captureJpeg();};
	};
	var cirAttr = function() 
	{
		this.radius=10;
		this.use= false;
	};

	var sqAttr = function() 
	{
		this.length=10;
		this.angle=0;
		this.use=false;
	};
	var captureJpeg =function(){
		var canvas = document.getElementById("sketchCanvas");
		canvas.toBlob(function(blob) {			
			socket.send(blob);
		}, "image/jpeg", 1);
	}
	

	var ab2str = function(buf) {
		return String.fromCharCode.apply(null, new Uint16Array(buf));
	}

	var str2ab = function (str) {
	  var buf = new ArrayBuffer(str.length*2); // 2 bytes for each char
	  var bufView = new Uint16Array(buf);
	  for (var i=0, strLen=str.length; i<strLen; i++) {
	  	bufView[i] = str.charCodeAt(i);
	  }
	  return buf;
	}

	var gui = new dat.GUI();
	var led = new ledAttr();

	var cir = new cirAttr();
	var sq = new sqAttr();
	var cF = gui.addFolder('Cicrle');

		// cF.open();

		// var rF = gui.addFolder('Square');
		// rF.add(sq, 'length',1,20);
		// rF.add(sq, 'angle',0,90);
		// rF.add(sq, 'use');
		// rF.open();

		//var ledSpeed = gui.add(led, 'speed', 0, 500);
		//var strokeW = gui.add(led, 'stroke', 1, 20).step(1);
		var clearField = gui.add(led, 'clear');
		var clearLast = gui.add(led, 'clearLast');
		var capture = gui.add(led, 'capture');
		//var circleR = cF.add(cir, 'radius', 1, 20);
		//var circleUsing = cF.add(cir, 'use');

		//cF.add(obj, 'speed', 0, 500);	
	}
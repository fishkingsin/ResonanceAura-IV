
precision highp float;

uniform sampler2D tex0;

varying vec2 texCoordVarying;
uniform float iGlobalTime;
vec4 iResolution ;

float speed=iGlobalTime;
const float PI = 3.141592658;
const float TAU = 2.0 * PI;
const float sections = 10.0;
uniform float scale;
void main()
{
  vec2 fragCoord = texCoordVarying;
  vec2 uv = fragCoord.xy;
  float x = (uv.x * scale)-1.0;
  float y = uv.y * 1.0;
  // uv.x = uv.x * -scale;
    // uv.y = uv.y * 2;
if(iGlobalTime/60.0>7.0) {
  uv.x*=2.0;
  uv.y*=1.0;
}else if(iGlobalTime/60.0>6.0) {
  uv.x*=8.0;
  uv.y*=4.0;
}else if(iGlobalTime/60.0>4.0) {
  uv.x*=6.0;
  uv.y*=3.0;
} else if(iGlobalTime/60.0>2.0) {
  uv.x*=4.0;
  uv.y*=2.0;
} else {
  uv.x*=2.0;
  uv.y*=1.0;
}
  

  float delta = sin(iGlobalTime*PI*0.004);
  float delta2 = cos(iGlobalTime*PI*0.0041);
  float delta3 = 0.0;//sin(iGlobalTime*0.001);
  float delta4 = 0.0;//sin(iGlobalTime*0.0011);

  vec2 OVal = uv;

//  float tmpT = floor(speed/60.0);
//  speed-=60.0*tmpT;
//  speed*=.5;
//    uv.x+=speed;
  if(iGlobalTime/60.0>7.0) {
  OVal=uv;
  } else if(iGlobalTime/60.0>5.0) {
    OVal = uv+vec2(0.0,((fragCoord.x<0.5)?iGlobalTime*0.3:-(iGlobalTime*0.8)));
  } else if(iGlobalTime/60.0>3.0) {
    OVal = uv+vec2(((fragCoord.y<0.5)?sin(iGlobalTime*0.3):-sin(iGlobalTime*0.6)),0.0);
  } else {
  OVal = uv;
}
  //uv.x+=iGlobalTime*.535;
  gl_FragColor = texture2D(tex0, OVal);
   //gl_FragColor = texture2D(tex0, uv);
}
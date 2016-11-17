
precision highp float;

uniform sampler2D tex0;

varying vec2 texCoordVarying;
vec4 iResolution ;
void main()
{
	iResolution = texCoordVarying;
    mainImage(gl_FragColor, glFragCoord);
}

const float PI = 3.141592658;
const float TAU = 2.0 * PI;
const float sections = 10.0;

void mainImage( out vec4 fragColor, in vec2 fragCoord ){
  vec2 pos = vec2(fragCoord.xy - 0.5 * iResolution.xy) / iResolution.y;

  float rad = length(pos);
  float angle = atan(pos.y, pos.x);

  float ma = mod(angle, TAU/sections);
  ma = abs(ma - PI/sections);
  
  float x = cos(ma) * rad;
  float y = sin(ma) * rad;
	
  float time = iGlobalTime/10.0;
  
  fragColor = texture2D(iChannel0, vec2(x-time, y-time));
}

float Tile1D(float p, float a){
  p -= 4.0 * a * floor(p/4.0 * a);
  p -= 2.* max(p - 2.0 * a , 0.0);
  return p;
}
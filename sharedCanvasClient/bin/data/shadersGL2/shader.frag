#version 120

// this is how we receive the texture
uniform sampler2DRect tex0;

uniform int iGlobalTime;
varying vec2 texCoordVarying;
uniform vec3 iResolution ;

const float PI = 3.141592658;
const float TAU = 2.0 * PI;
const float sections = 10.0;


void mainImage( out vec4 fragColor, in vec2 fragCoord ){
  vec2 uv = fragCoord.xy;
  // vec2 output = uv+vec2(iGlobalTime, 0);
  
  vec2 OVal = uv+vec2(iGlobalTime/100.0, 0);
 
  fragColor = texture2DRect(tex0, OVal);
}

float Tile1D(float p, float a){
  p -= 4.0 * a * floor(p/4.0 * a);
  p -= 2.* max(p - 2.0 * a , 0.0);
  return p;
}
void main()
{
	// gl_FragColor = texture2DRect(tex0, texCoordVarying);
    mainImage(gl_FragColor, texCoordVarying);
}


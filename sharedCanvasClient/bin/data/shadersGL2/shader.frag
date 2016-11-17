#version 120

// this is how we receive the texture
uniform sampler2D tex0;

uniform float iGlobalTime;
varying vec2 texCoordVarying;
uniform vec3 iResolution ;
float PI = 3.141592658;

// const float sections = 10.0;


void mainImage( out vec4 fragColor, in vec2 fragCoord ){
  vec2 uv = fragCoord.xy;
  float delta = sin(iGlobalTime*PI*0.03);
  float delta2 = sin(iGlobalTime*PI*0.07);
  float delta3 = sin(iGlobalTime*PI*0.01);
  float delta4 = sin(iGlobalTime*PI*0.02);
  vec2 OVal = uv+vec2(((fragCoord.y>0.5)? delta : delta2), ((fragCoord.y>0.5)? delta3 : delta4));
 
  fragColor = texture2D(tex0, OVal);
}

float Tile1D(float p, float a){
  p -= 4.0 * a * floor(p/4.0 * a);
  p -= 2.* max(p - 2.0 * a , 0.0);
  return p;
}
void main()
{
    mainImage(gl_FragColor, texCoordVarying);
}


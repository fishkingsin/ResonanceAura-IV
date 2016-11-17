
precision highp float;

uniform sampler2D tex0;

varying vec2 texCoordVarying;
uniform float iGlobalTime;
vec4 iResolution ;
void main()
{
	iResolution = texCoordVarying;
  mainImage(gl_FragColor, texCoordVarying);
}

const float PI = 3.141592658;
const float TAU = 2.0 * PI;
const float sections = 10.0;

void mainImage( out vec4 fragColor, in vec2 fragCoord ){
  vec2 uv = fragCoord.xy;
  float delta = sin(iGlobalTime*PI*0.03);
  float delta2 = sin(iGlobalTime*PI*0.07);
  float delta3 = sin(iGlobalTime*PI*0.01);
  float delta4 = sin(iGlobalTime*PI*0.02);
  vec2 OVal = uv+vec2(((fragCoord.y>0.5)? delta : delta2), ((fragCoord.y>0.5)? delta3 : delta4));
 
  fragColor = texture2D(tex0, OVal);
}

// float Tile1D(float p, float a){
//   p -= 4.0 * a * floor(p/4.0 * a);
//   p -= 2.* max(p - 2.0 * a , 0.0);
//   return p;
// }
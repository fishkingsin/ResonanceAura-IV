
precision highp float;

uniform sampler2D tex0;

varying vec2 texCoordVarying;
uniform float iGlobalTime;
vec4 iResolution ;


const float PI = 3.141592658;
const float TAU = 2.0 * PI;
const float sections = 10.0;
uniform float scale;
void main()
{
	vec2 fragCoord = texCoordVarying;
	vec2 uv = fragCoord.xy;
	float x = uv.x * -scale;
	float y = uv.y * 1.0;
	// uv.x = uv.x * -scale;//-scale*abs(sin(0.01 * iGlobalTime)+2); 
  	// uv.y = uv.y * 2; 
	

	float delta = sin(iGlobalTime*PI*0.04);
	float delta2 = sin(iGlobalTime*PI*0.041);
	float delta3 = 0.0;//sin(iGlobalTime*0.001);
	float delta4 = 0.0;//sin(iGlobalTime*0.0011);

	vec2 OVal = vec2(x,y)-vec2(((fragCoord.y>0.5)? delta : delta2), ((fragCoord.y>0.5)? delta3 : delta4));


	gl_FragColor = texture2D(tex0, OVal);
	// gl_FragColor = texture2D(tex0, vec2(x,y));
}
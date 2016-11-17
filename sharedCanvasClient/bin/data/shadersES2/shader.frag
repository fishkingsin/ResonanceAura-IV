
precision highp float;

uniform sampler2D tex0;

varying vec2 texCoordVarying;
uniform float iGlobalTime;
vec4 iResolution ;


const float PI = 3.141592658;
const float TAU = 2.0 * PI;
const float sections = 10.0;

void main()
{
	vec2 fragCoord = texCoordVarying;
	vec2 uv = fragCoord.xy;
	float delta = sin(iGlobalTime*PI*0.03);
	float delta2 = sin(iGlobalTime*PI*0.07);
	float delta3 = sin(iGlobalTime*PI*0.01);
	float delta4 = sin(iGlobalTime*PI*0.02);
	vec2 OVal = uv+vec2(((fragCoord.y>0.5)? delta : delta2), ((fragCoord.y>0.5)? delta3 : delta4));
	
	gl_FragColor = texture2D(tex0, OVal);
	
}
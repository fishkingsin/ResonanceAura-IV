
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
	float d = (-scale*abs(sin(0.01 * iGlobalTime)+2));
	float scale_uv_x = uv.x * d; 
	float scale_uv_y = uv.y * d; 

	float delta = sin(iGlobalTime*PI*0.04);
	float delta2 = sin(iGlobalTime*PI*0.041);
	float delta3 = sin(iGlobalTime*0.001);
	float delta4 = sin(iGlobalTime*0.0011);

	vec2 OVal = vec2(scale_uv_x,scale_uv_y)+vec2(((fragCoord.y>0.5)? delta : delta2), ((fragCoord.y>0.5)? delta3 : delta4));


	fragColor = texture2D(tex0, OVal);
	
}
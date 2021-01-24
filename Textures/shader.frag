#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float ratio;


void main()
{
	//y=0.5에 대하여 대칭을 해야 함.
	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(1-TexCoord.x, TexCoord.y)), 0.2);
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), ratio);
}
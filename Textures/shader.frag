#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;


void main()
{
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2); //3번째 인자가 0이면 앞, 1이면 뒤 그 사이면 퍼센트만큼 보간
}
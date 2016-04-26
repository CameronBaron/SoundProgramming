#version 410

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexcoord;

out vec4 FragColor;

uniform vec3 ambientLight = vec3(0.75, 0.75, 0.75);

void main()
{	
	FragColor = vec4(vTexcoord, 1, 1);
	//FragColor.rgb *= ambientLight;
}
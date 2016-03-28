#version 410

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexcoord;

out vec4 FragColor;

uniform sampler2D material;

void main()
{
	//FragColor = vec4( 1, 1, 1, 1);
	FragColor = texture(material, vTexcoord);
}
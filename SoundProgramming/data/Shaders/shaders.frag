#version 410

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexcoord;

out vec4 FragColor;

uniform sampler2D material;

uniform vec3 ambientLight = vec3(0.7f, 0.7f, 0.7f);

void main()
{

	// Ambient light
	vec3 Ambient = ambientLight * FragColor.rgb;

	
	FragColor = texture(material, vTexcoord);
	FragColor.rgb *= Ambient;
}
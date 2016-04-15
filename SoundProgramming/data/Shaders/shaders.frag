#version 410

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexcoord;

out vec4 FragColor;

uniform sampler2D material;

uniform vec3 ambientLight = vec3(0.75f, 0.75f, 0.75f);

void main()
{
	// Ambient light
	//vec3 Ambient = ambientLight * FragColor.rgb;
	//
	//FragColor = texture(material, vTexcoord);
	//FragColor.rgb *= Ambient;
	
	FragColor = vec4( 0.1f, 0.1f, 0.1f, 1);
	FragColor.rgb *= ambientLight;
}
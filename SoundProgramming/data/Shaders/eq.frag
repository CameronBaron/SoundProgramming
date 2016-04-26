#version 410

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexcoord;

out vec4 FragColor;

uniform sampler1D tex;

uniform vec3 ambientLight = vec3(0.75, 0.75, 0.75);

float GetHeightValue()
{
	float texcoord = vTexcoord.x;
	vec4 temp = texture(tex, texcoord);
	return temp.r;
}

void main()
{
	// Ambient light
	//vec3 Ambient = ambientLight * FragColor.rgb;
	//
	//FragColor = texture(material, vTexcoord);
	//FragColor.rgb *= Ambient;
	
	//FragColor.rgb *= ambientLight;

	//float offset = GetHeightValue();
	//FragColor = vec4(offset, offset, offset, 1);

	float offset = GetHeightValue();
	if (vTexcoord.y > offset)
	{
		FragColor = vec4(0, 0, 0, 1);
	}
	else
	{
		FragColor = mix(vec4(0,0.5,0.5,1), vec4(1,0,0,1), offset);
		//FragColor = vec4(0, 0.5, 0.5, 1);
	}
}

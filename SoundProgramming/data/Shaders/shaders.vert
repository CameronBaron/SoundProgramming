 #version 410

layout(location=0) in vec3 Position;
layout(location=1) in vec3 Normal;
layout(location=2) in vec2 Texcoord;

out vec3 vPosition;
out vec3 vNormal;
out vec2 vTexcoord;

uniform mat4 MVP;

void main()
{
	vPosition = Position;

	gl_Position = MVP * vec4(Position, 1); 
}
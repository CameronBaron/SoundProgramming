 #version 410

layout(location=0) in vec3 Position;
layout(location=1) in vec3 Normal;
layout(location=2) in vec2 Texcoord;

out vec3 vPosition;
out vec3 vNormal;
out vec2 vTexcoord;

uniform mat4 ProjectionView;
uniform mat4 ModelMatrix;

void main()
{
	vNormal = Normal;
	vTexcoord = Texcoord;
	vPosition = Position;

	gl_Position = ProjectionView * ModelMatrix * vec4(Position, 1); 
}
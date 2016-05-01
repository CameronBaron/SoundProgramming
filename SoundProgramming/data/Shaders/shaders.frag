#version 410

struct PointLight 
{ 
   vec3 Color;
   vec3 Position; 
    
   float fAmbient; 

   float fConstantAtt; 
   float fLinearAtt; 
   float fExpAtt; 
};

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexcoord;

out vec4 FragColor;

uniform vec4 ambientLight = vec4(0.1, 0.1, 0.1, 1);
uniform float beatIntensity;
uniform PointLight light;
uniform vec3 worldPos;

vec4 getPointLightColor(PointLight ptLight) 
{ 
	if (beatIntensity < 0.2)
	{
		return vec4(0,0,0,1);
	}
    vec3 vPosToLight = (vPosition + worldPos) - ptLight.Position;
	float fDist = length(vPosToLight);
	vPosToLight = normalize(vPosToLight);
	
	float fDiffuse = max(0.0, dot(vNormal, -vPosToLight));
	
	if (fDist < 15)
	{
		float fAttTotal = ptLight.fConstantAtt + ptLight.fLinearAtt * fDist + (1 - beatIntensity * 2) * fDist * fDist;
		return vec4(ptLight.Color, 1) * (ptLight.fAmbient + fDiffuse ) / fAttTotal;
	}
	
	return vec4(0,0,0,1);
}

void main()
{	
	FragColor = vec4(1,1,1, 1) * ambientLight * getPointLightColor(light);
}
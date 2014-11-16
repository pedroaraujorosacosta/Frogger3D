#version 330 core
 
uniform sampler2D texUnit;
uniform int texMode;

struct Materials {
	vec4 diffuse;
	vec4 ambient;
	vec4 specular;
	vec4 emissive;
	float shininess;
	int texCount;
};

uniform Materials mat;

in vec2 TexCoord;
out vec4 color;
 
void main()
{
	color = texture(texUnit, TexCoord) * mat.ambient;
}
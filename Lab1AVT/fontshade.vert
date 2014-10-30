#version 330 core

uniform mat4 m_pvm;

in vec3 position;
in vec2 texCoord;

out vec4 vertexPos;
out vec2 TexCoord;

void main () 
{
	TexCoord = vec2(texCoord);
	gl_Position = m_pvm * vec4(position,1.0);
}
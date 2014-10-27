#version 330

uniform	mat4 m_pvm;
uniform	mat4 m_viewModel;
uniform	mat3 m_normal;

uniform	vec3 l_dir;	   // camera space

in vec4 position;	// local space
in vec3 normal;		// local space

// the data to be sent to the fragment shader
out Data {
	vec3 normal;
	vec3 eye;
	vec3 lightDir;
} DataOut;

void main () {
	vec4 pos = m_viewModel * position;

	DataOut.normal = normalize(m_normal * normal.xyz);
	DataOut.lightDir = vec3(-l_dir);
	DataOut.eye = vec3(-pos);

	gl_Position = m_pvm * position;	
}
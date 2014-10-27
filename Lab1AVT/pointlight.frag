#version 330

out vec4 colorOut;

struct Materials {
	vec4 diffuse;
	vec4 ambient;
	vec4 specular;
	vec4 emissive;
	float shininess;
	int texCount;
};

struct LightProperties {
	bool isEnabled;
};

uniform LightProperties light[1];
uniform LightProperties dirLight;

uniform Materials mat;

in Data {
	vec3 normal;
	vec3 eye;
	vec3 lightDir;
	vec3 lightDirectional;
} DataIn;

void main() {

	vec4 spec = vec4(0.0);

	vec3 n = normalize(DataIn.normal);
	vec3 l = normalize(DataIn.lightDir);
	vec3 e = normalize(DataIn.eye);

	vec3 ld = normalize(DataIn.lightDirectional);

	float intensity = 0.0;
	if(light[0].isEnabled)
		intensity = max(dot(n, l), 0.0);

	if (intensity > 0.0) 
	{
		vec3 h = normalize(l + e);
		float intSpec = max(dot(h, n), 0.0);
		spec = mat.specular * pow(intSpec, mat.shininess);
	}
	
	float intensity2 = 0.0;
	if(dirLight.isEnabled)
		intensity2 = max(dot(n, ld), 0.0);

	if (intensity2 > 0.0) 
	{
		vec3 h = normalize(ld + e);
		float intSpec = max(dot(h, n), 0.0);
		spec += mat.specular * pow(intSpec, mat.shininess);
	}

	colorOut = max((intensity + intensity2)* mat.diffuse + spec, mat.ambient);
}
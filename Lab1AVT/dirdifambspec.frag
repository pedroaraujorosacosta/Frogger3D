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
	bool isLocal;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
};

const int MaxLights = 1;
uniform LightProperties Lights[MaxLights];

uniform Materials mat;

in Data {
	vec3 normal;
	vec3 eye;
	vec3 pos;
} DataIn;

void main() {
	vec3 scatteredLight = vec3(0.0); 
	vec3 reflectedLight = vec3(0.0);

	vec3 Normal = normalize(DataIn.normal);
	vec3 EyeDirection = normalize(DataIn.eye);

	float attenuation = 0.0; // remover isto

	// for all lights
	for (int light = 0; light < MaxLights; ++light) {		
		
		// Accumulate all the lights’ effects
		scatteredLight += Lights[light].ambient;
		reflectedLight += Lights[light].specular;
	}
	
	vec3 rgb = scatteredLight;
	colorOut = vec4(rgb, 1.0);
}
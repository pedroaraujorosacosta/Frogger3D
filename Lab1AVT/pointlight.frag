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
bool isSpot;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;
vec3 halfVector;
vec3 coneDirection;
float spotCosCutoff;
float spotExponent;
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
};

const int MaxLights = 7;
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

	// for all lights
	for (int light = 0; light < MaxLights; ++light) {
		if (! Lights[light].isEnabled)
			continue;
		
		vec3 halfVector;
		vec3 lightDirection = Lights[light].position; // works for both directional and point
		float attenuation = 1.0;

		if (Lights[light].isLocal) {
			lightDirection = lightDirection - vec3(DataIn.pos);
			float lightDistance = length(lightDirection);
			lightDirection = lightDirection / lightDistance;

			attenuation = 1.0 /
				(Lights[light].constantAttenuation
					+ Lights[light].linearAttenuation * lightDistance
					+ Lights[light].quadraticAttenuation * lightDistance * lightDistance);	
		}

		halfVector = normalize(lightDirection + EyeDirection);

		float diffuse = max(0.0, dot(Normal, lightDirection));
		float specular = max(0.0, dot(Normal, halfVector));

		if (diffuse == 0.0)
			specular = 0.0;
		else
			specular = pow(specular, mat.shininess);
			
		// Accumulate all the lights’ effects
		scatteredLight.x += Lights[light].ambient.x * mat.ambient.x * attenuation + Lights[light].diffuse.x * mat.diffuse.x * diffuse * attenuation;
		scatteredLight.y += Lights[light].ambient.y * mat.ambient.y * attenuation + Lights[light].diffuse.y * mat.diffuse.y * diffuse * attenuation;
		scatteredLight.z += Lights[light].ambient.z * mat.ambient.z * attenuation + Lights[light].diffuse.z * mat.diffuse.z * diffuse * attenuation;
		reflectedLight.x += Lights[light].specular.x * mat.specular.x * specular * attenuation;
		reflectedLight.y += Lights[light].specular.y * mat.specular.y * specular * attenuation;
		reflectedLight.z += Lights[light].specular.z * mat.specular.z * specular * attenuation;
	}
	
	vec3 rgb = min(scatteredLight + reflectedLight, vec3(1.0));
	colorOut = vec4(rgb, 1.0);
}
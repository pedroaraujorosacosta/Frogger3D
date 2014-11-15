#version 330

const int ALPHA_TEST_NONE = 0;
const int ALPHA_TEST_OPAQUE = 1;
const int ALPHA_TEST_TRANS = 2;

uniform int alphaTest;
uniform sampler2D texmap;
uniform sampler2D texmap1;
uniform sampler2D texmap2;
uniform sampler2D texmap3;

uniform int texMode;
uniform vec3 frogPos;
uniform vec4 fogColor;
uniform float fogDensity;
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
	vec3 coneDirection;
	float spotCosCutoff;
	float spotExponent;
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
};

const int MaxLights = 8;
uniform LightProperties Lights[MaxLights];

uniform Materials mat;

in Data {
	vec3 normal;
	vec3 eye;
	vec3 pos;
	vec2 tex_coord;
} DataIn;

vec4 applyFog(in vec4 rgba, in float distance) {
	float fogAmount = exp(-(distance * fogDensity));
	return mix(rgba, fogColor, (1.0 - fogAmount));
}

void main() {
	vec4 texel, texel1;
	vec4 scatteredLight = vec4(0.0); 
	vec4 reflectedLight = vec4(0.0);

	vec3 Normal = normalize(DataIn.normal);
	vec3 EyeDirection = normalize(DataIn.eye);

	colorOut = vec4(1.0);

	if(texMode == 1) // modulate Phong color with texel color
	{
		texel = texture(texmap, DataIn.tex_coord);  // water.tga
		colorOut = texel;
	} else 	if(texMode == 2) // modulate Phong color with texel color
	{
		texel = texture(texmap1, DataIn.tex_coord);  // stone.tga
		colorOut = texel;
	} else 	if(texMode == 3) // modulate Phong color with texel color
	{
		texel = texture(texmap2, DataIn.tex_coord);  // grass.tga
		colorOut = texel;
	} else 	if(texMode == 4) // modulate Phong color with texel color
	{
		texel = texture(texmap3, DataIn.tex_coord);  // tree.tga
		colorOut = texel;
	}

	// do an early alpha test (we can cull some of the textured fragments early)
	if(alphaTest == ALPHA_TEST_OPAQUE) 
	{
		if(colorOut.w < 1.0)
			discard;
	}

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

			if (Lights[light].isSpot) {
				float spotCos = dot(lightDirection,	-Lights[light].coneDirection);
				if (spotCos < Lights[light].spotCosCutoff)
					attenuation = 0.0;
				else
					attenuation *= pow(spotCos,	Lights[light].spotExponent);
			}
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
		scatteredLight.w += mat.ambient.w * attenuation + mat.diffuse.w * diffuse * attenuation;
		reflectedLight.x += Lights[light].specular.x * mat.specular.x * specular * attenuation;
		reflectedLight.y += Lights[light].specular.y * mat.specular.y * specular * attenuation;
		reflectedLight.z += Lights[light].specular.z * mat.specular.z * specular * attenuation;
		reflectedLight.w += mat.specular.w * specular * attenuation;
	}
	
	vec4 rgba = min(scatteredLight + reflectedLight, vec4(1.0));
	// do the alpha test again
	if(alphaTest == ALPHA_TEST_OPAQUE) 
	{
		if(rgba.w < 1.0)
			discard;
	} 
	else if(alphaTest == ALPHA_TEST_TRANS) 
	{
		if(rgba.w == 1.0)
			discard;
	}
	
	colorOut *= vec4(rgba);
	float distance = length(DataIn.pos - frogPos);
	colorOut = applyFog(colorOut, distance);
	
}
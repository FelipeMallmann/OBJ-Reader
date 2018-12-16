#version 330 core

in vec3 Normal;
in vec3 FragPos;  
in vec2 Texture;

uniform sampler2D texture1;
uniform vec3 lightPos, viewPos;
uniform vec3 Ka, Kd, Ks;
uniform float Ns; // shininess

vec3 ambientColor = vec3 (1.0, 1.0, 1.0); // grey ambient colour
vec3 diffuseColor = vec3 (1.0, 1.0, 1.0); // dull white diffuse light colour
vec3 specularColor = vec3 (1.0, 1.0, 1.0); // white specular colour

out vec4 color;

void main() {

	// Ambient
    vec3 ambient = Ka * ambientColor;

	// Diffuse
	vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = Kd * diffuseColor * diff;

	// Specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), Ns);
	vec3 specular = Ks * specularColor * spec;  

	// Texture
	vec4 texel = texture(texture1, Texture);

	// Result
    vec3 result = (ambient + diffuse + specular) * texel.rgb;
	color = vec4(result, 1.0f);
	//color = texel;
};
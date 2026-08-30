#version 330 core

in vec2 TexCoord;
in vec3 FragNormal;
in vec3 WorldPos;

uniform sampler2D texture1;
uniform vec3 viewPos;

struct Material {
	vec3  tint;
	float shininess;
	float specStrength;
};
uniform Material material;

struct DirLight {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirLight sun;
uniform DirLight skyFill;

#define NR_POINT_LIGHTS 4
struct PointLight {
	vec3 position;
	vec3 color;
	float constant;
	float linear;
	float quadratic;
};
uniform PointLight pointLights[NR_POINT_LIGHTS];

out vec4 FragColor;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 color)
{
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec3 ambient  = light.ambient  * color;
	vec3 diffuse  = light.diffuse  * diff * color;
	vec3 specular = light.specular * spec * material.specStrength;
	return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 color)
{
	vec3 lightDir = normalize(light.position - WorldPos);
	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	float dist = length(light.position - WorldPos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);

	vec3 diffuse  = light.color * diff * color;
	vec3 specular = light.color * spec * material.specStrength;
	return (diffuse + specular) * attenuation;
}

void main()
{
	vec3 color = texture(texture1, TexCoord).rgb * material.tint;

	vec3 normal = normalize(FragNormal);
	vec3 viewDir = normalize(viewPos - WorldPos);

	vec3 result = CalcDirLight(sun, normal, viewDir, color);
	result += CalcDirLight(skyFill, normal, viewDir, color);
	for (int i = 0; i < NR_POINT_LIGHTS; i++)
		result += CalcPointLight(pointLights[i], normal, viewDir, color);

	FragColor = vec4(result, 1.0);
}

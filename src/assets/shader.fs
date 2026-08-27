#version 330 core

in vec2 TexCoord;
in vec3 FragNormal;
in vec3 WorldPos;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec3 sunDir;
uniform vec3 sunColor;
uniform vec3 skyColor;

out vec4 FragColor;

void main()
{
	vec3 albedo = texture(texture1, TexCoord).rgb;
	vec3 N = normalize(FragNormal);
	vec3 L = normalize(-sunDir);

	float diff = max(dot(N, L), 0.0);

	float up = N.y * 0.5 + 0.5;
	vec3 ambient = mix(vec3(0.25, 0.22, 0.20), skyColor * 0.45, up);

	vec3 lighting = ambient + sunColor * diff;

	FragColor = vec4(albedo * lighting, 1.0);
}

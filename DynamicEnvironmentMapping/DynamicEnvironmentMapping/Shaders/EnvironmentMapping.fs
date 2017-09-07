#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{
	// reflection
	/*vec3 I = normalize(Position - cameraPos);
	vec3 R = reflect(I, normalize(Normal));
	FragColor = vec4(texture(skybox, R).rgb, 1.0);*/
	// refraction
	float ratio = 1.00 / 1.52;
	vec3 I = normalize(Position - cameraPos);
	vec3 R = refract(I, normalize(Normal), ratio);
	FragColor = vec4(texture(skybox, R).rgb, 1.0);
}
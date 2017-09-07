#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

const float offset = 1.0 / 300.0;

void main()
{
	// Color inversion
	//FragColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);
	// Grayscale
	//FragColor = texture(screenTexture, TexCoords);
	//float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b; // improved equation
	//FragColor = vec4(average, average, average, 1.0);
	// Kernel
	vec2 offsets[9] = vec2[]
	(
		vec2(-offset, offset),
		vec2(0.0f, offset),
		vec2(offset, offset),
		vec2(-offset, 0.0f),
		vec2(0.0f, 0.0f),
		vec2(offset, 0.0f),
		vec2(-offset, -offset),
		vec2(0.0f, -offset),
		vec2(offset, -offset)
	);
	// sharpen
	/*float kernel[9] = float[]
	(
		-1, -1, -1,
		-1, 9, -1,
		-1, -1, -1
	);*/
	/*float kernel[9] = float[]
	(
		1.0 / 16, 2.0 / 16, 1.0 / 16,
		2.0 / 16, 4.0 / 16, 2.0 / 16,
		1.0 / 16, 2.0 / 16, 1.0 / 16
	);*/
	float kernel[9] = float[]
	(
		1.0, 1.0, 1.0,
		1.0, -8.0, 1.0,
		1.0, 1.0, 1.0
	);
	vec3 sampleTex[9];
	for (int i = 0; i < 9; ++i)
	{
		vec2 newTex = TexCoords.st + offsets[i];
		if (newTex.s < 0.0) newTex.s = 0.0;
		if (newTex.t < 0.0) newTex.t = 0.0;
		if (newTex.s > 1.0) newTex.s = 1.0;
		if (newTex.t > 1.0) newTex.t = 1.0;
		sampleTex[i] = vec3(texture(screenTexture, newTex));
	}
	vec3 col = vec3(0.0);
	for (int i = 0; i < 9; ++i)
		col += sampleTex[i] * kernel[i];
	FragColor = vec4(col, 1.0);
	// Normal results
	//FragColor = texture(screenTexture, TexCoords);
}
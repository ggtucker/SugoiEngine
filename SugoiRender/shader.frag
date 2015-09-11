#version 330 core

in vec3 Color;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D Texture1;
uniform sampler2D Texture2;

void main() {
	color = mix(texture(Texture1, TexCoord), texture(Texture2, TexCoord), 0.8);
}
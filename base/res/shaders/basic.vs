#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texture;
layout(location = 2) in vec3 a_normal;

uniform mat4 model, view, projection;

out vec3 Normal;
out vec3 FragPos;  
out vec2 Texture;

void main()
{	
	//Normal = a_normal;
	Normal = mat3(transpose(inverse(model))) * a_normal;  //Use this if perform any scale on objects

	Texture = a_texture;
	FragPos = vec3(model * vec4(a_position, 1.0));

	gl_Position = projection * view * model * vec4(a_position, 1.0f);
}
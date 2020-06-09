#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;


out vec2 TexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;


void main() {
     vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
     vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
    // gl_Position = projection * view * model * vec4(aPos, 1.0);
     gl_Position =   projection * view * model * vec4(aPos, 1.0);
     gl_PointSize = 10.0;
}
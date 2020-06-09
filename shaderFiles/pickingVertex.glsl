#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 Pos;

// Values that stay constant for the whole mesh.
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main(){


    gl_Position =  projection * view * model * vec4(Pos,1);
}
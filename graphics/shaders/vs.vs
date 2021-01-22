#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texCoords;
layout(location = 2) in vec3 a_normal;

out vec3 v_position;
out vec2 v_texCoords;
out vec3 v_normal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_iModel;

void main()
{
    v_position = vec3(u_model * vec4(a_position, 1.0));
    v_normal = vec3(u_iModel * vec4(a_normal, 1));
    v_normal = normalize(v_normal);
    v_texCoords = a_texCoords;
    gl_Position = u_projection * u_view * vec4(v_position, 1);
}
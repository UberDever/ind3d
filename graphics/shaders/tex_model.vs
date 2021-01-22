#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texture;
layout(location = 2) in vec3 a_normal;

out vec2 v_texture;
out vec3 v_pos;
out vec3 v_normal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    vec4 pos = u_model * vec4(a_position, 1.0);
    gl_Position = u_projection * u_view *  pos;
    v_pos = vec3(pos.xyz);
    v_texture = a_texture;
    v_normal = a_normal;
}
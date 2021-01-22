#version 330 core

in vec2 v_texture;

out vec4 f_color;

uniform sampler2D u_texture;

void main()
{
    //f_color = vec4(1, 1, 1, 1);
    f_color = texture(u_texture, v_texture);
}
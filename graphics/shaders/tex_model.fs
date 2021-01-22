#version 330 core

in vec2 v_texture;
in vec3 v_pos;
in vec3 v_normal;

out vec4 f_color;

uniform sampler2D u_texture;
uniform vec3 u_light_pos;
uniform vec3 u_light_color;

void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * u_light_color;
  	
    // diffuse 
    vec3 norm = normalize(v_normal);
    vec3 lightDir = normalize(u_light_pos - v_pos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_light_color;
            
    vec3 obj_color = vec3(texture(u_texture, v_texture));
    vec3 result = (ambient + diffuse) * obj_color;
    f_color = vec4(result, 1);
}
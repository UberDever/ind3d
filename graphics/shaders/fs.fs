#version 330 core
#define N_POINT_LIGHTS 128

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirLight {
    vec3 direction;
	Light properties;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;

	Light properties;
};

in vec3 v_position;
in vec2 v_texCoords;
in vec3 v_normal;

out vec4 f_color;

uniform Material u_material;
uniform vec3 u_viewPos;
uniform DirLight u_dirLight;
uniform PointLight u_pointLights[N_POINT_LIGHTS];
uniform int u_pLightCount;

vec4 calculateLight(Light lightProperties, vec3 lightDir, vec3 fragmentPos, vec3 viewDir, vec3 normal, vec4 diffuse_color, vec4 specular_color)
{
    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
    // combine results
    vec4 ambient = vec4(lightProperties.ambient, 1) * diffuse_color;
    vec4 diffuse = vec4(lightProperties.diffuse, 1) * diffuse_color * diff;
    vec4 specular = vec4(lightProperties.specular, 1) * specular_color * spec;
    return (ambient + diffuse + specular);
}

void main()
{
    vec4 resColor = vec4(0);
    vec3 viewDir = normalize(u_viewPos - v_position);

    vec4 diffuse_color = texture(u_material.diffuse, v_texCoords);
    vec4 specular_color = texture(u_material.specular, v_texCoords);

    //Dir light
    //vec3 lightDir = normalize(-u_dirLight.direction);
    //resColor += calculateLight(u_dirLight.properties, lightDir, v_position, viewDir, v_normal, diffuse_color, specular_color);
    for (int i = 0; i < u_pLightCount; i++)
    {
        vec3 lightDir = normalize(u_pointLights[i].position - v_position);
        float distance = length(u_pointLights[i].position - v_position);
        if (distance > 32)
            continue;
        vec4 pointColor = calculateLight(u_pointLights[i].properties, lightDir, v_position, viewDir, v_normal, diffuse_color, specular_color);
        float attenuation = 1.0 / (u_pointLights[i].constant +
                                  u_pointLights[i].linear * distance +
                                  u_pointLights[i].quadratic * (distance * distance));
        pointColor *= attenuation;
        resColor += vec4(pointColor.xyz, 0);
    }
    resColor.w = diffuse_color.w;
    f_color = resColor;
}
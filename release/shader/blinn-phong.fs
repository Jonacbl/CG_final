#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D floorTexture;
uniform vec3 lightDirection; //parallel light
uniform vec3 viewPos;

void main()
{           
    vec3 color = texture(floorTexture, fs_in.TexCoords).rgb;

    // Ambient
    vec3 ambient = 0.2 * color;

    // Diffuse
    vec3 normalizedLightDirection = normalize(-lightDirection); 
    vec3 normal = normalize(fs_in.Normal);
    float diff = max(dot(normalizedLightDirection, normal), 0.0);
    vec3 diffuse = diff * color;

    // Specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-normalizedLightDirection, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(normalizedLightDirection + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0); // shininess factor

    vec3 specular = vec3(0.3) * spec;

    // ×îÖÕÑÕÉ«
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}
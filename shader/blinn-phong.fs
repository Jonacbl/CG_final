#version 330 core

in vec3 FragPos;             // 片元位置
in vec3 Normal;              // 法向量
in vec2 TexCoords;           // 纹理坐标
in vec4 FragPosLightSpace;   // 片元在光源空间中的位置

uniform sampler2D floorTexture;  // 纹理
uniform vec3 lightDirection;     // 平行光方向
uniform vec3 viewPos;           // 摄像机位置
uniform sampler2D shadowMap;    // 深度贴图

out vec4 FragColor;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float bias = 0.005;
    return currentDepth > closestDepth + bias ? 1.0 : 0.0;
}

void main()
{           
    vec3 color = texture(floorTexture, TexCoords).rgb;

    // 环境光
    vec3 ambient = 0.05 * color;

    // 漫反射光
    vec3 normalizedLightDirection = normalize(-lightDirection); 
    vec3 normal = normalize(Normal);
    float diff = max(dot(normalizedLightDirection, normal), 0.0);
    vec3 diffuse = diff * color;

    // 镜面反射
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-normalizedLightDirection, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(normalizedLightDirection + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.3) * spec;

    // 阴影计算
    float shadow = ShadowCalculation(FragPosLightSpace);

    FragColor = vec4(ambient + diffuse + specular, 1.0) * (1.0 - shadow);
}

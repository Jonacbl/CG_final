#version 330 core

layout (location = 0) in vec3 aPos;       
layout (location = 1) in vec3 aNormal;      
layout (location = 2) in vec2 aTexCoords;      
layout (location = 3) in vec3 tangent;        
layout (location = 4) in vec3 bitangent;     
layout (location = 5) in ivec4 boneIds;       
layout (location = 6) in vec4 weights;       

uniform mat4 projection;                
uniform mat4 view;                          
uniform mat4 model;                          

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

out VS_OUT {
    vec3 FragPos;                               
    vec3 Normal;                            
    vec2 TexCoords;                             
} vs_out;

void main()
{
    vec4 totalPosition = vec4(0.0f);
    vec3 totalNormal = vec3(0.0f);

    // 计算顶点位置和法线
    for(int i = 0; i < MAX_BONE_INFLUENCE; i++)
    {
        if(boneIds[i] == -1) 
            continue;
        if(boneIds[i] >= MAX_BONES) 
        {
            totalPosition = vec4(aPos, 1.0f);
            totalNormal = aNormal; // 如果没有骨骼影响，使用原法线
            break;
        }
        vec4 localPosition = finalBonesMatrices[boneIds[i]] * vec4(aPos, 1.0f);
        totalPosition += localPosition * weights[i];
        totalNormal += mat3(finalBonesMatrices[boneIds[i]]) * aNormal * weights[i]; // 加权法线
    }

    
    vs_out.FragPos = vec3(model * totalPosition); // 计算片段位置
    vs_out.Normal = normalize(mat3(transpose(inverse(model))) * totalNormal); // 使用转置逆矩阵规范化法线
    vs_out.TexCoords = aTexCoords; // 纹理坐标
    gl_Position = projection * view * model * totalPosition;

}
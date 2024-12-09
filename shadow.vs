#version 330 core

layout (location = 0) in vec3 aPos;  // ����λ��

uniform mat4 lightSpaceMatrix;  // ��Դ�ռ����
uniform mat4 model;            // ģ�;���

void main()
{
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
}

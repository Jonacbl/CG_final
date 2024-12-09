#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <learnopengl/shader_m.h>
#include <learnopengl/model_animation.h>
#include <learnopengl/animator.h>
#include <learnopengl/camera.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const unsigned int SCR_WIDTH_ = 1600;
const unsigned int SCR_HEIGHT_ = 900;
const glm::vec3 cameraPos = glm::vec3(0.0f, -5.0f, -10.0f);


void draw_cube(Shader &ourShader, Camera &camera, glm::vec3 lightDir, glm::vec3 worldPos, Model cube) {
    ourShader.use();

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH_ / (float)SCR_HEIGHT_, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    ourShader.setMat4("projection", projection);
    ourShader.setMat4("view", view);
    ourShader.setVec3("viewPos", camera.Position);
    ourShader.setVec3("lightDirection", lightDir);

    glm::mat4 modelMatrix = glm::mat4(1.0f);

    modelMatrix = glm::translate(modelMatrix, worldPos);
    modelMatrix = glm::translate(modelMatrix, cameraPos);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
    ourShader.setMat4("model", modelMatrix);

    cube.Draw(ourShader);
}
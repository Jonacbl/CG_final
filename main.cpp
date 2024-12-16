#define _CRT_SECURE_NO_WARNINGS // SOS

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>

#include <learnopengl/model_animation.h>
#include <learnopengl/animator.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadCubemap(vector<std::string> faces);
void draw_cube(Shader& ourShader, Camera& camera, glm::vec3 lightDir, glm::vec3 worldPos, Model cube, bool ro = false);

// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;

// camera
Camera camera(glm::vec3(0.0f, 8.0f, 10.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

const glm::vec3 cameraPos = glm::vec3(0.0f, 1.0f, -10.0f);

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//parallel light
glm::vec3 lightDir(0.0f, -1.0f, -1.0f);


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Final", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader ourShader("shader/blinn-phong.vs", "shader/blinn-phong.fs");
    Shader aniShader("shader/ani_shader.vs", "shader/ani_shader.fs");
    Shader skyboxShader("shader/skybox.vs", "shader/skybox.fs");
    Shader groundShader("shader/ground.vs", "shader/ground.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float skyboxVertices[] = {
        // positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };

    // skybox VAO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // load textures
    // -------------
    std::vector<std::string> faces
    {
        "resources/textures/clouds/clouds1_east.bmp",   // 东
        "resources/textures/clouds/clouds1_west.bmp",   // 西
        "resources/textures/clouds/clouds1_down.bmp",   // 下
        "resources/textures/clouds/clouds1_up.bmp",     // 上
        "resources/textures/clouds/clouds1_north.bmp",  // 北
        "resources/textures/clouds/clouds1_south.bmp",  // 南
    };

    //std::vector<std::string> faces
    //{
    //    "resources/textures/park-skyboxes/Park2/posx.jpg",  // 右
    //    "resources/textures/park-skyboxes/Park2/negx.jpg",  // 左
    //    "resources/textures/park-skyboxes/Park2/negy.jpg",  // 下
    //    "resources/textures/park-skyboxes/Park2/posy.jpg",  // 上
    //    "resources/textures/park-skyboxes/Park2/posz.jpg",  // 前
    //    "resources/textures/park-skyboxes/Park2/negz.jpg"   // 后
    //};
    unsigned int cubemapTexture = loadCubemap(faces);

    // shader configuration
    // --------------------

    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    // load models
    // -----------
    //Model ourModel("resources/autumn-house/source/House_scene_01.fbx");
    Model ourModel("resources/abbey/scene.gltf");
    ourModel.CalculateCenter();
    
    Model table_and_chair("resources/table-and-chairs/source/Combo.dae");
    table_and_chair.CalculateCenter();

    Model bench("resources/bench-minecraft/source/model.gltf");
    bench.CalculateCenter();

    //Model rock("resources/rock/rock_base_LP.obj");
    //rock.CalculateCenter();

    Model bModel("resources/minecraft_chest/scene.gltf");
    Animation danceAnimation("resources/minecraft_chest/scene.gltf", &bModel);
    Animator animator(&danceAnimation);
    
    //minecraft_cube
    Model grass_cube("resources/grass_cube/scene.gltf");
    Model stone_cube("resources/stone_cube/scene.gltf");
    Model wood_cube("resources/wood_cube/scene.gltf");
    Model brick_cube("resources/brick_cube/scene.gltf");
    Model stone_brick_cube("resources/stone_brick_cube/scene.gltf");
    Model sand_cube("resources/sand_cube/scene.gltf");

    // Draw ground with grass cube
    unsigned int size = 500;    
    glm::mat4* groundModelMatrices;
    groundModelMatrices = new glm::mat4[size * size];
    for (auto i = 0; i < size * size; i++) {
        glm::mat4 model = glm::mat4(1.0f);
        // Calculate world position of the cube according to i
        int row = i / size;
        int col = i % size;
        model = glm::translate(model, glm::vec3(static_cast<float>(col) - static_cast<float>(size)/2.0f, -2.0f, static_cast<float>(row) - static_cast<float>(size) / 2.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

        groundModelMatrices[i] = model;
    }
    // configure instanced array of ground
    // -------------------------
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, size * size * sizeof(glm::mat4), &groundModelMatrices[0], GL_STATIC_DRAW);
    for (unsigned int i = 0; i < grass_cube.meshes.size(); i++)
    {
        unsigned int VAO = grass_cube.meshes[i].VAO;
        glBindVertexArray(VAO);
        // set attribute pointers for matrix (4 times vec4)
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);
        glBindVertexArray(0);
    }

    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);
		  //animator.UpdateAnimation(deltaTime/1.5f);
          animator.UpdateAnimation(deltaTime);
 
        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render a ground with `grass cube`
        {
            groundShader.use();
            glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            glm::mat4 view = camera.GetViewMatrix();
            groundShader.setMat4("projection", projection);
            groundShader.setMat4("view", view);
            groundShader.setVec3("viewPos", camera.Position);
            groundShader.setVec3("lightDirection", lightDir);
            groundShader.setInt("texture_diffuse1", 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, grass_cube.textures_loaded[0].id);
            for (unsigned int i = 0; i < grass_cube.meshes.size(); i++)
            {
                glBindVertexArray(grass_cube.meshes[i].VAO);
                glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(grass_cube.meshes[i].indices.size()), GL_UNSIGNED_INT, 0, size * size);
                glBindVertexArray(0);
            }
        }

        //static model of House
        //{
        //    ourShader.use();

        //    // view/projection transformations
        //    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //    glm::mat4 view = camera.GetViewMatrix();
        //    ourShader.setMat4("projection", projection);
        //    ourShader.setMat4("view", view);
        //    ourShader.setVec3("viewPos", camera.Position);
        //    ourShader.setVec3("lightDirection", lightDir);

        //    // render the loaded model
        //    glm::mat4 model = glm::mat4(1.0f);
        //    model = glm::translate(model, glm::vec3(1.07109f, -5.22503f, 3.59047f)); // translate it to the center
        //    model = glm::translate(model, glm::vec3(0.0f, 1.0f, -50.0f)); // far away from camera
        //    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //rotate
        //    model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));	// it's a bit too big for our scene, so scale it down

        //    ourShader.setMat4("model", model);
        //    ourModel.Draw(ourShader);

        //}

        //abbey
        {
            ourShader.use();

            // view/projection transformations
            glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 125.0f);
            glm::mat4 view = camera.GetViewMatrix();
            ourShader.setMat4("projection", projection);
            ourShader.setMat4("view", view);
            ourShader.setVec3("viewPos", camera.Position);
            ourShader.setVec3("lightDirection", lightDir);

            // render the loaded model
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.00898295f, -1.0f + 0.0819152f, 0.346726f)); // translate it to the center
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, -100.0f)); // far away from camera
            model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); //rotate
            model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //rotate
            model = glm::scale(model, glm::vec3(75.0f, 75.0f, 75.0f));	// it's a bit too big for our scene, so scale it down

            ourShader.setMat4("model", model);
            ourModel.Draw(ourShader);

        }

        //table and chairs
        {
            ourShader.use();

            // view/projection transformations
            glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            glm::mat4 view = camera.GetViewMatrix();
            ourShader.setMat4("projection", projection);
            ourShader.setMat4("view", view);
            ourShader.setVec3("viewPos", camera.Position);
            ourShader.setVec3("lightDirection", lightDir);

            // render the loaded model
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.672699f, 3.19014f, -1.056f)); // translate it to the center
            model = glm::translate(model, glm::vec3(-30.0f, -3.0f, 10.0f)); // switch to proper place
            model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //rotate
            model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); //rotate
            model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down

            ourShader.setMat4("model", model);
            table_and_chair.Draw(ourShader);

        }

        // bench
        //{
        //    ourShader.use();

        //    // view/projection transformations
        //    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //    glm::mat4 view = camera.GetViewMatrix();
        //    ourShader.setMat4("projection", projection);
        //    ourShader.setMat4("view", view);
        //    ourShader.setVec3("viewPos", camera.Position);
        //    ourShader.setVec3("lightDirection", lightDir);

        //    // render the loaded model
        //    glm::mat4 model = glm::mat4(1.0f);
        //    model = glm::translate(model, glm::vec3(0.672699f, 3.19014f, -1.056f)); // translate it to the center
        //    model = glm::translate(model, glm::vec3(-30.0f, -3.0f, 30.0f)); // switch to proper place
        //    //model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //rotate
        //    //model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); //rotate
        //    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down

        //    ourShader.setMat4("model", model);
        //    bench.Draw(ourShader);

        //}

        //rock
        //{
        //    ourShader.use();

        //    // view/projection transformations
        //    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //    glm::mat4 view = camera.GetViewMatrix();
        //    ourShader.setMat4("projection", projection);
        //    ourShader.setMat4("view", view);
        //    ourShader.setVec3("viewPos", camera.Position);
        //    ourShader.setVec3("lightDirection", lightDir);

        //    // render the loaded model
        //    glm::mat4 model = glm::mat4(1.0f);
        //    model = glm::translate(model, glm::vec3(-1.20083f, -0.134133f, 1.93921f)); // translate it to the center
        //    model = glm::translate(model, glm::vec3(-30.0f, -2.0f, -0.0f)); // switch to proper place
        //    //model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //rotate
        //    //model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); //rotate
        //    model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));	// it's a bit too big for our scene, so scale it down

        //    ourShader.setMat4("model", model);
        //    rock.Draw(ourShader);

        //}


         //chest_animation
        {
            aniShader.use();

            // view/projection transformations
            glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            glm::mat4 view = camera.GetViewMatrix();
            aniShader.setMat4("projection", projection);
            aniShader.setMat4("view", view);
            aniShader.setVec3("lightDirection", lightDir);

            auto transforms = animator.GetFinalBoneMatrices();
            for (int i = 0; i < transforms.size(); ++i)
                aniShader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);

            // render the loaded model
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 0.3f, 0.0f)); // higher
            model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // rotate some degrees around the Y axis
            model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f)); // scale it down
            aniShader.setMat4("model", model);
            bModel.Draw(aniShader);
        }

        // draw skybox as last
        {
            glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
            skyboxShader.use();
            glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
            skyboxShader.setMat4("view", view);
            skyboxShader.setMat4("projection", projection);
            // skybox cube
            glBindVertexArray(skyboxVAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
            glDepthFunc(GL_LESS); // set depth function back to default
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime * 10.0f);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime * 10.0f);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime * 10.0f);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime * 10.0f);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime * 10.0f);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime * 10.0f);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front)
// -Z (back)
// -------------------------------------------------------
unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

/// @brief Draw a cube, at the given position, read README for more details
/// @param ourShader shader
/// @param camera    camera
/// @param lightDir  light direction
/// @param worldPos  world position of to draw the model
/// @param cube      cube model
/// @param ro        a control flag, by default false
void draw_cube(Shader& ourShader, Camera& camera, glm::vec3 lightDir, glm::vec3 worldPos, Model cube, bool ro) {
    ourShader.use();

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    ourShader.setMat4("projection", projection);
    ourShader.setMat4("view", view);
    ourShader.setVec3("viewPos", camera.Position);
    ourShader.setVec3("lightDirection", lightDir);

    glm::mat4 modelMatrix = glm::mat4(1.0f);

    modelMatrix = glm::translate(modelMatrix, worldPos);
    modelMatrix = glm::translate(modelMatrix, camera.Position);
    if (ro) {
        modelMatrix = glm::rotate(modelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    }
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
    ourShader.setMat4("model", modelMatrix);

    cube.Draw(ourShader);
}
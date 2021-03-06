// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace glm;

#include <common/shader.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>
#include <common/texture.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "../common/stb_image.h"
//#include "tools/Object.cpp"
#include "tools/InputManager.cpp"

//void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

/*******************************************************************************/

int main( void )
{
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 1024, 768, "TP3 - GLFW", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    //  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);

    // Dark blue background
    glClearColor(0.f, 0.f, 0.f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    //glEnable(GL_CULL_FACE);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders( "vertex_shader.glsl", "fragment_shader.glsl" );

    /*****************TODO***********************/
    // Get a handle for our "Model View Projection" matrices uniforms
    glm::mat4 modelMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    // --------------------------------------------------------------------------------------------
    // |                                      TEXTURES                                            |
    // --------------------------------------------------------------------------------------------

    GLuint sun_texture = loadBMP_custom("textures/sunTexture.bmp");
    GLuint earth_texture = loadBMP_custom("textures/earthTexture.bmp");
    GLuint moon_texture = loadBMP_custom("textures/moonTexture.bmp");
    GLuint galaxy_texture = loadBMP_custom("textures/galaxyTexture.bmp");

    GLuint TextureID = glGetUniformLocation(programID,"texSampler");

    // Get a handle for our "LightPosition" uniform
    glUseProgram(programID);
    GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

    GLint modelID = glGetUniformLocation(programID, "modelMatrix");

    // For speed computation
    double lastTime = glfwGetTime();
    int nbFrames = 0;

    // --------------------------------------------------------------------------------------------
    // |                                         SCENE                                            |
    // --------------------------------------------------------------------------------------------

    Scene *scene = new Scene();
    InputManager *inputManager = new InputManager(window, scene);
    CAMERA camera = inputManager->scene->cameras[0]; 

    unique_ptr<Object> galaxy_uniquePtr = make_unique<Object>("sphere", 20., galaxy_texture, TextureID, modelID);
    Object* galaxy = galaxy_uniquePtr.get();
    
    unique_ptr<Object> sun_uniquePtr = make_unique<Object>("sphere", 20., sun_texture, TextureID, modelID);
    Object* sun = sun_uniquePtr.get();
    
    unique_ptr<Object> earth_uniquePtr = make_unique<Object>("sphere", 20., earth_texture, TextureID, modelID);
    Object* earth = earth_uniquePtr.get();
    
    unique_ptr<Object> moon_uniquePtr = make_unique<Object>("sphere", 20., moon_texture, TextureID, modelID);
    Object* moon = moon_uniquePtr.get();

    sun->addChild(move(earth_uniquePtr));
    earth->addChild(move(moon_uniquePtr));
    
    galaxy->transform->setLocalScale(vec3( 50, 50, 50 ));
    sun->transform->setLocalScale(vec3( 4, 4, 4 ));
    earth->transform->setLocalTranslation(vec3(5, 0., 0.));
    earth->transform->setLocalScale(vec3( 0.3, 0.3, 0.3 ));
    moon->transform->setLocalTranslation(vec3(2, 0., 0.));
    moon->transform->setLocalScale(vec3( 0.3, 0.3, 0.3 ));

    scene->addObject(move(galaxy));
    scene->addObject(move(sun));
    scene->addObject(move(earth));
    scene->addObject(move(moon));

    do{

        // Measure speed
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        inputManager->processInput(deltaTime);
        CAMERA camera = inputManager->scene->cameras[0];

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);

        sunRotation += 40. * deltaTime;
        earthRotation += 60. * deltaTime;
        moonRotation += 60. * deltaTime;
        galaxyRotation -= 5. * deltaTime;

        //glUniformMatrix4fv(selfModelID, 1, GL_FALSE, &sun->transform->getSelfModelMatrix()[0][0]);

        galaxy->transform->setLocalRotation(vec3(0., galaxyRotation, 0.));
        sun->transform->setLocalRotation(vec3(90., sunRotation, 0.));
        earth->transform->setLocalRotation(vec3(180., -23.44, earthRotation));
        moon->transform->setLocalRotation(vec3(180., 6.68, moonRotation));

        sun->updateSelfAndChild();
        galaxy->updateSelfAndChild();

        scene->draw();

        //-------------------------------------------------------------------------------------------------

        glm::mat4 viewMatrix = glm::lookAt(camera.position, camera.position + camera.target, camera.up);

        viewMatrix = glm::rotate(viewMatrix, glm::radians(rotationX), glm::vec3(1, 0, 0));
        viewMatrix = glm::rotate(viewMatrix, glm::radians(rotationY), glm::vec3(0, 1, 0));
        viewMatrix = glm::rotate(viewMatrix, glm::radians(rotationZ), glm::vec3(0, 0, 1));
 
        glm::mat4 projectionMatrix = glm::perspective<float>(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.f);
        
        GLint viewID = glGetUniformLocation(programID, "viewMatrix");
        GLint projectionID = glGetUniformLocation(programID, "projectionMatrix");

        //glUniformMatrix4fv(modelID, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniformMatrix4fv(viewID, 1, GL_FALSE, &viewMatrix[0][0]);
        glUniformMatrix4fv(projectionID, 1, GL_FALSE, &projectionMatrix[0][0]);

  /*    glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);*/
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    // Cleanup VBO and shader
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VertexArrayID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}


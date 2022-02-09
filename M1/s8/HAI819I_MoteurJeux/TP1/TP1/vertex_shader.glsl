#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertices_position_modelspace;
//layout (location = 2) in vec2 aTexCoord;

//TODO create uniform transformations matrices Model View Projection
// Values that stay constant for the whole mesh.

//uniform mat4 transfo_mat;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

//out vec3 ourColor;
//out vec2 TexCoord;

void main(){

        // TODO : Output position of the vertex, in clip space : MVP * position
        gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertices_position_modelspace,1);
        //TexCoord = aTexCoord;
}


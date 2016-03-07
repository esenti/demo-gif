#define GLEW_STATIC
#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <chrono>
#include <cmath>
#include <vector>

GLuint createShader(std::string filename, GLenum shaderType)
{
    GLuint shader = glCreateShader(shaderType);

    std::ifstream sourceFile(filename);
    std::stringstream source;
    source << sourceFile.rdbuf();
    std::string temp = source.str();
    const char* sourcePtr = temp.c_str();

    std::cout << filename << std::endl;

    glShaderSource(shader, 1, &sourcePtr, NULL);
    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if(status == GL_TRUE) {
        std::cout << "noice" << std::endl;
    } else {
        char buffer[512];
        glGetShaderInfoLog(shader, 512, NULL, buffer);
        std::cout << buffer << std::endl;
    }

    return shader;
}

struct Mesh
{
    std::vector<float> vertexData;
    std::vector<unsigned int> elements;
};

Mesh buildSphereMesh(int M, int N, int V)
{
    std::vector<float> vertexData;
    std::vector<unsigned int> elements;


    vertexData.reserve((V + 1) * N * 6);

    for(int m = 0; m <= V; ++m)
    {
        for(int n = 0; n < N; ++n)
        {
            glm::vec3 vertex(
                sin(M_PI * (m / (float)M)) * cos(2.0f * M_PI * (n / (float)N)),
                sin(M_PI * (m / (float)M)) * sin(2.0f * M_PI * (n / (float)N)),
                cos(M_PI * (m / (float)M))
            );

            glm::vec3 normal = glm::normalize(vertex);

            vertexData.push_back(vertex.x);
            vertexData.push_back(vertex.y);
            vertexData.push_back(vertex.z);
            vertexData.push_back(normal.x);
            vertexData.push_back(normal.y);
            vertexData.push_back(normal.z);
        }
    }

    for(int i = 0; i < (V + 1) * N - N; ++i)
    {
        if((i + 1) % N)
        {
            elements.push_back(i);
            elements.push_back(i + N);
            elements.push_back(i + 1);
            elements.push_back(i + 1);
            elements.push_back(i + N);
            elements.push_back(i + 1 + N);
        }
        else
        {
            elements.push_back(i);
            elements.push_back(i + N);
            elements.push_back(i - (N - 1));
            elements.push_back(i - (N - 1));
            elements.push_back(i + N);
            elements.push_back(i + 1);
        }
    }

    float innerScale = 0.96f;

    for(int m = 0; m <= V; ++m)
    {
        for(int n = 0; n < N; ++n)
        {
            glm::vec3 vertex(
                sin(M_PI * (m / (float)M)) * cos(2.0f * M_PI * (n / (float)N)),
                sin(M_PI * (m / (float)M)) * sin(2.0f * M_PI * (n / (float)N)),
                cos(M_PI * (m / (float)M))
            );

            glm::vec3 normal = -1.0f * glm::normalize(vertex);

            vertexData.push_back(innerScale * vertex.x);
            vertexData.push_back(innerScale * vertex.y);
            vertexData.push_back(innerScale * vertex.z);
            vertexData.push_back(normal.x);
            vertexData.push_back(normal.y);
            vertexData.push_back(normal.z);
        }
    }

    for(int i = (V + 1) * N; i < 2 * ((V + 1) * N) - N; ++i)
    {
        if((i + 1) % N)
        {
            elements.push_back(i);
            elements.push_back(i + 1);
            elements.push_back(i + N);
            elements.push_back(i + 1);
            elements.push_back(i + 1 + N);
            elements.push_back(i + N);
        }
        else
        {
            elements.push_back(i);
            elements.push_back(i - (N - 1));
            elements.push_back(i + N);
            elements.push_back(i + N);
            elements.push_back(i - (N - 1));
            elements.push_back(i + 1);
        }
    }

    for(int i = 0; i < N; ++i)
    {
        // elements.push_back((V + 1) * N - 1 - i);
        // elements.push_back((V + 1) * N - 2 - i);
        // elements.push_back(2 * ((V + 1) * N) - 1 - i);
        // elements.push_back((V + 1) * N - 2 - i);
        // elements.push_back(2 * ((V + 1) * N) - 1 - i);
        // elements.push_back(2 * ((V + 1) * N) - 2 - i);

        vertexData.push_back(vertexData[6 * ((V + 1) * N - 1 - i)]);
        vertexData.push_back(vertexData[6 * ((V + 1) * N - 1 - i) + 1]);
        vertexData.push_back(vertexData[6 * ((V + 1) * N - 1 - i) + 2]);
        vertexData.push_back(0.0f);
        vertexData.push_back(0.0f);
        vertexData.push_back(-1.0f);
        vertexData.push_back(vertexData[6 * ((V + 1) * N - 1 - ((i + 1) % N))]);
        vertexData.push_back(vertexData[6 * ((V + 1) * N - 1 - ((i + 1) % N)) + 1]);
        vertexData.push_back(vertexData[6 * ((V + 1) * N - 1 - ((i + 1) % N)) + 2]);
        vertexData.push_back(0.0f);
        vertexData.push_back(0.0f);
        vertexData.push_back(-1.0f);
        vertexData.push_back(vertexData[6 * (2 * ((V + 1) * N) - 1 - i)]);
        vertexData.push_back(vertexData[6 * (2 * ((V + 1) * N) - 1 - i) + 1]);
        vertexData.push_back(vertexData[6 * (2 * ((V + 1) * N) - 1 - i) + 2]);
        vertexData.push_back(0.0f);
        vertexData.push_back(0.0f);
        vertexData.push_back(-1.0f);

        elements.push_back(vertexData.size() / 6 - 3);
        elements.push_back(vertexData.size() / 6 - 2);
        elements.push_back(vertexData.size() / 6 - 1);

        vertexData.push_back(vertexData[6 * ((V + 1) * N - 1 - ((i + 1) % N))]);
        vertexData.push_back(vertexData[6 * ((V + 1) * N - 1 - ((i + 1) % N)) + 1]);
        vertexData.push_back(vertexData[6 * ((V + 1) * N - 1 - ((i + 1) % N)) + 2]);
        vertexData.push_back(0.0f);
        vertexData.push_back(0.0f);
        vertexData.push_back(-1.0f);
        vertexData.push_back(vertexData[6 * (2 * ((V + 1) * N) - 1 - ((i + 1) % N))]);
        vertexData.push_back(vertexData[6 * (2 * ((V + 1) * N) - 1 - ((i + 1) % N)) + 1]);
        vertexData.push_back(vertexData[6 * (2 * ((V + 1) * N) - 1 - ((i + 1) % N)) + 2]);
        vertexData.push_back(0.0f);
        vertexData.push_back(0.0f);
        vertexData.push_back(-1.0f);
        vertexData.push_back(vertexData[6 * (2 * ((V + 1) * N) - 1 - i)]);
        vertexData.push_back(vertexData[6 * (2 * ((V + 1) * N) - 1 - i) + 1]);
        vertexData.push_back(vertexData[6 * (2 * ((V + 1) * N) - 1 - i) + 2]);
        vertexData.push_back(0.0f);
        vertexData.push_back(0.0f);
        vertexData.push_back(-1.0f);

        elements.push_back(vertexData.size() / 6 - 3);
        elements.push_back(vertexData.size() / 6 - 2);
        elements.push_back(vertexData.size() / 6 - 1);
    }

    Mesh mesh;

    mesh.vertexData = vertexData;
    mesh.elements = elements;

    return mesh;
}

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    SDL_Window* window = SDL_CreateWindow("OpenGL", 100, 100, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(window);

    glewExperimental = GL_TRUE;
    glewInit();

    float vertices[] = {
         -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
         -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
         1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
         1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

         1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
         1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f,

         1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f,
         1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f,
         -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f,
         -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f,

         -1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
         -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
         -1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
         -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,

         -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
         -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f,
         1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f,

         -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
         -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
         1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
         1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };

    // GLuint elements[] = {
    //     0, 1, 2,
    //     2, 3, 0,
    //
    //     4, 5, 6,
    //     6, 7, 4,
    //
    //     8, 9, 10,
    //     10, 11, 8,
    //
    //     12, 13, 14,
    //     14, 15, 12,
    //
    //     16, 17, 18,
    //     18, 19, 16,
    //
    //     20, 21, 22,
    //     22, 23, 20,
    // };

    GLuint elements[] = {
        8, 9, 8 + 8,
        9, 8 + 8, 9 + 8,
    };

    GLuint vertexShader = createShader("shader.vert", GL_VERTEX_SHADER);
    GLuint fragmentShader = createShader("shader.frag", GL_FRAGMENT_SHADER);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    GLuint vbo;
    glGenBuffers(1, &vbo);

    GLuint ebo;
    glGenBuffers(1, &ebo);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    Mesh sphere = buildSphereMesh(32, 64, 16);

    std::cout << sphere.vertexData.size() / 6 << std::endl;

    for(int i = 0; i < sphere.vertexData.size(); i += 6)
    {
        std::cout << i / 6 << ": " << sphere.vertexData[i] << ", " << sphere.vertexData[i + 1] << ", " << sphere.vertexData[i + 2] << ", " << std::endl;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * sphere.vertexData.size(), sphere.vertexData.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * sphere.elements.size(), sphere.elements.data(), GL_STATIC_DRAW);

    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

    GLint normalAttrib = glGetAttribLocation(shaderProgram, "normal");
    glEnableVertexAttribArray(normalAttrib);
    glVertexAttribPointer(normalAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    glm::mat4 model;

    GLint uniformModel = glGetUniformLocation(shaderProgram, "M");
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

    float lightPos[] = { 5.0, 5.0, 5.0 };
    GLint uniformLight = glGetUniformLocation(shaderProgram, "lightPos");
    glUniform3fv(uniformLight, 1, lightPos);

    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 5.0f, 5.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    GLint uniformView = glGetUniformLocation(shaderProgram, "V");
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 1.0f, 10.0f);;

    GLint uniformProjection = glGetUniformLocation(shaderProgram, "P");
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

    GLint stepLocation = glGetUniformLocation(shaderProgram, "timeStep");
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    auto startTime = std::chrono::high_resolution_clock::now();
    SDL_Event windowEvent;
    while(true)
    {
        if(SDL_PollEvent(&windowEvent))
        {
            if(windowEvent.type == SDL_QUIT)
            {
                break;
            }
        }

        auto now = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration_cast<std::chrono::duration<float>>(now - startTime).count();

        float step = sin(time) + 1.0f;

        glUniform1f(stepLocation, step);
        model = glm::rotate(glm::mat4(), step * glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(glm::mat4(), step * glm::radians(90.0f) - 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        glClearColor(0.0, 0.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, sphere.elements.size(), GL_UNSIGNED_INT, 0);
        // glDrawArrays(GL_LINE_STRIP, 0, sphere.vertexData.size() / 6);
        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(context);
    SDL_Quit();
    return 0;
}

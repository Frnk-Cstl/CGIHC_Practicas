#include <stdio.h>
#include <string.h>
#include <cstdlib>    // rand()
#include <ctime>      // time()
#include <glew.h>
#include <glfw3.h>

// Dimensiones de la ventana
const int WIDTH = 800, HEIGHT = 800;
GLuint shader;

// VAOs y VBOs para letras
GLuint VAO_A, VBO_A;  // A
GLuint VAO_B, VBO_B;  // T
GLuint VAO_C, VBO_C;  // S
GLuint VAO_D, VBO_D;  // I

// Shaders GLSL
static const char* vShader = R"(
#version 330
layout (location =0) in vec3 pos;
void main() {
    gl_Position = vec4(pos, 1.0f);
}
)";

static const char* fShader = R"(
#version 330
out vec4 color;
void main() {
    // Color fijo para las letras
    color = vec4(0.9f, 0.1f, 0.7f, 1.0f); 
}
)";

// Función genérica para crear VAO de una figura
void CrearFigura(GLuint& VAO, GLuint& VBO, GLfloat* vertices, int size) {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// Crear letras con triángulos
void CrearLetras() {
    // Letra A
    GLfloat verticesA[] = {
        -0.8f, -0.5f, 0.0f,  -0.6f,  -0.5f, 0.0f,  -0.6f, 0.5f, 0.0f, // triángulo 1 lado izquierdo
        -0.8f, -0.5f, 0.0f,  -0.8f,  0.5f, 0.0f,  -0.6f, 0.5f, 0.0f, // triángulo 2 lado izquierdo
        -0.6f, 0.5f, 0.0f, -0.6f, 0.4f, 0.0f, -0.4f,  0.5f, 0.0f, // barra central arriba 1
       -0.4f, 0.5f, 0.0f,  -0.4f, 0.4f, 0.0f,  -0.6f, 0.4f, 0.0f, // barra central arriba 2
       -0.6f, 0.0f, 0.0f, -0.6f, -0.1f, 0.0f, -0.4f,  0.0f, 0.0f, //barra central abajo 1
       -0.6f, -0.1f, 0.0f, -0.4f, -0.1f, 0.0f, -0.4f,  0.0f, 0.0f,  //barra central abajo 2
       -0.4f, -0.5f, 0.0f, -0.2f, -0.5f, 0.0f, -0.2f,  0.5f, 0.0f,  // triángulo 1 lado derecho
       -0.4f, -0.5f, 0.0f, -0.4f, 0.5f, 0.0f, -0.2f,  0.5f, 0.0f,   // Triángulo 2 lado derecho
    };
    CrearFigura(VAO_A, VBO_A, verticesA, sizeof(verticesA));

    // Letra T
    GLfloat verticesB[] = {
        -0.18f, 0.5f, 0.0f, -0.18f,  0.3f, 0.0f, 0.1f,  0.5f, 0.0f,  // Parte de arriba 1
        -0.18f,  0.3f, 0.0f, 0.1f,  0.5f, 0.0f, 0.1f,  0.3f, 0.0f,   // Parte de arriba 2
        -0.09f,  0.3f, 0.0f, 0.01f, 0.3f, 0.0f, -0.09f, -0.5f, 0.0f, // Base 1
        -0.09f,  -0.5f, 0.0f, 0.01f, 0.3f, 0.0f, 0.01f, -0.5f, 0.0f  // Base 2
    };
    CrearFigura(VAO_B, VBO_B, verticesB, sizeof(verticesB));

    // Letra S
    GLfloat verticesC[] = {
         0.12f, 0.5f, 0.0f,  0.5f,  0.5f, 0.0f,  0.5f,  0.3f, 0.0f,  // Parte de arriba 1
         0.12f, 0.5f, 0.0f,  0.12f, 0.0f, 0.0f,  0.31f,  0.0f, 0.0f, // Parte del centro 1 izq
         0.31f, 0.0f, 0.0f,  0.5f,  0.0f, 0.0f,  0.5f,  -0.5f, 0.0f,  // Parte del centro 1 der
         0.12f, -0.5f, 0.0f,  0.12f, -0.3f, 0.0f,  0.5f,  -0.5f, 0.0f,// Parte de abajo 1
         0.12f, 0.5f, 0.0f,  0.12f,  0.3f, 0.0f,  0.5f,  0.3f, 0.0f,   // Parte de arriba 2
         0.12f, 0.5f, 0.0f,  0.31f, 0.5f, 0.0f,  0.31f,  0.0f, 0.0f,  // Parte del centro 2 der
         0.31f, 0.0f, 0.0f,  0.31f,  -0.5f, 0.0f,  0.5f,  -0.5f, 0.0f,  // Parte del centro 2 izq
         0.12f, -0.3f, 0.0f,  0.5f, -0.3f, 0.0f,  0.5f,  -0.5f, 0.0f   // Parte de abajo 2
    };
    CrearFigura(VAO_C, VBO_C, verticesC, sizeof(verticesC));
     // Letra I 
    GLfloat verticesD[] = {
        0.52f, 0.5f, 0.0f,   0.8f, 0.5f, 0.0f,   0.8f, 0.3f, 0.0f,  // triángulo 1 arriba
        0.52f, 0.5f, 0.0f,   0.52f,  0.3f, 0.0f,   0.8f, 0.3f, 0.0f,  // triángulo 2 arriba
        0.61f, 0.3f, 0.0f,    0.7f, 0.3f, 0.0f,   0.7f, -0.4f, 0.0f,  // barra central izq
        0.61f, 0.3f, 0.0f,    0.7f, -0.4f, 0.0f,   0.61f, -0.4f, 0.0f,  // barra central der
        0.52f, -0.3f, 0.0f,    0.52f, -0.5f, 0.0f,   0.8f, -0.3f, 0.0f, // triángulo 1 abajo
        0.52f, -0.5f, 0.0f,   0.8f, -0.3f, 0.0f,   0.8f, -0.5f, 0.0f, // triángulo 2 abajo
    };
    CrearFigura(VAO_D, VBO_D, verticesD, sizeof(verticesD));

}

// Compilar shaders
void AddShader(GLuint program, const char* code, GLenum type) {
    GLuint shader = glCreateShader(type);
    const GLchar* src[1] = { code };
    GLint length[1] = { (GLint)strlen(code) };
    glShaderSource(shader, 1, src, length);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        printf("Error compilando shader: %s\n", infoLog);
    }
    glAttachShader(program, shader);
}

void CompileShaders() {
    shader = glCreateProgram();
    if (!shader) { printf("Error creando programa\n"); return; }

    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);

    glLinkProgram(shader);
    GLint success;
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetProgramInfoLog(shader, 1024, NULL, infoLog);
        printf("Error linkeando: %s\n", infoLog);
    }
}

int main() {
    srand((unsigned int)time(0)); // inicializar random

    if (!glfwInit()) {
        printf("Falló inicialización de GLFW");
        glfwTerminate();
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Ventana", NULL, NULL);
    if (!mainWindow) {
        printf("Falló creación de ventana");
        glfwTerminate();
        return 1;
    }

    int BufferWidth, BufferHeight;
    glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);
    glfwMakeContextCurrent(mainWindow);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        printf("Falló inicialización de GLEW");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    glViewport(0, 0, BufferWidth, BufferHeight);

    CrearLetras();
    CompileShaders();

    // Control de tiempo y color
    double lastTime = glfwGetTime();
    float r = 0.0f, g = 0.0f, b = 0.0f;

    while (!glfwWindowShouldClose(mainWindow)) {
        glfwPollEvents();

        double currentTime = glfwGetTime();
        if (currentTime - lastTime >= 2.0) { // Cada 2 segundos
            r = (float)rand() / RAND_MAX;
            g = (float)rand() / RAND_MAX;
            b = (float)rand() / RAND_MAX;
            lastTime = currentTime;
        }

        glClearColor(r, g, b, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);

        // Dibujar letras
        glBindVertexArray(VAO_A);
        glDrawArrays(GL_TRIANGLES, 0, 24);  // 24 vértices porque usé 8 triángulos para dibujar una A
        glBindVertexArray(0);

        glBindVertexArray(VAO_B);
        glDrawArrays(GL_TRIANGLES, 0, 12);  // 12 porque usé 4 triángulos para dibujar una T
        glBindVertexArray(0); 

        glBindVertexArray(VAO_C);
        glDrawArrays(GL_TRIANGLES, 0, 24);  // Construí la S con 8 triángulos en total
        glBindVertexArray(0);

        glBindVertexArray(VAO_D);
        glDrawArrays(GL_TRIANGLES, 0, 18);  // La I la construí con seis triángulos en total
        glBindVertexArray(0);

        glUseProgram(0);

        glfwSwapBuffers(mainWindow);
    }

    return 0;
}

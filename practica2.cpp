// main.cpp
// Dibuja letras SIAT (cada una con color distinto) - RENDER DE LETRAS COMENTADO
// Dibuja "casa": cubo rojo y pirámide de base cuadrada azul encima

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <glew.h>
#include <glfw3.h>

// glm
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

// Tus clases (deben existir en el proyecto)
#include "Mesh.h"
#include "MeshColor.h" // si la tienes separada
#include "Shader.h"
#include "Window.h"

const float PI = 3.14159265358979323846f;

Window mainWindow;
std::vector<Mesh*> meshList;           // meshes indexadas (cubo, pirámide)
std::vector<MeshColor*> meshColorList; // meshes con color por vértice (letras)
std::vector<Shader> shaderList;        // shaders por color y shaders de color por vértice

// rutas a shaders
static const char* vsSolid = "shaders/solid.vert";
static const char* fsRed = "shaders/red.frag";
static const char* fsGreen = "shaders/green.frag";
static const char* fsBlue = "shaders/blue.frag";
static const char* fsBrown = "shaders/brown.frag";
static const char* fsDarkGreen = "shaders/darkgreen.frag";

static const char* vsColor = "shaders/colorvert.vert";
static const char* fsColor = "shaders/colorfrag.frag";

// ---------- Funciones para crear geometría ----------

// Crea un cubo centrado en el origen, tamaño 1
void CrearCubo() {
    GLfloat vertices[] = {
        // 8 vértices (x,y,z)
        -0.5f, -0.5f,  0.5f,  // 0
         0.5f, -0.5f,  0.5f,  // 1
         0.5f,  0.5f,  0.5f,  // 2
        -0.5f,  0.5f,  0.5f,  // 3
        -0.5f, -0.5f, -0.5f,  // 4
         0.5f, -0.5f, -0.5f,  // 5
         0.5f,  0.5f, -0.5f,  // 6
        -0.5f,  0.5f, -0.5f   // 7
    };
    unsigned int indices[] = {
        0,1,2, 2,3,0,
        1,5,6, 6,2,1,
        7,6,5, 5,4,7,
        4,0,3, 3,7,4,
        4,5,1, 1,0,4,
        3,2,6, 6,7,3
    };

    Mesh* cubo = new Mesh();
    cubo->CreateMesh(vertices, indices, 24, 36);
    meshList.push_back(cubo); // índice 0
}

// Crea una pirámide de base cuadrada centrada en el origen (base en y = -0.5, apex en y = +0.5)
void CrearPiramideCuadrada() {
    // 5 vértices: 4 de la base, 1 apex
    GLfloat vertices[] = {
        -0.5f, -0.5f,  0.5f, // 0 base front-left
         0.5f, -0.5f,  0.5f, // 1 base front-right
         0.5f, -0.5f, -0.5f, // 2 base back-right
        -0.5f, -0.5f, -0.5f, // 3 base back-left
         0.0f,  0.5f,  0.0f  // 4 apex
    };
    // Triangulación: 4 caras laterales + 2 triángulos para la base
    unsigned int indices[] = {
        // base (2 triángulos)
        0,1,2,  2,3,0,
        // caras
        0,1,4,
        1,2,4,
        2,3,4,
        3,0,4
    };

    Mesh* pir = new Mesh();
    pir->CreateMesh(vertices, indices, 15, 18);
    meshList.push_back(pir); // índice 1
}

// Generar letras SIAT con MeshColor (cada vértice lleva RGB).
// Para simplicidad cada letra la hago con triángulos (a modo de ejemplo).
void CrearLetrasSIAT() {
    // Ejemplo simplificado: cada letra será un conjunto de triángulos (no tipografía real)
    // Letter S (azul) - 3 triángulos ejemplo
    GLfloat S_vertices[] = {
        // x, y, z,   r, g, b
        -0.9f,  0.4f, 0.0f,   0.0f, 0.0f, 1.0f,
        -0.1f,  0.4f, 0.0f,   0.0f, 0.0f, 1.0f,
        -0.5f,  0.1f, 0.0f,   0.0f, 0.0f, 1.0f
    };
    MeshColor* ms = new MeshColor();
    ms->CreateMeshColor(S_vertices, 18);
    meshColorList.push_back(ms);

    // Letter I (verde)
    GLfloat I_vertices[] = {
        -0.0f,  0.4f, 0.0f,   0.0f, 1.0f, 0.0f,
         0.1f,  0.4f, 0.0f,   0.0f, 1.0f, 0.0f,
         0.05f, -0.4f, 0.0f,  0.0f, 1.0f, 0.0f
    };
    MeshColor* mi = new MeshColor();
    mi->CreateMeshColor(I_vertices, 18);
    meshColorList.push_back(mi);

    // Letter A (brown / café)
    GLfloat A_vertices[] = {
        0.2f, -0.4f, 0.0f,   0.478f, 0.255f, 0.067f,
        0.6f, -0.4f, 0.0f,   0.478f, 0.255f, 0.067f,
        0.4f,  0.4f, 0.0f,   0.478f, 0.255f, 0.067f
    };
    MeshColor* ma = new MeshColor();
    ma->CreateMeshColor(A_vertices, 18);
    meshColorList.push_back(ma);

    // Letter T (dark green / verde oscuro)
    GLfloat T_vertices[] = {
        0.8f,  0.4f, 0.0f,   0.0f, 0.39f, 0.0f,
        1.4f,  0.4f, 0.0f,   0.0f, 0.39f, 0.0f,
        1.1f, -0.4f, 0.0f,   0.0f, 0.39f, 0.0f
    };
    MeshColor* mt = new MeshColor();
    mt->CreateMeshColor(T_vertices, 18);
    meshColorList.push_back(mt);
}

// Crear shaders: creamos un Shader por cada color sólido (red, green, blue, brown, darkgreen)
// y además un shader para MeshColor (vertex color).
void CreateShaders() {
    // shaders sólidos (vertex común + fragment fijo por color)
    Shader* sRed = new Shader();    sRed->CreateFromFiles(vsSolid, fsRed); shaderList.push_back(*sRed);
    Shader* sGreen = new Shader();  sGreen->CreateFromFiles(vsSolid, fsGreen); shaderList.push_back(*sGreen);
    Shader* sBlue = new Shader();   sBlue->CreateFromFiles(vsSolid, fsBlue); shaderList.push_back(*sBlue);
    Shader* sBrown = new Shader();  sBrown->CreateFromFiles(vsSolid, fsBrown); shaderList.push_back(*sBrown);
    Shader* sDark = new Shader();   sDark->CreateFromFiles(vsSolid, fsDarkGreen); shaderList.push_back(*sDark);

    // shader para mallas que traen color por vértice
    Shader* sColor = new Shader();  sColor->CreateFromFiles(vsColor, fsColor); shaderList.push_back(*sColor);
}

int main() {
    // Inicializa la ventana
    mainWindow = Window(1024, 768);
    mainWindow.Initialise();

    // Crear geometría
    CrearCubo();           // meshList[0]
    CrearPiramideCuadrada(); // meshList[1]
    CrearLetrasSIAT();     // meshColorList[0..3]

    // Crear shaders
    CreateShaders();
    // shaderList:
    // [0]=red, [1]=green, [2]=blue, [3]=brown, [4]=darkgreen, [5]=meshColor

    // Configurar proyección (perspectiva)
    glm::mat4 projection = glm::perspective(glm::radians(60.0f),
        (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(),
        0.1f, 100.0f);

    glEnable(GL_DEPTH_TEST);

    // Variables para ubicaciones de uniforms
    GLuint uniformModel = 0;
    GLuint uniformProjection = 0;

    // LOOP principal
    while (!mainWindow.getShouldClose()) {
        glfwPollEvents();
        glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // =========================
        // RENDER: LETRAS (COMENTADO)
        // =========================
        /*
        // usar shader de color por vértice
        shaderList[5].useShader();
        uniformModel = shaderList[5].getModelLocation();
        uniformProjection = shaderList[5].getProjectLocation();

        // posición para las letras (opcional)
        glm::mat4 modelL = glm::mat4(1.0f);
        modelL = glm::translate(modelL, glm::vec3(0.0f, 0.0f, -2.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelL));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

        // Dibujar las letras (descomentar para verlas)
        for (size_t i = 0; i < meshColorList.size(); ++i) {
            meshColorList[i]->RenderMeshColor();
        }
        */

        // =========================
        // RENDER: CASA (CUBO ROJO + PIRÁMIDE AZUL)
        // =========================

        // --- Cubo rojo: usamos shader rojo (shaderList[0]) ---
        shaderList[0].useShader();
        uniformModel = shaderList[0].getModelLocation();
        uniformProjection = shaderList[0].getProjectLocation();

        glm::mat4 model = glm::mat4(1.0f);
        // ubicar el cubo un poco más lejos en Z
        model = glm::translate(model, glm::vec3(0.0f, -0.5f, -3.5f)); // baja el cubo para que la pirámide quede encima
        // opcional: escalado si quieres un "cubo más grande": glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshList[0]->RenderMesh(); // cubo

        // --- Pirámide azul: shader azul (shaderList[2]) ---
        shaderList[2].useShader();
        uniformModel = shaderList[2].getModelLocation();
        uniformProjection = shaderList[2].getProjectLocation();

        glm::mat4 modelP = glm::mat4(1.0f);
        // ubicar la pirámide centrada sobre el cubo:
        // el cubo se extiende de y=-1.0 a y=0.0 (porque lo traslamos -0.5 en y y la mitad del cubo es 0.5).
        // para que la base de la pirámide descanse sobre la cara superior del cubo necesitamos ajustar:
        modelP = glm::translate(modelP, glm::vec3(0.0f, 0.0f, -3.5f)); // misma Z que el cubo
        // mover la pirámide hacia arriba la mitad del cubo + mitad de la pirámide:
        // cubo altura = 1.0 → su cara superior está en y = -0.5 + 1.0 = 0.5
        // pirámide base está en y = -0.5 local, apex en +0.5 local. Para que la base esté en y=0.5 global:
        modelP = glm::translate(modelP, glm::vec3(0.0f, 0.5f + 0.5f, 0.0f)); // sube para que base descanse sobre cubo
        // si el tamaño no encaja perfecto, ajusta estos valores.
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelP));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        meshList[1]->RenderMesh(); // pirámide

        // --- Fin render casa ---
        glUseProgram(0);
        mainWindow.swapBuffers();
    }

    return 0;
}


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL2/SOIL2.h>
#include <iostream>

#include "Skybox.h"
#include "Camera.h"
#include "Model.h"
#include "Shader.h"
#include "Mesa.cpp"
#include "Animaciones.cpp"

const GLuint WIDTH = 1093, HEIGHT = 615;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

//Variables globales
GLfloat deltaTime=0.0f;
//x z y
//Sale mirando hacia +x cpm +y a la derecha y z arriba
Camera camera(glm::vec3(-50.0f, 20.0f, 0.0f));

bool keys[1024];
GLdouble lastX = 400, lastY = 300;
bool firstMouse = true;

//DEFINIENDO ANIMACIONES
Animaciones::aPuerta animacion_puerta(&deltaTime);
Animaciones::aPuertaRefri animacion_puerta_refri(&deltaTime);
Animaciones::aGrifo animacion_grifo(&deltaTime);


using namespace std;
int main() {
    glfwInit();
    // Set all the required options for GLFW

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4); //ATIALIASING

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto final", nullptr, nullptr);
    // Disabling cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (nullptr == window){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_ALPHA_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);

    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

    GLfloat lastFrame = 0.0f;
    //CARGANDO SHADERS
    Shader material_shader("shaders/phong_texture.vs","shaders/phong_texture.frag");
    Shader SkyBoxshader("shaders/SkyBox.vs", "shaders/SkyBox.frag");
    //Skybox
    SkyBox fondo((char*)"SkyBox/derecha.jpg", (char*)"SkyBox/izquierda.jpg", (char*)"SkyBox/arriba.jpg",
        (char*)"SkyBox/abajo.jpg", (char*)"SkyBox/atras.jpg", (char*)"SkyBox/delante.jpg",
        skyboxVertices_1, sizeof(skyboxVertices_1));

    //CARGANDO MODELOS
    Model lamp((GLchar*)"models/lámpara.obj");
    Model oven((GLchar*)"models/estufa.obj");
    Model refri((GLchar*)"models/refri.obj");
    Model refri_puerta((GLchar*)"models/refri_puerta.obj");
    Model grifo((GLchar*)"models/mixer.obj");
    Model perilla_grifo((GLchar*)"models/fregadero_manija.obj");
    Model casa((GLchar*)"models/Casa.obj");
    Model pivote_couch((GLchar*)"models/couch.obj");
    Model silla((GLchar*)"models/silla.obj");
    Model mesa((GLchar*)"models/mesa.obj");
    Model puerta((GLchar*)"models/puerta.obj");
    Model isla((GLchar*)"models/isla.obj");
    Model taza((GLchar*)"models/taza.obj");
    
    //CARGANDO CUBOS PARA MODELADO EN TIEMPO DE EJECUCION
    Mesa mi_mesa=Mesa((GLchar*) "models/texturas/negro.jpg");
    Mesa mi_pata=Mesa((GLchar*)"models/texturas/blanco.jpg");

    // Game loop
    while (!glfwWindowShouldClose(window)){
        // Set frame time
        GLfloat currentFrame = (GLfloat)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();

        // Clear the colorbuffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //CONFIGURANDO VISTA
        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "projection"),
            1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "view"),
            1, GL_FALSE, glm::value_ptr(view));


        //CONFIGURANDO EL SHADER
        glUniform3f(glGetUniformLocation(material_shader.Program, "viewPos"),
            camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        glUniform3f(glGetUniformLocation(material_shader.Program, "light.ambient"), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(material_shader.Program, "light.diffuse"), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(material_shader.Program, "light.specular"), 0.1f, 0.1f, 0.1f);

        glUniform3f(glGetUniformLocation(material_shader.Program, "light.direction"), 1.0f, -1.0f, 0.0f);
        
        SkyBoxshader.Use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
        glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        fondo.Draw();

        material_shader.Use();
        glm::mat4 model(1);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(13.12, 2.20f, 9.14f));
        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        oven.Draw(material_shader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(13.22f, 5.34f, 0.73f));
        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        lamp.Draw(material_shader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(6.34f, 2.25f, 10.12f));
        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        refri.Draw(material_shader);
        
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(4.68f, 7.85f, 9.10f));
        animacion_puerta_refri.play(&model);
        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        refri_puerta.Draw(material_shader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(13.05f, 2.21f, 1.35f));
        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        grifo.Draw(material_shader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(14.0f, 5.52f, 2.22f));
        animacion_grifo.play(&model);
        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        perilla_grifo.Draw(material_shader);

        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.3f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        perilla_grifo.Draw(material_shader);

        glm::mat4 aux_model(1);
        model=glm::mat4(1);
        aux_model=model=glm::translate(model, glm::vec3(0.3521f, 2.6264f, 1.1447f));
        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        pivote_couch.Draw(material_shader);
        model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        pivote_couch.Draw(material_shader);
        model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        pivote_couch.Draw(material_shader);
        model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        pivote_couch.Draw(material_shader);
        model = aux_model;
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        pivote_couch.Draw(material_shader);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        pivote_couch.Draw(material_shader);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        pivote_couch.Draw(material_shader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, 4.49f, 7.99f));
        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        silla.Draw(material_shader);
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(7.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        silla.Draw(material_shader);
        
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-3.94f, 5.13f, 8.02f));
        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        mesa.Draw(material_shader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(11.94f, 2.63f, -6.8f));
        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        taza.Draw(material_shader);
        
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-14.93f, 2.28f, 3.77f));
        animacion_puerta.play(&model);
        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        puerta.Draw(material_shader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-5.83f, 3.47f, -6.15f));
        model = glm::scale(model, glm::vec3(2*2.45f, 2*0.2f, 2*3.8f));
        
        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        mi_mesa.draw();

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-4.1843f, 2.7264f, -9.5062f));
        model = glm::scale(model, glm::vec3(2 * 0.289f, 2 * 0.64f, 2 * 0.289f));

        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        mi_pata.draw();

        aux_model = model;
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 2*5.8f));
        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        mi_pata.draw();

        model = aux_model;
        model = glm::translate(model, glm::vec3(-2*3.09f, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        mi_pata.draw();

        model = aux_model;
        model = glm::translate(model, glm::vec3(-2*3.09f, 0.0f, 2*5.8f));
        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        mi_pata.draw();

        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        casa.Draw(material_shader);
        
        isla.Draw(material_shader);
        // Swap the buffers
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
    // Camera controls
    if (keys[GLFW_KEY_W])
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }

    if (keys[GLFW_KEY_S])
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    /*Agregando movimiento en el eje y*/

    if (keys[GLFW_KEY_UP])
    {
        camera.ProcessKeyboard(UP, deltaTime);
    }
    if (keys[GLFW_KEY_DOWN])
    {
        camera.ProcessKeyboard(DOWN, deltaTime);
    }
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action){
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024){
        if (action == GLFW_PRESS){
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE){
            keys[key] = false;
        }
    }
    if (keys[GLFW_KEY_1]) {
        animacion_puerta.running = true;
    }
    if (keys[GLFW_KEY_2]) {
        animacion_puerta_refri.running = true;
    }
    if (keys[GLFW_KEY_3]) {
        animacion_grifo.running = true;
    }
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse){
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL2/SOIL2.h>
#include <iostream>

#include <vector>
#include "Skybox.h"
#include "Camera.h"
#include "Model.h"
#include "Shader.h"
#include "Mesa.h"
#include "Animaciones.h"
#include "KeyFrameAnimation.h"
#include "AnimationRecorder.h"

const GLuint WIDTH = 400, HEIGHT = 400;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

//Variables globales
GLfloat deltaTime = 0.0f;
//x z y
//Sale mirando hacia +x cpm +y a la derecha y z arriba
Camera camera(glm::vec3(0.0f, 5.0f, 0.0f));

bool keys[1024];
GLdouble lastX = 400, lastY = 300;
bool firstMouse = true;

using namespace std;
//DEFINIENDO ANIMACIONES
Animaciones::aPuerta animacion_puerta(&deltaTime);
Animaciones::aPuertaRefri animacion_puerta_refri(&deltaTime);
Animaciones::aGrifo animacion_grifo(&deltaTime);
//DEFINIENDO ANIMACIONES COMPLEJAS
Keyframe::KeyFrameAnimation animacion_compleja_1(60);
Keyframe::KeyFrameAnimation animacion_compleja_2(120);

Keyframe::Frame frame;
AnimationRecoder anima_rec(&frame);

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
    if (nullptr == window) {
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
    //DEFNIENDO KEYFRAMES
    std::vector<Keyframe::Frame> frame_list = std::vector<Keyframe::Frame>();
    frame_list.reserve(3);
    /*
    frame_list.push_back(Keyframe::Frame(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f));
    frame_list.push_back(Keyframe::Frame(0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f));
    frame_list.push_back(Keyframe::Frame(0.0f, 8.0f, 0.0f, 0.0f, 0.0f, 0.0f));
    */

    //CARGANDO SHADERS
    Shader material_shader("shaders/phong_texture.vs", "shaders/phong_texture.frag");
    Shader SkyBoxshader("shaders/SkyBox.vs", "shaders/SkyBox.frag");
    //Skybox
    SkyBox fondo((char*)"SkyBox/derecha.jpg", (char*)"SkyBox/izquierda.jpg", (char*)"SkyBox/arriba.jpg",
        (char*)"SkyBox/abajo.jpg", (char*)"SkyBox/atras.jpg", (char*)"SkyBox/delante.jpg",
        skyboxVertices_1, sizeof(skyboxVertices_1));

    //CARGANDO MODELOS
    Model casa((GLchar*)"models/Casa.obj");
    //Model esfera((GLchar*)"models/esfera.obj");
    //Model refri((GLchar*)"models/refri.obj");
    Model lamp((GLchar*)"models/lámpara.obj");
    Model grifo((GLchar*)"models/mixer.obj");


    // Game loop
    while (!glfwWindowShouldClose(window)) {
        // Set frame time
        GLfloat currentFrame = (GLfloat)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        //printf("Delta time inside %f", deltaTime);
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

        /*
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(7.57f, 7.26f, 9.75f));
        model = glm::translate(model, glm::vec3(frame.posX,frame.posY, frame.posZ));
        model = glm::rotate(model, glm::radians(frame.rotX), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(frame.rotY), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(frame.rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
        animacion_compleja_1.play(&model);
        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        esfera.Draw(material_shader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(6.34f, 2.25f, 10.12f));
        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        refri.Draw(material_shader);
        */
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(13.05f, 2.21f, 1.35f));
        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "model"),
            1, GL_FALSE, glm::value_ptr(model));

        grifo.Draw(material_shader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(13.22f, 5.34f, 0.73f));
        model = glm::translate(model, glm::vec3(frame.posX, frame.posY, frame.posZ));
        model = glm::rotate(model, glm::radians(frame.rotX), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(frame.rotY), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(frame.rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
        animacion_compleja_1.play(&model);
        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        lamp.Draw(material_shader);

        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(material_shader.Program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        casa.Draw(material_shader);

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

    if (keys[GLFW_KEY_A])
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }

    if (keys[GLFW_KEY_D])
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    if (keys[GLFW_KEY_PAGE_UP])
    {
        camera.ProcessKeyboard(UP, deltaTime);
    }

    if (keys[GLFW_KEY_PAGE_DOWN])
    {
        camera.ProcessKeyboard(DOWN, deltaTime);
    }
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE) {
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
    if (keys[GLFW_KEY_4]) {
        animacion_compleja_1.start();
    }
    float speed = 10*deltaTime;

    if (keys[GLFW_KEY_UP]) {
        frame.posY += speed;
    }
    if (keys[GLFW_KEY_DOWN]) {
        frame.posY -= speed;
    }
    if (keys[GLFW_KEY_LEFT]) {
        frame.posZ -= speed;
    }
    if (keys[GLFW_KEY_RIGHT]) {
        frame.posZ += speed;
    }
    if (keys[GLFW_KEY_KP_8]) {
        frame.posX += speed;
    }
    if (keys[GLFW_KEY_KP_2]) {
        frame.posX -= speed;
    }
    if (keys[GLFW_KEY_KP_7]) {
        //if (frame.rotY<360)
            frame.rotY += 30 * speed;
    }
    if (keys[GLFW_KEY_KP_9]) {
        //if (frame.rotY > 0) 
            frame.rotY -= 30 * speed;
    }
    if (keys[GLFW_KEY_PERIOD]) {
        //if (frame.rotY < 360)
            frame.rotX += 30 * speed;
    }
    if (keys[GLFW_KEY_COMMA]) {
        //if (frame.rotX > 0)
            frame.rotX -= 30 * speed;
    }
    if (keys[GLFW_KEY_H]) {
        //if (frame.rotY < 360)
            frame.rotZ += 30 * speed;
    }
    if (keys[GLFW_KEY_Y]) {
       // if (frame.rotZ > 0)
            frame.rotZ -= 30 * speed;
    }
    if (keys[GLFW_KEY_K]) {
        puts("Agregado el frame");
        anima_rec.addFreame();
    }
    if (keys[GLFW_KEY_C]) {
        puts("Reiniciando grabacion");
        anima_rec.reiniciar();
    }
    if (keys[GLFW_KEY_P]) {
        anima_rec.print_all();
    }
    if (keys[GLFW_KEY_Z]) {
        puts("Desaciendo");
        anima_rec.delete_last();
    }
    if (keys[GLFW_KEY_L]) {
        puts("corriendo animacion");
        frame.posX = 0.0f;
        frame.posY = 0.0f;
        frame.posZ = 0.0f;
        frame.rotX = 0.0f;
        frame.rotY = 0.0f;
        frame.rotZ = 0.0f;
        if (anima_rec.frame_list.size() > 0) {
            animacion_compleja_1.setAnimation(anima_rec.frame_list);
            animacion_compleja_1.start();
        }
    }
    if (keys[GLFW_KEY_Q]) {
        puts("loading Last frame");
        anima_rec.last_frame();
    }
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse) {
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

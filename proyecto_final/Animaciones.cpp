#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace Animaciones {
    class aPuerta {
    private:
        float rotacion = 0.0f;
        GLfloat* deltatime;
        int estado = 1;
        int velocidad = 30;
    public:
        bool running = false;

        aPuerta(GLfloat* deltatime) {
            this->deltatime = deltatime;
        }
        void play(glm::mat4* model) {
            if (running) {
                switch (estado) {
                case 1:
                    if (rotacion < 90)
                        rotacion += velocidad * (*deltatime);
                    else {
                        puts("Cmbiando a estado 2");
                        estado = 2;
                    }

                    break;
                case 2:
                    if (rotacion > 0) {
                        rotacion -= velocidad * (*deltatime);
                        puts("LLendo en sentido contrario");
                    }
                        
                    else {
                        running = false;
                        estado = 1;
                    }
                    break;
                }
            }
            *model = glm::rotate(*model, glm::radians(rotacion), glm::vec3(0.0f, 1.0f, 0.0f));

        }
    };

    class aPuertaRefri {
    private:
        float rotacion = 0.0f;
        GLfloat* deltatime;
        int estado = 1;
        int velocidad = 30;
    public:
        bool running = false;

        aPuertaRefri(GLfloat* deltatime) {
            this->deltatime = deltatime;
        }
        void play(glm::mat4* model) {
            if (running) {
                switch (estado) {
                case 1:
                    if (rotacion < 150)
                        rotacion += velocidad * (*deltatime);
                    else {
                        puts("Cmbiando a estado 2");
                        estado = 2;
                    }

                    break;
                case 2:
                    if (rotacion > 0) {
                        rotacion -= velocidad * (*deltatime);
                        puts("LLendo en sentido contrario");
                    }

                    else {
                        running = false;
                        estado = 1;
                    }
                    break;
                }
            }
            *model = glm::rotate(*model, glm::radians(rotacion), glm::vec3(0.0f, 1.0f, 0.0f));

        }
    };
    class aGrifo {
    private:
        float rotacion = 0.0f;
        GLfloat* deltatime;
        int estado = 1;
        int velocidad = 30;
    public:
        bool running = false;

        aGrifo(GLfloat* deltatime) {
            this->deltatime = deltatime;
        }
        void play(glm::mat4* model) {
            if (running) {
                switch (estado) {
                case 1:
                    if (rotacion < 90)
                        rotacion += velocidad * (*deltatime);
                    else {
                        puts("Cmbiando a estado 2");
                        estado = 2;
                    }

                    break;
                case 2:
                    if (rotacion > 0) {
                        rotacion -= velocidad * (*deltatime);
                        puts("LLendo en sentido contrario");
                    }

                    else {
                        running = false;
                        estado = 1;
                    }
                    break;
                }
            }
            *model = glm::rotate(*model, glm::radians(rotacion), glm::vec3(0.0f, 0.0f, 1.0f));

        }
    };
};
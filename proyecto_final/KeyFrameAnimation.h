#pragma once
#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
using namespace  std;
namespace Keyframe{
    class Frame {
    public:
        //Variables para GUARDAR Key Frames
        float posX;
        float posY;
        float posZ;
        float incX;
        float incY;
        float incZ;
        float rotX;
        float rotXInc;
        float rotY;
        float rotYInc;
        float rotZ;
        float rotZInc;
        Frame(float _posX, float _posY, float _posZ,float _rotX, float _rotY, float _rotZ) {
            posX = _posX;
            posY = _posY;
            posZ = _posZ;
            rotX = _rotX;
            rotY = _rotY;
            rotZ = _rotZ;
        }
        Frame() {
            ;
        }
    };

    class KeyFrameAnimation {
    private:
        int i_max_steps;
        int i_curr_steps = 0;
        int playIndex = 0;
        GLfloat* deltatime;
    public:
        float posX, posY, posZ, incX, incY, incZ, rotX, rotXInc, rotY, rotYInc, rotZ, rotZInc;
        vector<Frame> KeyFrame;
        GLint FrameIndex = 0;
        bool running = false;

        KeyFrameAnimation(int _i_max_steps = 120) {
            KeyFrame = vector<Frame>();
            //Initializing the animation
            i_max_steps = _i_max_steps;
        }

        void start() {
            if (!running) {
                running = true;
                reset();
                interpolation();
            }
        }

        void setAnimation(vector<Frame> frame_list) {
            KeyFrame.clear();
            KeyFrame.reserve(frame_list.size());
            for (Frame new_frame : frame_list) {
                KeyFrame.push_back(new_frame);
            }
        }

        void resetElements(void)
        {
            posX = KeyFrame[0].posX;
            posY = KeyFrame[0].posY;
            posZ = KeyFrame[0].posZ;

            rotX = KeyFrame[0].rotX;
            rotY = KeyFrame[0].rotY;
            rotZ = KeyFrame[0].rotZ;
        }

        void interpolation(void) {

            KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
            KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
            KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

            KeyFrame[playIndex].rotXInc = (KeyFrame[playIndex + 1].rotX - KeyFrame[playIndex].rotX) / i_max_steps;
            KeyFrame[playIndex].rotYInc = (KeyFrame[playIndex + 1].rotY - KeyFrame[playIndex].rotY) / i_max_steps;
            KeyFrame[playIndex].rotZInc = (KeyFrame[playIndex + 1].rotZ - KeyFrame[playIndex].rotZ) / i_max_steps;
        }
        void reset() {
            playIndex = 0;
            i_curr_steps = 0;
            resetElements();
        }
        void play(glm::mat4* model) {
            //Movimiento del personaje
            if (running) {
                //cout <<"Index: "<< playIndex << "Frame: " << i_curr_steps << endl;
                if (i_curr_steps >= i_max_steps) {
                    playIndex++;
                    if (playIndex > KeyFrame.size() - 2)	//end of total animation?
                    {
                        //printf("termina anim\n");
                        playIndex = 0;
                        running = false;
                        reset();
                    }
                    else //Next frame interpolations
                    {
                        i_curr_steps = 0; //Reset counter
                                            //Interpolation
                        interpolation();
                    }

                }
                else {
                    //Draw animation
                    posX += KeyFrame[playIndex].incX;
                    posY += KeyFrame[playIndex].incY;
                    posZ += KeyFrame[playIndex].incZ;

                    rotX += KeyFrame[playIndex].rotXInc;
                    rotY += KeyFrame[playIndex].rotYInc;
                    rotZ += KeyFrame[playIndex].rotZInc;

                    i_curr_steps++;
                }
                /*
                printf("Movimiento en x:%f y:%f z:%f\n",posX,posY,posZ);
                printf("Rotacion en x:%f y:%f z:%f\n", rotX, rotY, rotZ);
                */
                *model = glm::translate(*model, glm::vec3(posX, posY, posZ));
                *model = glm::rotate(*model, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
                *model = glm::rotate(*model, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
                *model = glm::rotate(*model, glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
            }

        }
    };
};
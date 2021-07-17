#pragma once
#include "KeyFrameAnimation.h"
#include <vector>
using namespace std;
class AnimationRecoder {
public:
    Keyframe::Frame* frame;
    vector<Keyframe::Frame> frame_list;
    AnimationRecoder(Keyframe::Frame* _frame) {
        this->frame = _frame;
    }
    void addFreame() {
        Keyframe::Frame current = *this->frame;
        /*
        Keyframe::Frame frame(this->frame.posX, *this->frame.posY, *this->frame.posZ,
            *this->frame.rotX, *this->frame.rotY, *this->frame.rotZ);
        */
        frame_list.push_back(current);
    }
    void reiniciar() {
        frame_list.clear();
        this->frame->posX = 0.0f;
        this->frame->posY = 0.0f;
        this->frame->posZ = 0.0f;
        this->frame->rotX = 0.0f;
        this->frame->rotY = 0.0f;
        this->frame->rotZ = 0.0f;
    }
    void last_frame() {
        if (frame_list.size() > 0) {
            Keyframe::Frame current = this->frame_list[frame_list.size() - 1];
            this->frame->posX = current.posX;
            this->frame->posY = current.posY;
            this->frame->posZ = current.posZ;
            this->frame->rotX = current.rotX;
            this->frame->rotY = current.rotY;
            this->frame->rotZ = current.rotZ;
        }
        
    }
    void delete_last() {
        if (frame_list.size() > 0) {
            puts("Elimiando el frame");
            frame_list.pop_back();
            if (frame_list.size() > 0) {
                puts("Regresando el frame");
                Keyframe::Frame current = this->frame_list[frame_list.size() - 1];
                this->frame->posX = current.posX;
                this->frame->posY = current.posY;
                this->frame->posZ = current.posZ;
                this->frame->rotX = current.rotX;
                this->frame->rotY = current.rotY;
                this->frame->rotZ = current.rotZ;
            }
        }
        
    }
    void print_all() {
        int i = 0;
        puts("MOSTRANDO ANIMACION");
        for (Keyframe::Frame frame : frame_list) {

            printf("Frame %.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n",frame.posX,frame.posY,frame.posZ,frame.rotX,frame.rotY,frame.rotZ);
            i += 1;
        }
    }
    
};
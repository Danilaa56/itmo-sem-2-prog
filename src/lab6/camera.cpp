#include "GL/gl.h"
#include "windows.h"
#include "camera.h"
#include "math.h"

struct SCamera camera = {0,1.7f,3,0,0};

void cameraApply(){
    glRotatef(-camera.pitch,1,0,0);
    glRotatef(-camera.yaw,0,1,0);
    glTranslatef(-camera.x, -camera.y, -camera.z);
}

void cameraRotate(float yaw, float pitch){
    camera.yaw += yaw;
    if(camera.yaw < -180) camera.yaw+=360;
    if(camera.yaw > 180) camera.yaw-=360;
    camera.pitch += pitch;
    if(camera.pitch < -90) camera.pitch = -90;
    if(camera.pitch > 90) camera.pitch = 90;
    //printf("Rotation: %f %f\n", camera.yaw, camera.pitch);
}

void cameraAutoMouseMove(int centerX, int centerY, float speedX, float speedY){
    POINT cur;
    POINT base = {centerX, centerY};
    GetCursorPos(&cur);
    if(base.x!=cur.x || base.y !=cur.y)
        cameraRotate((base.x-cur.x)*speedX,(base.y-cur.y)*speedY);
    SetCursorPos(base.x, base.y);
}

void cameraMove(enum DIRECTION direction, float speed){
    float rads = camera.yaw/180*M_PI;
    switch(direction){
        case FORWARD:
            camera.z -= speed*cosf(rads);
            camera.x -= speed*sinf(rads);
            break;
        case BACKWARD:
            camera.z += speed*cosf(rads);
            camera.x += speed*sinf(rads);
            break;
        case LEFT:
            camera.x -= speed*cosf(rads);
            camera.z += speed*sinf(rads);
            break;
        case RIGHT:
            camera.x += speed*cosf(rads);
            camera.z -= speed*sinf(rads);
            break;
        case UP:
            camera.y += speed;
            break;
        case DOWN:
            camera.y -= speed;
            break;
    }
    //printf("Position: %f %f\n", camera.x, camera.z);
}
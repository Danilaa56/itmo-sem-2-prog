#ifndef FIRST_CAMERA_H
#define FIRST_CAMERA_H
struct SCamera{
    float x,y,z,yaw,pitch;
    //float yaw, pitch;
};

enum DIRECTION{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

void cameraApply();
void cameraRotate(float yaw, float pitch);
void cameraAutoMouseMove(int centerX, int centerY, float speedX, float speedY);
void cameraMove(enum DIRECTION direction, float speed);
#endif

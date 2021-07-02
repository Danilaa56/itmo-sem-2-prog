#include <windows.h>
#include <GL/gl.h>
#include <cstdio>
#include "camera.h"
#include "CubeManager.h"

void update();
void draw();
void windowResize(int width, int height);
void init();
void checkKeys();

static float vert[] = {0,0,0, 0,1,0, 1,1,0, 1,0,0};
static float textCoords[] = {0,1, 0,0, 1,0, 1,1};

void drawGrayScreen(){
    static float vertScreen[] = {-1,-1, -1,1, 1,1, 1,-1};
    glPushMatrix();
    glLoadIdentity();
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glVertexPointer(2, GL_FLOAT, 0, &vertScreen);
    glEnableClientState(GL_VERTEX_ARRAY);
    glColor4f(0, 0, 0, 0.5);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisable(GL_BLEND);
    glPopMatrix();
}

int isPaused = 0;
HWND hwnd;
HDC hdc;

void setPause(int isPause){
    if(isPause != isPaused){
        isPaused = isPause;
        if(isPaused){
            ShowCursor(TRUE);
        }else{
            ShowCursor(FALSE);
            SetCursorPos(500, 500);
        }
    }
}
void togglePause(){
    setPause(1 - isPaused);
}

void checkKeys(){
    static float speed = 0.3716666;
    if(!isPaused){
        if(GetAsyncKeyState(VK_UP))// ||GetAsyncKeyState('W'))
            cameraMove(FORWARD, speed);
        if(GetAsyncKeyState(VK_DOWN))// ||GetAsyncKeyState('S'))
            cameraMove(BACKWARD, speed);
        if(GetAsyncKeyState(VK_LEFT))// ||GetAsyncKeyState('A'))
            cameraMove(LEFT, speed);
        if(GetAsyncKeyState(VK_RIGHT))// ||GetAsyncKeyState('D'))
            cameraMove(RIGHT, speed);
        if(GetAsyncKeyState(VK_SPACE))
            cameraMove(UP, speed);
        if(GetAsyncKeyState(VK_SHIFT))
            cameraMove(DOWN, speed);
    }
}
void windowResize(int width, int height){
    float ratio = (float)width / (float)height;
    GLdouble k = 0.1;
    glViewport(0,0, width, height);
    glLoadIdentity();
    glFrustum(-ratio*k, ratio*k, -k, k, k*2, 100);
}

void update(){
    checkKeys();
    if(!isPaused){
        if(GetForegroundWindow() == hwnd)
            cameraAutoMouseMove(500, 500, 0.1, 0.1);
    }
}

bool fixTurnOffPause = true;
void draw(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    {
        cameraApply();
        drawCube();

        if(isPaused)
            drawGrayScreen();
    }
    glPopMatrix();
    SwapBuffers(hdc);

    if(fixTurnOffPause){
        fixTurnOffPause = false;
        togglePause();
    }
}

void init(){
    RECT rect;
    GetClientRect(hwnd, &rect);
    windowResize(rect.right-rect.left, rect.bottom-rect.top);
    SetCursorPos(500, 500);
    ShowCursor(FALSE);

    initCube("cube.txt");
    togglePause();
}



void EnableOpenGL(HGLRC* hRC){
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    hdc = GetDC(hwnd);

    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(hdc, &pfd);

    SetPixelFormat(hdc, iFormat, &pfd);

    *hRC = wglCreateContext(hdc);

    wglMakeCurrent(hdc, *hRC);
}
void DisableOpenGL (HGLRC hRC){
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hdc);
}

LRESULT CALLBACK WindowProc(HWND _hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    hwnd = _hwnd;
    switch (uMsg){
        case WM_CLOSE:
            PostQuitMessage(0);
            break;
        case WM_DESTROY:
            return 0;
        case WM_KEYDOWN:
            switch (wParam){
                case VK_ESCAPE:
                    togglePause();
                    break;
                case VK_BACK:
                    PostQuitMessage(0);
                    break;
                default:
                    onKeyDown(wParam);
                    break;
            }
            break;
        case WM_SIZE:
            windowResize(LOWORD(lParam), HIWORD(lParam));
            break;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
    WNDCLASSEX wcex;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "MyLab6CubeWindowClass";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;


    if (!RegisterClassEx(&wcex))
        return 0;

    hwnd = CreateWindowEx(0,
                          wcex.lpszClassName,
                          "Cubic lab",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          1600,
                          900,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);
    ShowWindow(hwnd, nCmdShow);

    EnableOpenGL(&hRC);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.7f, 1.0f, 0.8f, 0.0f);

    init();

    while (!bQuit){
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
            if (msg.message == WM_QUIT){
                bQuit = TRUE;
            }
            else{
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else{
            update();
            draw();
            Sleep(16);
        }
    }

    DisableOpenGL(hRC);
    DestroyWindow(hwnd);
    return msg.wParam;
}
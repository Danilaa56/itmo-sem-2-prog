#include "CubeManager.h"
#include "GL/gl.h"
#include <iostream>
#include <ctime>
#include <utility>
#include <fstream>
#include "Color.h"

float square[12] = {0,0,0, 1,0,0, 1,1,0, 0,1,0};
float cubeSize = 0.3f;

Color WHITE("WHITE", 0, 1, 1, 1);
Color RED("RED", 1, 1, 0, 0);
Color BLUE("BLUE", 2, 0, 0, 1);
Color ORANGE("ORANGE", 3, 1, 0.5, 0);
Color GREEN("GREEN", 4, 0, 1, 0);
Color YELLOW("YELLOW", 5, 1, 1, 0);

Color colorList[] = {WHITE, RED, BLUE, ORANGE, GREEN, YELLOW};

void cyclicSwap(int& num1, int& num2, int& num3, int& num4){
    int tmp = num1;
    num1 = num2;
    num2 = num3;
    num3 = num4;
    num4 = tmp;
}

class Cube{
public:
    union {
        int** sides[6];
        struct{
            int** front = new int*[3];

            int** right = new int*[3];
            int** top = new int*[3];
            int** left = new int*[3];
            int** bottom = new int*[3];

            int** back = new int*[3];
        };
    };

    Cube(){
        for(int side = 0; side < 6; side++){
            for(int i = 0; i < 3; i++){
                sides[side][i] = new int[3];
            }
        }
        clear();
    }

    void drawSide(int** side){
        for (int i = 0; i < 3; i++){
            for (int j = 0; j < 3; j++) {
                glPushMatrix();

                glTranslatef(j, i,0);
                colorList[side[i][j]].color();
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

                glPopMatrix();
            }
        }
    }

    void draw() {
        glPushMatrix();

        glTranslatef(-0.2, 1, 0);
        glRotatef(20, 1, 0, 0);
        glRotatef(-20, 0, 1, 0);

        glEnableClientState(GL_VERTEX_ARRAY);
        glScalef(cubeSize, cubeSize, cubeSize);

        glTranslatef(-1.5, 0, 0);

        glVertexPointer(3, GL_FLOAT, 0, &square);

        drawSide(front);
        glTranslatef(3, 0, 0);
        glRotatef(90, 0, 1, 0);
        drawSide(right);
        glTranslatef(3, 0, 0);
        glRotatef(90, 0, 1, 0);
        drawSide(back);
        glTranslatef(3, 0, 0);
        glRotatef(90, 0, 1, 0);
        drawSide(left);
        glTranslatef(3, 0, 0);
        glRotatef(90, 0, 1, 0);

        glTranslatef(0, 3, 0);
        glRotatef(-90, 1, 0, 0);
        drawSide(top);
        glTranslatef(0, 3, -3);
        glRotatef(180, 1, 0, 0);
        drawSide(bottom);

        glPopMatrix();
    }

    void clear(){
        for(int side = 0; side < 6; side++){
            for(int i = 0; i < 3; i++){
                for(int j = 0; j < 3; j++)
                    sides[side][i][j] = side;
            }
        }
    }

    void shuffle(){
        srand(time(nullptr));
        for(int i = 0; i < 100; i++){
            int action = (rand()%10);
            switch(action){
                case 0:
                    RotateAntiClockWise(false);
                    break;
                case 1:
                    RotateClockWise(false);
                    break;
                case 2:
                    RotateRightUp(false);
                    break;
                case 3:
                    RotateRightDown(false);
                    break;
                case 4:
                    RotateLeftUp(false);
                    break;
                case 5:
                    RotateLeftDown(false);
                    break;
                case 6:
                    RotateTopLeft(false);
                    break;
                case 7:
                    RotateTopRight(false);
                    break;
                case 8:
                    RotateBottomLeft(false);
                    break;
                case 9:
                    RotateBottomRight(false);
                    break;
            }
        }
    }

    void save(std::string const& fileName){
        std::ofstream ofile;
        ofile.open(fileName, std::ios::out);

        for(int i = 2; i >= 0; i--)
            ofile << "   " << top[i][0] << top[i][1] << top[i][2] << '\n';

        for(int i = 2; i >= 0; i--)
            ofile << left[i][0] << left[i][1] << left[i][2]
                    << front[i][0] << front[i][1] << front[i][2]
                    << right[i][0] << right[i][1] << right[i][2]
                    << back[i][0] << back[i][1] << back[i][2] << '\n';
        for(int i = 2; i >= 0; i--)
            ofile << "   " << bottom[i][0] << bottom[i][1] << bottom[i][2] << '\n';

        ofile.close();

        std::cout << "Saved at " << fileName << std::endl;
    }

    void load(std::string const& fileName){
        std::ifstream in;
        in.open(fileName, std::ios::in);
        if(!in.is_open())
            return;

        char c0, c1, c2;

        for(int i = 2; i >= 0; i--){
            in >> c0 >> c1 >> c2;
            top[i][0] = c0 - '0';
            top[i][1] = c1 - '0';
            top[i][2] = c2 - '0';
        }

        for(int i = 2; i >= 0; i--){
            in >> c0 >> c1 >> c2;
            left[i][0] = c0 - '0';
            left[i][1] = c1 - '0';
            left[i][2] = c2 - '0';
            in >> c0 >> c1 >> c2;
            front[i][0] = c0 - '0';
            front[i][1] = c1 - '0';
            front[i][2] = c2 - '0';
            in >> c0 >> c1 >> c2;
            right[i][0] = c0 - '0';
            right[i][1] = c1 - '0';
            right[i][2] = c2 - '0';
            in >> c0 >> c1 >> c2;
            back[i][0] = c0 - '0';
            back[i][1] = c1 - '0';
            back[i][2] = c2 - '0';
        }

        for(int i = 2; i >= 0; i--){
            in >> c0 >> c1 >> c2;
            bottom[i][0] = c0 - '0';
            bottom[i][1] = c1 - '0';
            bottom[i][2] = c2 - '0';
        }

        std::cout << "Loaded at " << fileName << std::endl;
        in.close();

        if(!checkInvariant()){
            std::cout << "Loaded state is invalid, clearing " << std::endl;
            clear();
        }
    }

    bool checkInvariant(){
        int colorsCounter[6] = {0, 0, 0, 0, 0, 0};
        for(int side = 0; side < 6; side++){
            for(int i = 0; i < 3; i++){
                for(int j = 0; j < 3; j++){
                    if(sides[side][i][j] > 5 || sides[side][i][j] < 0)
                        return false;
                    colorsCounter[sides[side][i][j]]++;
                }
            }
        }
        for(int& side : colorsCounter){
            if(side != 9)
                return false;
            side = 0;
        }

        for(int side = 0; side < 6; side++){
            colorsCounter[sides[side][1][1]]++;
        }
        for(int& side : colorsCounter){
            if(side != 1)
                return false;
            side = 0;
        }

//        for(int& side : colorsCounter){
//            for(int i = 0; i < 3; i++){
//                for(int j = 0; j < 3; j++){
//                    colorsCounter[sides[side][i][j]]++;
//                }
//            }
//        }
        return true;
    }

    void _rotateSideClock(int** side){
        cyclicSwap(side[0][0], side[0][2], side[2][2], side[2][0]);
        cyclicSwap(side[0][1], side[1][2], side[2][1], side[1][0]);
    }
    void _rotateSideAntiClock(int** side){
        cyclicSwap(side[2][0], side[2][2], side[0][2], side[0][0]);
        cyclicSwap(side[1][0], side[2][1], side[1][2], side[0][1]);
    }

    void RotateClockWise(bool showMessage){
        _rotateSideClock(front);

        cyclicSwap(left[0][2], bottom[2][2], right[2][0], top[0][0]);
        cyclicSwap(left[1][2], bottom[2][1], right[1][0], top[0][1]);
        cyclicSwap(left[2][2], bottom[2][0], right[0][0], top[0][2]);
    }
    void RotateAntiClockWise(bool showMessage){
        _rotateSideAntiClock(front);

        cyclicSwap(top[0][0], right[2][0], bottom[2][2], left[0][2]);
        cyclicSwap(top[0][1], right[1][0], bottom[2][1], left[1][2]);
        cyclicSwap(top[0][2], right[0][0], bottom[2][0], left[2][2]);
    }

    void ChooseSideRight(bool showMessage){
        if(showMessage)
            std::cout << "Choose right side" << std::endl;
        auto tmp = front;
        front = right;
        right = back;
        back = left;
        left = tmp;

        _rotateSideClock(top);
        _rotateSideAntiClock(bottom);
    }

    void ChooseSideLeft(bool showMessage){
        if(showMessage)
            std::cout << "Choose left side" << std::endl;
        auto tmp = front;
        front = left;
        left = back;
        back = right;
        right = tmp;

        _rotateSideAntiClock(top);
        _rotateSideClock(bottom);
    }

    void ChooseSideTop(bool showMessage){
        if(showMessage)
            std::cout << "Choose top side" << std::endl;
        RotateRightDown(false);
        RotateLeftDown(false);

        cyclicSwap(front[0][1], top[0][1], back[2][1], bottom[0][1]);
        cyclicSwap(front[1][1], top[1][1], back[1][1], bottom[1][1]);
        cyclicSwap(front[2][1], top[2][1], back[0][1], bottom[2][1]);
    }

    void ChooseSideBottom(bool showMessage){
        if(showMessage)
            std::cout << "Choose bottom side" << std::endl;
        RotateRightUp(false);
        RotateLeftUp(false);

        cyclicSwap(bottom[0][1], back[2][1], top[0][1], front[0][1]);
        cyclicSwap(bottom[1][1], back[1][1], top[1][1], front[1][1]);
        cyclicSwap(bottom[2][1], back[0][1], top[2][1], front[2][1]);
    }

    void RotateRightUp(bool showMessage){
        if(showMessage)
            std::cout << "Rotate right up" << std::endl;
        ChooseSideRight(false);
        RotateClockWise(false);
        ChooseSideLeft(false);
    }

    void RotateRightDown(bool showMessage){
        if(showMessage)
            std::cout << "Rotate right down" << std::endl;
        ChooseSideRight(false);
        RotateAntiClockWise(false);
        ChooseSideLeft(false);
    }

    void RotateLeftUp(bool showMessage){
        if(showMessage)
            std::cout << "Rotate left up" << std::endl;
        ChooseSideLeft(false);
        RotateAntiClockWise(false);
        ChooseSideRight(false);
    }

    void RotateLeftDown(bool showMessage){
        if(showMessage)
            std::cout << "Rotate left down" << std::endl;
        ChooseSideLeft(false);
        RotateClockWise(false);
        ChooseSideRight(false);
    }

    void RotateTopRight(bool showMessage){
        if(showMessage)
            std::cout << "Rotate top right" << std::endl;
        ChooseSideTop(false);
        RotateAntiClockWise(false);
        ChooseSideBottom(false);
    }

    void RotateTopLeft(bool showMessage){
        if(showMessage)
            std::cout << "Rotate top left" << std::endl;
        ChooseSideTop(false);
        RotateClockWise(false);
        ChooseSideBottom(false);
    }

    void RotateBottomRight(bool showMessage){
        if(showMessage)
            std::cout << "Rotate bottom right" << std::endl;
        ChooseSideBottom(false);
        RotateClockWise(false);
        ChooseSideTop(false);
    }

    void RotateBottomLeft(bool showMessage){
        if(showMessage)
            std::cout << "Rotate bottom left" << std::endl;
        ChooseSideBottom(false);
        RotateAntiClockWise(false);
        ChooseSideTop(false);
    }

    void RotateBackClockWise(bool showMessage){
        if(showMessage)
            std::cout << "Rotate back clockwise" << std::endl;
        _rotateSideAntiClock(back);

        cyclicSwap(left[0][0], bottom[0][2], right[2][2], top[2][0]);
        cyclicSwap(left[1][0], bottom[0][1], right[1][2], top[2][1]);
        cyclicSwap(left[2][0], bottom[0][0], right[0][2], top[2][2]);
    }

    void RotateBackAntiClockWise(bool showMessage){
        if(showMessage)
            std::cout << "Rotate back anti clockwise" << std::endl;
        _rotateSideClock(back);

        cyclicSwap(top[2][0], right[2][2], bottom[0][2], left[0][0]);
        cyclicSwap(top[2][1], right[1][2], bottom[0][1], left[1][0]);
        cyclicSwap(top[2][2], right[0][2], bottom[0][0], left[2][0]);
    }

    void RotateMidRight(bool showMessage){
        if(showMessage)
            std::cout << "Rotate mid right" << std::endl;
        cyclicSwap(front[1][0], left[1][0], back[1][0], right[1][0]);
        cyclicSwap(front[1][1], left[1][1], back[1][1], right[1][1]);
        cyclicSwap(front[1][2], left[1][2], back[1][2], right[1][2]);
    }

    void RotateMidLeft(bool showMessage){
        if(showMessage)
            std::cout << "Rotate mid left" << std::endl;
        cyclicSwap(right[1][0], back[1][0], left[1][0], front[1][0]);
        cyclicSwap(right[1][1], back[1][1], left[1][1], front[1][1]);
        cyclicSwap(right[1][2], back[1][2], left[1][2], front[1][2]);
    }



    void pass1(){
        if(front[1][1] == 0 || front[1][1] == 5)
            ChooseSideLeft(true);
        switch(front[1][1]){
            case 1:
            case 3:
                if(front[1][1] == 1)
                    ChooseSideLeft(true);
                else
                    ChooseSideRight(true);
                if(front[1][1] == 5){
                    ChooseSideTop(true);
                    ChooseSideTop(true);
                } else if(front[1][1] == 2){
                    ChooseSideBottom(true);
                } else if(front[1][1] == 4){
                    ChooseSideTop(true);
                }
                break;
            case 2:
            case 4:
                if(front[1][1] == 2)
                    ChooseSideBottom(true);
                else
                    ChooseSideTop(true);
                if(front[1][1] == 5){
                    ChooseSideLeft(true);
                    ChooseSideLeft(true);
                } else if(front[1][1] == 1){
                    ChooseSideLeft(true);
                } else if(front[1][1] == 3){
                    ChooseSideRight(true);
                }
                break;
        }

        int sideColor = right[1][1];
        int needColor = front[1][1];

        while(1){
            if( (top[2][1] == needColor && back[2][1] == sideColor) || (top[2][1] == sideColor && back[2][1] == needColor)){
                RotateTopLeft(true);
                RotateTopLeft(true);
            }
            if( (right[1][2] == needColor && back[1][0] == sideColor) || (right[1][2] == sideColor && back[1][0] == needColor)){
                RotateRightDown(true);
                RotateRightDown(true);
            }
            if( (bottom[0][1] == needColor && back[0][1] == sideColor) || (bottom[0][1] == sideColor && back[0][1] == needColor)){
                RotateBottomRight(true);
                RotateBottomRight(true);
            }
            if( (left[1][0] == needColor && back[1][2] == sideColor) || (left[1][0] == sideColor && back[1][2] == needColor)){
                RotateLeftUp(true);
                RotateLeftUp(true);
            }

            if(top[1][2] == needColor && right[2][1] == sideColor){
                RotateRightDown(true);
                break;
            }
            if(top[1][2] == sideColor && right[2][1] == needColor){
                RotateTopLeft(true);
                RotateClockWise(true);
                break;
            }

            if(top[1][0] == needColor && left[2][1] == sideColor){
                RotateTopLeft(true);
                RotateTopLeft(true);
                RotateRightDown(true);
                break;
            }
            if(top[1][0] == sideColor && left[2][1] == needColor){
                RotateTopRight(true);
                RotateClockWise(true);
                break;
            }

            if(left[0][1] == needColor && bottom[1][0] == sideColor){
                RotateBottomRight(true);
                RotateAntiClockWise(true);
                break;
            }
            if(left[0][1] == sideColor && bottom[1][0] == needColor){
                RotateLeftUp(true);
                RotateClockWise(true);
                RotateClockWise(true);
                break;
            }

            if(bottom[1][2] == needColor && right[0][1] == sideColor){
                RotateRightUp(true);
                break;
            }
            if(bottom[1][2] == sideColor && right[0][1] == needColor){
                RotateBottomLeft(true);
                RotateAntiClockWise(true);
                break;
            }

            if(front[1][2] == front[1][1] && right[1][0] == sideColor){
                break;
            }
            if(front[0][1] == front[1][1] && bottom[2][1] == sideColor){
                RotateAntiClockWise(true);
                break;
            }
            if(front[1][0] == front[1][1] && left[1][2] == sideColor){
                RotateAntiClockWise(true);
                RotateAntiClockWise(true);
                break;
            }
            if(front[2][1] == front[1][1] && top[0][1] == sideColor){
                RotateClockWise(true);
                break;
            }

            if(front[1][2] == sideColor && right[1][0] == front[1][1]){
                RotateAntiClockWise(true);
                RotateTopRight(true);
                RotateRightDown(true);
                break;
            }
            if(front[0][1] == sideColor && bottom[2][1] == front[1][1]){
                RotateBottomRight(true);
                RotateRightUp(true);
                break;
            }
            if(front[1][0] == sideColor && left[1][2] == front[1][1]){
                RotateAntiClockWise(true);
                RotateBottomRight(true);
                RotateRightUp(true);
                break;
            }
            if(front[2][1] == sideColor && top[0][1] == front[1][1]){
                RotateTopRight(true);
                RotateRightDown(true);
                break;
            }
        }

        ChooseSideRight(true);
        ChooseSideTop(true);
        ChooseSideLeft(true);

        sideColor = right[1][1];
        needColor = front[1][1];
        while(1){
            if( (top[2][1] == needColor && back[2][1] == sideColor) || (top[2][1] == sideColor && back[2][1] == needColor)){
                RotateBackClockWise(true);
            }
            if( (bottom[0][1] == needColor && back[0][1] == sideColor) || (bottom[0][1] == sideColor && back[0][1] == needColor)){
                RotateBackAntiClockWise(true);
            }
            if( (left[1][0] == needColor && back[1][2] == sideColor) || (left[1][0] == sideColor && back[1][2] == needColor)){
                RotateBackAntiClockWise(true);
                RotateBackAntiClockWise(true);
            }

            if(top[1][2] == needColor && right[2][1] == sideColor || top[1][2] == sideColor && right[2][1] == needColor){
                RotateRightUp(true);
            }

            if(top[1][0] == needColor && left[2][1] == sideColor || top[1][0] == sideColor && left[2][1] == needColor){
                RotateTopLeft(true);
                RotateBackClockWise(true);
            }
            if(left[0][1] == needColor && bottom[1][0] == sideColor || left[0][1] == sideColor && bottom[1][0] == needColor){
                RotateLeftDown(true);
                RotateBackClockWise(true);
                RotateBackClockWise(true);
            }

            if( (bottom[1][2] == needColor && right[0][1] == sideColor) || (bottom[1][2] == sideColor && right[0][1] == needColor) ){
                RotateRightDown(true);
            }

            if(front[1][2] == needColor && right[1][0] == sideColor){
                break;
            }
            if( (front[1][0] == needColor && left[1][2] == sideColor) || (front[1][0] == sideColor && left[1][2] == needColor) ){
                RotateLeftUp(true);
                RotateLeftUp(true);
                RotateBackClockWise(true);
                RotateBackClockWise(true);
            }
            if( (front[2][1] == needColor && top[0][1] == sideColor) || (front[2][1] == sideColor && top[0][1] == needColor) ){
                RotateTopLeft(true);
                RotateTopLeft(true);
                RotateBackClockWise(true);
            }

            if(front[1][2] == sideColor && right[1][0] == front[1][1]){
                RotateRightDown(true);
                RotateRightDown(true);
            }

            if(right[1][2] == needColor && back[1][0] == sideColor){
                RotateBackAntiClockWise(true);
                RotateTopLeft(true);
                RotateRightDown(true);
                RotateTopRight(true);
                break;
            }
            if(right[1][2] == sideColor && back[1][0] == needColor){
                RotateRightDown(true);
                RotateRightDown(true);
                break;
            }
            break;
        }

        ChooseSideRight(true);
        ChooseSideTop(true);
        ChooseSideLeft(true);

        sideColor = right[1][1];
        needColor = front[1][1];
        while(1){
            if( (top[2][1] == needColor && back[2][1] == sideColor) || (top[2][1] == sideColor && back[2][1] == needColor)){
                RotateBackClockWise(true);
            }
            if( (bottom[0][1] == needColor && back[0][1] == sideColor) || (bottom[0][1] == sideColor && back[0][1] == needColor)){
                RotateBackAntiClockWise(true);
            }
            if( (left[1][0] == needColor && back[1][2] == sideColor) || (left[1][0] == sideColor && back[1][2] == needColor)){
                RotateBackAntiClockWise(true);
                RotateBackAntiClockWise(true);
            }

            if(top[1][2] == needColor && right[2][1] == sideColor || top[1][2] == sideColor && right[2][1] == needColor){
                RotateRightUp(true);
            }

            if(top[1][0] == needColor && left[2][1] == sideColor || top[1][0] == sideColor && left[2][1] == needColor){
                RotateTopLeft(true);
                RotateBackClockWise(true);
            }
            if(left[0][1] == needColor && bottom[1][0] == sideColor || left[0][1] == sideColor && bottom[1][0] == needColor){
                RotateLeftDown(true);
                RotateBackClockWise(true);
                RotateBackClockWise(true);
                RotateLeftUp(true);
            }

            if( (bottom[1][2] == needColor && right[0][1] == sideColor) || (bottom[1][2] == sideColor && right[0][1] == needColor) ){
                RotateRightDown(true);
            }

            if(front[1][2] == needColor && right[1][0] == sideColor){
                break;
            }
            if( (front[2][1] == needColor && top[0][1] == sideColor) || (front[2][1] == sideColor && top[0][1] == needColor) ){
                RotateTopLeft(true);
                RotateTopLeft(true);
                RotateBackClockWise(true);
            }

            if(front[1][2] == sideColor && right[1][0] == front[1][1]){
                RotateRightDown(true);
                RotateRightDown(true);
            }

            if(right[1][2] == needColor && back[1][0] == sideColor){
                RotateBackAntiClockWise(true);
                RotateTopLeft(true);
                RotateRightDown(true);
                RotateTopRight(true);
                break;
            }
            if(right[1][2] == sideColor && back[1][0] == needColor){
                RotateRightDown(true);
                RotateRightDown(true);
                break;
            }
            break;
        }

        ChooseSideRight(true);
        ChooseSideTop(true);
        ChooseSideLeft(true);

        sideColor = right[1][1];
        needColor = front[1][1];
        while(1){
            if( (top[2][1] == needColor && back[2][1] == sideColor) || (top[2][1] == sideColor && back[2][1] == needColor)){
                RotateBackClockWise(true);
            }
            if( (bottom[0][1] == needColor && back[0][1] == sideColor) || (bottom[0][1] == sideColor && back[0][1] == needColor)){
                RotateBackAntiClockWise(true);
            }
            if( (left[1][0] == needColor && back[1][2] == sideColor) || (left[1][0] == sideColor && back[1][2] == needColor)){
                RotateBackAntiClockWise(true);
                RotateBackAntiClockWise(true);
            }

            if(top[1][2] == needColor && right[2][1] == sideColor || top[1][2] == sideColor && right[2][1] == needColor){
                RotateRightUp(true);
            }

            if(top[1][0] == needColor && left[2][1] == sideColor || top[1][0] == sideColor && left[2][1] == needColor){
                RotateTopLeft(true);
                RotateBackClockWise(true);
                RotateTopRight(true);
            }
            if(left[0][1] == needColor && bottom[1][0] == sideColor || left[0][1] == sideColor && bottom[1][0] == needColor){
                RotateLeftDown(true);
                RotateBackClockWise(true);
                RotateBackClockWise(true);
                RotateLeftUp(true);
            }

            if( (bottom[1][2] == needColor && right[0][1] == sideColor) || (bottom[1][2] == sideColor && right[0][1] == needColor) ){
                RotateRightDown(true);
            }

            if(front[1][2] == needColor && right[1][0] == sideColor){
                break;
            }

            if(front[1][2] == sideColor && right[1][0] == front[1][1]){
                RotateRightDown(true);
                RotateRightDown(true);
            }

            if(right[1][2] == needColor && back[1][0] == sideColor){
                RotateBackAntiClockWise(true);
                RotateTopLeft(true);
                RotateRightDown(true);
                RotateTopRight(true);
                break;
            }
            if(right[1][2] == sideColor && back[1][0] == needColor){
                RotateRightDown(true);
                RotateRightDown(true);
                break;
            }
            break;
        }

        ChooseSideRight(true);
        ChooseSideTop(true);
        ChooseSideLeft(true);
    }

    bool compareColors(int color1a, int color1b, int color2a, int color2b){
        if(color1a == color2a && color1b == color2b)
            return true;
        if(color1b == color2a && color1a == color2b)
            return true;
        return false;
    }

    bool compareColors(int color1a, int color1b, int color1c, int color2a, int color2b, int color2c){
        if(color1a == color2a && color1b == color2b && color1c == color2c)
            return true;
        if(color1a == color2a && color1c == color2b && color1b == color2c)
            return true;
        if(color1b == color2a && color1a == color2b && color1c == color2c)
            return true;
        if(color1b == color2a && color1c == color2b && color1a == color2c)
            return true;
        if(color1c == color2a && color1a == color2b && color1b == color2c)
            return true;
        if(color1c == color2a && color1b == color2b && color1a == color2c)
            return true;
        return false;
    }

    void pass2(){

        for(int side = 0; side < 4; side++){
            int neededColor = front[1][1];
            int side1Color = right[1][1];
            int side2Color = bottom[1][1];
            int i;
            for(i = 0; i < 4; i++){
                if(compareColors(neededColor, side1Color, side2Color, front[0][2], right[0][0], bottom[2][2])){
                    RotateBottomRight(true);
                    RotateBackAntiClockWise(true);
                    RotateBottomLeft(true);
                    RotateBackClockWise(true);
                    break;
                }
                if(compareColors(neededColor, side1Color, side2Color, right[0][2], back[0][0], bottom[0][2])){
                    break;
                }
                ChooseSideRight(true);
                ChooseSideTop(true);
                ChooseSideLeft(true);
            }
            for(int j = i; j > 0; j--){
                ChooseSideRight(true);
                ChooseSideBottom(true);
                ChooseSideLeft(true);
                RotateBackClockWise(true);
            }

            if(right[0][2] == side1Color){
                RotateBottomRight(true);
                RotateBackAntiClockWise(true);
                RotateBottomLeft(true);
            } else if(right[0][2] == neededColor){
                RotateRightDown(true);
                RotateBackClockWise(true);
                RotateRightUp(true);
            } else {
                RotateRightDown(true);
                RotateTopRight(true);
                RotateBackAntiClockWise(true);
                RotateBackAntiClockWise(true);
                RotateTopLeft(true);
                RotateRightUp(true);
            }
            ChooseSideRight(true);
            ChooseSideTop(true);
            ChooseSideLeft(true);
        }
    }

    void pass3(){
        ChooseSideTop(true);

        for(int side = 0; side < 4; side++){
            int neededColor = front[1][1];
            int sideColor = right[1][1];

            if(front[1][2] == neededColor && right[1][0] == sideColor){
                ChooseSideLeft(true);
                continue;
            }
            int i;
            for(i = 0; i < 4; i++){
                if(compareColors(neededColor, sideColor, front[1][2], right[1][0])){
                    RotateTopLeft(true);
                    RotateRightUp(true);
                    RotateTopRight(true);
                    RotateRightDown(true);
                    RotateTopRight(true);
                    RotateAntiClockWise(true);
                    RotateTopLeft(true);
                    RotateClockWise(true);
                }
                ChooseSideLeft(true);
            }

            while(!compareColors(neededColor, sideColor, front[2][1], top[0][1]))
                RotateTopRight(true);
            if(front[2][1] == neededColor){
                RotateTopLeft(true);
                RotateRightUp(true);
                RotateTopRight(true);
                RotateRightDown(true);
                RotateTopRight(true);
                RotateAntiClockWise(true);
                RotateTopLeft(true);
                RotateClockWise(true);
            } else {
                RotateTopRight(true);

                RotateTopRight(true);
                RotateAntiClockWise(true);
                RotateTopLeft(true);
                RotateClockWise(true);
                RotateTopLeft(true);
                RotateRightUp(true);
                RotateTopRight(true);
                RotateRightDown(true);
            }

            ChooseSideLeft(true);
        }
    }

    void pass4(){
        while(! (front[2][1] == front[1][1] || top[0][1] == front[1][1])){
            RotateTopLeft(true);
        }

        while(1){
            if(compareColors(top[1][1], left[1][1], left[2][1], top[1][0])){
                if(compareColors(top[1][1], right[1][1], right[2][1], top[1][2])){
                    break;
                } else {
                    ChooseSideRight(true);
                    ChooseSideRight(true);
                    subpass4();
                    ChooseSideRight(true);
                    ChooseSideRight(true);
                }
            } else {
                if(compareColors(top[1][1], right[1][1], right[2][1], top[1][2])){
                    ChooseSideLeft(true);
                    subpass4();
                    ChooseSideRight(true);
                } else if(compareColors(top[1][1], back[1][1], back[2][1], top[2][1])){
                    ChooseSideLeft(true);
                    subpass4();
                    ChooseSideLeft(true);
                    subpass4();
                    ChooseSideRight(true);
                    subpass4();
                    ChooseSideRight(true);
                } else if(compareColors(top[1][1], back[1][1], top[1][0], left[1][2])){
                    ChooseSideLeft(true);
                    ChooseSideLeft(true);
                    subpass4();
                    ChooseSideRight(true);
                    subpass4();
                    ChooseSideRight(true);
                } else {
                    ChooseSideLeft(true);
                    subpass4();
                    ChooseSideLeft(true);
                    subpass4();
                    ChooseSideLeft(true);
                    ChooseSideLeft(true);
                }
            }
        }
    }
    void subpass4(){
        RotateTopLeft(true);
        RotateClockWise(true);
        RotateRightUp(true);
        RotateTopLeft(true);
        RotateRightDown(true);
        RotateTopRight(true);
        RotateAntiClockWise(true);
    }

    void pass5() {
        if(top[1][0] != top[1][1] && top[1][2] != top[1][1]){
            ChooseSideLeft(true);
            subpass5_2();
            ChooseSideRight(true);
        }

        if(top[0][1] != top[1][1]){
            if(top[2][1] != top[1][1]){
                subpass5_2();
            } else if(top[1][0] != top[1][1]){
                subpass5_1();
            } else {
                ChooseSideRight(true);
                subpass5_1();
                ChooseSideLeft(true);
            }
        } else if(top[1][0] != top[1][1]){
            ChooseSideLeft(true);
            subpass5_1();
            ChooseSideRight(true);
        } else if(top[2][1] != top[1][1]){
            ChooseSideLeft(true);
            ChooseSideLeft(true);
            subpass5_1();
            ChooseSideRight(true);
            ChooseSideRight(true);
        }
    }
    void subpass5() {
        RotateClockWise(true);
        RotateMidRight(true);
        RotateClockWise(true);
        RotateMidRight(true);
        RotateClockWise(true);
        RotateMidRight(true);
        RotateClockWise(true);
        RotateMidRight(true);
    }
    void subpass5_1(){
        subpass5();
        RotateTopRight(true);
        subpass5();
        RotateTopLeft(true);
    }
    void subpass5_2(){
        subpass5();
        RotateTopRight(true);
        RotateTopRight(true);
        subpass5();
        RotateTopLeft(true);
        RotateTopLeft(true);
    }

    void pass6(){
        int i = 4;
        while(1){
            for(i = 0; i < 4; i++){
                if(compareColors(back[1][1], top[1][1], left[1][1], top[2][0], left[2][0], back[2][2])){
                    break;
                }
                ChooseSideRight(true);
            }
            if(i < 4)
                break;
            subpass6_1();
        }
        if(compareColors(front[1][1], left[1][1], top[1][1], front[2][0], left[2][2], top[0][0])){
            for(int j = i; j > 0; j--)
                ChooseSideLeft(true);
            return;
        }
        if(compareColors(front[1][1], top[1][1], right[1][1], front[2][0], left[2][2], top[0][0])){
            subpass6_2();
        } else {
            subpass6_1();
        }
        for(int j = i; j > 0; j--)
            ChooseSideLeft(true);
    }
    void subpass6_1(){
        RotateRightDown(true);
        RotateAntiClockWise(true);
        RotateLeftUp(true);
        RotateClockWise(true);
        RotateRightUp(true);
        RotateAntiClockWise(true);
        RotateLeftDown(true);
        RotateClockWise(true);
    }
    void subpass6_2(){
        RotateAntiClockWise(true);
        RotateLeftUp(true);
        RotateClockWise(true);
        RotateRightDown(true);
        RotateAntiClockWise(true);
        RotateLeftDown(true);
        RotateClockWise(true);
        RotateRightUp(true);
    }

    void pass7() {
        for(int i = 0; i < 4; i++){
            if(top[0][2] != top[1][1]){
                if(top[0][2] == front[1][1]){
                    subpass7_1();
                } else {
                    subpass7_1();
                    subpass7_1();
                }
            }
            ChooseSideLeft(true);
        }
        ChooseSideBottom(true);
        std::cout << "=== Cube is ready ===" << std::endl;
    }
    void subpass7(){
        RotateRightUp(true);
        RotateAntiClockWise(true);
        RotateRightDown(true);
        RotateClockWise(true);
        RotateRightUp(true);
        RotateAntiClockWise(true);
        RotateRightDown(true);
        RotateClockWise(true);
    }
    void subpass7_1(){
        subpass7();
        RotateTopRight(true);
        subpass7();
        RotateTopLeft(true);
        subpass7();
    }
};

Cube cube;
std::string storageFileName;

void initCube(std::string const& fileName){
    storageFileName = fileName;
    cube.load(storageFileName);
}

void drawCube(){
    cube.draw();
}

void onKeyDown(WPARAM code){
    switch(code){
        case 'I':
            cube.clear();
            break;
        case 'R':
            cube.shuffle();
            break;
        case '0':
            cube.RotateAntiClockWise(true);
            break;
        case '1':
            cube.RotateClockWise(true);
            break;
        case '2':
            cube.RotateRightUp(true);
            break;
        case '3':
            cube.RotateRightDown(true);
            break;
        case '4':
            cube.RotateLeftUp(true);
            break;
        case '5':
            cube.RotateLeftDown(true);
            break;
        case '6':
            cube.RotateTopLeft(true);
            break;
        case '7':
            cube.RotateTopRight(true);
            break;
        case '8':
            cube.RotateBottomLeft(true);
            break;
        case '9':
            cube.RotateBottomRight(true);
            break;
        case 'Q':
            cube.RotateMidLeft(true);
            break;
        case 'W':
            cube.RotateMidRight(true);
            break;
//        case 'Z':
//            cube.pass1();
//            break;
//        case 'X':
//            cube.pass2();
//            break;
//        case 'C':
//            cube.pass3();
//            break;
//        case 'V':
//            cube.pass4();
//            break;
//        case 'B':
//            cube.pass5();
//            break;
//        case 'N':
//            cube.pass6();
//            break;
//        case 'M':
//            cube.pass7();
//            break;
        case VK_RETURN:
            cube.pass1();
            cube.pass2();
            cube.pass3();
            cube.pass4();
            cube.pass5();
            cube.pass6();
            cube.pass7();
            break;
        case 'J':
            cube.RotateBackClockWise(true);
            break;
        case 'K':
            cube.RotateBackAntiClockWise(true);
            break;
        case 'S':
            cube.save(storageFileName);
            break;
        case 'L':
            cube.load(storageFileName);
            break;
    }
}

//
// Created by Neil Ketteringham on 9/30/24.
//

#include <math.h>
#include <iostream>

struct point{
    float x, y, z;
    float x_normal, y_normal, z_normal;

};

class DonutRenderer{
    const static int _screenWidth = 70;
    const static int _screenHeight = 30;

    const static float _phiSpacing = 0.05f;
    const static float _thetaSpacing = 0.05f;

    const static float _largeRadius = 20;
    const static float _smallRadius = 5.0f;

    const static float _depth = -5.0f;
    const static float ratio = 0.5f;



    public:
        static float dotNormVector(float x1, float y1, float z1, float x2, float y2, float z2){
            return x1 * x2 + y1 * y2 + z1 * z2;
        }

        static void renderFrame(float rotationXRad){

            char luminanceIndex[11] = {
                    '.', '-', '~', ':',
                    ';', '=', '!', '*', '#', '$', '@'
            };

            int screenDepth[_screenWidth][_screenHeight];
            int luminance[_screenWidth][_screenHeight];
            for (int i = 0; i < _screenWidth; i++) {
                for (int j = 0; j < _screenHeight; j++) {
                    screenDepth[i][j] = -100;
                }
            }
            for (int i = 0; i < _screenWidth; i++) {
                for (int j = 0; j < _screenHeight; j++) {
                    luminance[i][j] = 0;
                }
            }

            for (float i = 0; i < 6.28; i += _thetaSpacing) {
                for (float j = 0; j < 6.28; j += _phiSpacing) {
                    float pointX = _largeRadius * cos(i);
                    pointX += _smallRadius * cos(j) * cos(i);

                    float pointY = _largeRadius * sin(i);
                    pointY += _smallRadius * cos(j)  * sin(i);

                    float pointZ = _depth + _smallRadius * sin(j);

                    float normalX = cos(i) * cos(j);
                    float normalY = sin(i) * cos(j);
                    float normalZ = sin(i) * sin(j);

                    float newX = 1*pointX + 0*pointY + 0*pointZ;
                    float newY = 0*pointX + cos(rotationXRad)*pointY - sin(rotationXRad)*pointZ;
                    float newZ = 0*pointX + sin(rotationXRad) * pointY  + cos(rotationXRad)*pointZ;

                    float newNormalX = 1*normalX + 0*normalY + 0*normalZ;
                    float newNormalY = 0*normalX + cos(rotationXRad)*normalY - sin(rotationXRad)*normalZ;
                    float newNormalZ = 0*normalX + sin(rotationXRad)*normalY + cos(normalZ);

                    float lum = dotNormVector(newNormalX, newNormalY, newNormalZ, 0,-1,0);
                    lum += 1;
                    lum *= 5.5;
                    lum = ceil(lum);

                    if(screenDepth[(int)floor(newX + (int)(_screenWidth/2))][(int)((ratio*newY  + ((float)_screenHeight/2)))] < newZ){
                        luminance[(int)floor(newX + (int)(_screenWidth/2))][(int)((ratio*newY +( (float)_screenHeight/2))) ] = lum;
                        screenDepth[(int)floor(newX + (int)(_screenWidth/2))][(int)((ratio*newY  + ((float)_screenHeight/2)))] = newZ;
                    }
                }
            }


            for (int i = 0; i < _screenHeight; i++) {
                std::string data = "";
                for (int j = 0; j < _screenWidth; j++) {
                    data += luminanceIndex[luminance[j][i]];
                }
                std::cout << data << std::endl;
            }


            //need to render points then generate normals
            //apply rotation matrix to both
            //dot product with up vector
            //project point to screen and store depth
            //if depth is less than max ignore and wipe.
            //win?


        }
};

int main(){
    DonutRenderer * dr = new DonutRenderer();
    dr->renderFrame(1.6);

    for (float i = 0; i < 100; i+= 0.005) {
        system("clear");
        dr->renderFrame(i);
    }
    return 0;
}
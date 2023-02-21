#include "scene3d.h"

#include <QApplication>
#include <iostream>
#include <cmath>





int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    scene3D w;
    w.resize(500, 500);

    w.show();

    //GLuint vertexShader;
   // vertexShader = glCreateShader(GL_VERTEX_SHADER);
    return a.exec();
}

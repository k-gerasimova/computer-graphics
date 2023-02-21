#include "scene3d.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    scene3D w;
    w.resize(500, 500);
    w.show();
    return a.exec();
}

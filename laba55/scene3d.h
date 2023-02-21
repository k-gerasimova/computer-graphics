#ifndef SCENE3D_H
#define SCENE3D_H

#include <QMainWindow>
#include <QGLWidget>
#include <QtOpenGL>
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <QGLFunctions>

#include <QOpenGLFunctions_3_3_Core>


class scene3D : public QGLWidget, protected QOpenGLFunctions_3_3_Core
{
private:
    GLfloat xRot;
    GLfloat yRot;
    GLfloat zRot;
    GLfloat zTra;
    GLfloat nSca;
    GLuint shaderProgram;

    QPoint ptrMousePosition; //координата указателя мыши

    void scale_plus(); //приблизить сцену
    void scale_minus(); //удалиться от сцены
    void rotate_up(); //повернуть сцену вверх
    void rotate_down(); //повернуть сцену вниз
    void rotate_left(); // повернуть сцену влево
    void rotate_right(); //повернуть сцену право
    void translate_down(); //транслировать сцену вниз
    void translate_up(); //транслировать сцену вверх
    void defaultScene(); //сцена по умолчанию
    void drawAxis(); //построить оси координат

    void getVertexArray(); //построить массив вершин
    void getColorArray(); //определить массив цветов вершин
    void getIndexArray(); //определить массив индексов вершин
    void drawFigure(); //построить фигуру
    void getSurfArray();
    void getDopSurf();

protected:
    void initializeGL();
    void resizeGL(int nWidth, int nHeight);
    void paintGL();

    void mousePressEvent(QMouseEvent* pe); //обработка события нажатия мыши
    void mouseMoveEvent(QMouseEvent* pe); //обработка события перемещения мыши
    void mouseReleaseEvent(QMouseEvent* pe); //обработка события отжатия мыши
    void wheelEvent(QWheelEvent *pe); //обработка колесика мыши
    void keyPressEvent(QKeyEvent *pe); //событие нажатия клавиши

public:
    scene3D(QWidget *parent = nullptr);
    ~scene3D();
};
#endif // SCENE3D_H


#include "scene3d.h"
#include <QtGui>
#include <QOpenGLFunctions>
#include <QtGui/QOpenGLShaderProgram>
#include <QOpenGLShaderProgram>
#include <math.h>


const static float pi = 3.141593, k=pi/180; //перевод из градусов в радианы
GLfloat ColorArray[20][3]; //массив цветов вершин
GLfloat light2_position[4] = {0, 0, 1, 1};

GLfloat R = 0.75;
GLint const n = 50 ;

GLfloat SurfArray[n*(2*(n-1)+1)][3][3];
GLfloat VertexArray[(n+1)*n+1][3];
GLfloat DopSurf[3*n][3][3];


scene3D::scene3D(QWidget *parent)
    : QGLWidget(parent)
{
    xRot = -90; yRot =0; zRot=0; zTra=0; nSca=1; //начальные значения полей

}

void scene3D::initializeGL()
{
    initializeOpenGLFunctions();
    qglClearColor(Qt::white); //устанавливает белый экран
    glEnable(GL_DEPTH_TEST); //устанавливаем режим проверки глубины пикселей(чтобы
    glEnable(GL_CULL_FACE); //устанавливаем режим построения только внешних поверхностей

    glEnable(GL_LIGHTING); // включаем свет
    GLfloat light_ambient[3] = {3, 2, -5};
    glClearColor(0.3, 0.3, 0.3, 0.0);
    glShadeModel(GL_SMOOTH);
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
}

void scene3D::resizeGL(int nWidth, int nHeight)
{
    glMatrixMode(GL_PROJECTION); //устанавливаем текущей проекционную матрицу
    glLoadIdentity(); //присваиваем проекционной матрице единичную м
    GLfloat ratio = (GLfloat)nHeight/(GLfloat)nWidth;
    //отношение высоты окна к его ширине
    if (nWidth >= nHeight)
        //параметры видимости ортогональной проекции
        glOrtho(-1.0/ratio, 1.0/ratio, -1.0, 1.0, -10.0, 1.0);
    else
        glOrtho(-1.0, 1.0, -1.0*ratio, 1.0*ratio, -10.0, 1.0);
    glOrtho(-1.0, 1.0, -1.0, 1.0, -10.0, 1.0);
    glViewport(0, 0, (GLint)nWidth, (GLint)nHeight);
}

void scene3D::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotatef(xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);
    glRotatef(zRot, 0.0f, 0.0f, 1.0f);
    glTranslatef(0.0f , zTra, 0.0f);
    glScalef(nSca, nSca, nSca); //масштабирование

    GLfloat material_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_diffuse);

    GLfloat light2_diffuse[] = { 0.5, 0.7, 0.5 };

    glEnable(GL_LIGHT2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
    glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
    glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 0.5);
    glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.4);
    glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.2);
    drawAxis();

    drawFigure();



}

void scene3D::mousePressEvent(QMouseEvent *pe) //нажатие клавиши мыши
{
    ptrMousePosition = pe->pos(); //вытаскиваем координату указателя мыши
}

void scene3D::mouseReleaseEvent(QMouseEvent *pe) //отжатие клавиши мыши
{

}

void scene3D::mouseMoveEvent(QMouseEvent *pe) //изменение положения мыши
{
    //вычисление углов поворота
    xRot -= 180/nSca*(GLfloat)(pe->y() - ptrMousePosition.y())/height();
    //крутимся вокруг x
    zRot -= 180/nSca*(GLfloat)(pe->x()-ptrMousePosition.x())/width();
    //крутимся вокруг z(вправо-лево)

    ptrMousePosition = pe->pos();
    updateGL();
}
void scene3D::wheelEvent(QWheelEvent *pe)
{
    if ((pe->delta())>0) scale_plus(); else if ((pe->delta())<0) scale_minus();

    updateGL();
}

void scene3D::keyPressEvent(QKeyEvent *pe)
{
    switch(pe->key())
    {
        case Qt::Key_Plus:
            scale_plus();
        break;
        case Qt::Key_Minus:
            scale_minus();
        break;
        case Qt::Key_Equal:
            scale_plus();
        break;
        case Qt::Key_Up:
            rotate_up();
        break;
        case Qt::Key_Down:
            rotate_down();
        break;
        case Qt::Key_Left:
            rotate_left();
        break;
        case Qt::Key_Right:
            rotate_right();
        break;
        case Qt::Key_Z:
            translate_down();
        break;
        case Qt::Key_X:
            translate_up();
        break;
        case Qt::Key_Space:
            defaultScene();
        break;
        case Qt::Key_Escape:
            defaultScene();
        break;
    }
    updateGL();
}

void scene3D::scale_plus() //приблизить сцену
{
    nSca = nSca*1.1;
}


void scene3D::scale_minus()
{
    nSca = nSca/1.1;
}

void scene3D::rotate_up()
{
    xRot +=1.0;
}

void scene3D::rotate_down()
{
    xRot -=1.0;
}

void scene3D::rotate_left()
{
    zRot +=1.0;
}

void scene3D::rotate_right()
{
    zRot-=1.0;
}

void scene3D::translate_down()
{
    zTra -= 0.05;
}

void scene3D::translate_up()
{
    zTra += 0.05;
}


void scene3D::defaultScene()
{
    xRot = -90; yRot=0; zRot=0; zTra=0; nSca=1;
}



void scene3D::drawAxis()
{
    glLineWidth(3.0f);
    glColor4f(1.00f, 0.00f, 0.00f, 1.0f);
    glBegin(GL_LINES);
        glVertex3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-1.0f, 0.0f, 0.0f);
    glEnd();

    QColor halfGreen(0, 128, 0, 255);
    qglColor(halfGreen);
    glBegin(GL_LINES);
        glVertex3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, -1.0f, 0.0f);

        glColor4f(0.00f, 0.00f, 1.00f, 1.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, -1.0f);
        glColor4f(0.00f, 1.00f, 1.00f, 1.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 1.0f);

     glEnd();


}

void scene3D::getVertexArray() //определить массив вершин
{
    GLfloat a = 45*k/n;
    GLfloat x, y, xy, z;

    VertexArray[0][0] = 0; VertexArray[0][1] = 0; VertexArray[0][2] = R; // верзхняя вершина

    for(int i=0; i<n; i++) {
        z = R*cos((i+1)*a);
        xy = R*sin((i+1)*a);
        for (int j = 0; j <= n; j++) {
            VertexArray[i*(n+1)+(j+1)][0] = xy *  cos(2*pi*j/n);
            VertexArray[i*(n+1)+(j+1)][1] = xy *  sin(2*pi*j/n);
            VertexArray[i*(n+1)+(j+1)][2] = z;

        }
    }
}



void scene3D::getSurfArray()
{
    int t = 0;
    //тут код для верхних треугольников

    for (int a=0; a < n; a++) {
        SurfArray[a][0][0] = VertexArray[0][0];    SurfArray[a][0][1] = VertexArray[0][1];    SurfArray[a][0][2] = VertexArray[0][2];
        SurfArray[a][1][0] = VertexArray[a+1][0];  SurfArray[a][1][1] = VertexArray[a+1][1];  SurfArray[a][1][2] = VertexArray[a+1][2];
        SurfArray[a][2][0] = VertexArray[a+2][0];  SurfArray[a][2][1] = VertexArray[a+2][1];  SurfArray[a][2][2] = VertexArray[a+2][2];
        t = a;
    }
    t+=1;
    for (int i=0; i < n; i++) {
        for (int j = 1,  k=t; k< n*(2*(n-1)+1) && j<n+1; j++, k+=2){
                    SurfArray[k][0][0] = VertexArray[i*(n+1)+j][0];     SurfArray[k][0][1] = VertexArray[i*(n+1)+j][1];     SurfArray[k][0][2] = VertexArray[i*(n+1)+j][2];
                    SurfArray[k][1][0] = VertexArray[i*(n+1)+j+1][0];   SurfArray[k][1][1] = VertexArray[i*(n+1)+j+1][1];   SurfArray[k][1][2] = VertexArray[i*(n+1)+j+1][2];
                    SurfArray[k][2][0] = VertexArray[(i+1)*(n+1)+j][0]; SurfArray[k][2][1] = VertexArray[(i+1)*(n+1)+j][1]; SurfArray[k][2][2] = VertexArray[(i+1)*(n+1)+j][2];

                    SurfArray[k+1][0][0] = VertexArray[(i+1)*(n+1)+j][0];     SurfArray[k+1][0][1] = VertexArray[(i+1)*(n+1)+j][1];   SurfArray[k+1][0][2] = VertexArray[(i+1)*(n+1)+j][2];
                    SurfArray[k+1][1][0] = VertexArray[i*(n+1)+j+1][0];       SurfArray[k+1][1][1] = VertexArray[i*(n+1)+j+1][1];     SurfArray[k+1][1][2] = VertexArray[i*(n+1)+j+1][2];
                    SurfArray[k+1][2][0] = VertexArray[(i+1)*(n+1)+j+1][0];   SurfArray[k+1][2][1] = VertexArray[(i+1)*(n+1)+j+1][1]; SurfArray[k+1][2][2] = VertexArray[(i+1)*(n+1)+j+1][2];
                    t = k;
            }
            t += 2;
        }
}

void scene3D::getDopSurf()
{
    int t = 0;

    DopSurf[0][0][0] = 0; DopSurf[0][1][0] = 0; DopSurf[0][2][0] = 0;
    DopSurf[0][1][0] = VertexArray[0][0]; DopSurf[0][1][1] = VertexArray[0][1]; DopSurf[0][1][2] = VertexArray[0][2];
    DopSurf[0][2][0] = VertexArray[1][0]; DopSurf[0][2][1] = VertexArray[1][1]; DopSurf[0][2][2] = VertexArray[1][2];

    for (int i=0, k = t+1; i < n; i++, k++) {
        DopSurf[k][0][0] = 0; DopSurf[k][1][0] = 0; DopSurf[k][2][0] = 0;
        DopSurf[k][1][0] = VertexArray[i*(n+1)+1][0]; DopSurf[k][1][1] = VertexArray[i*(n+1)+1][1]; DopSurf[k][1][2] = VertexArray[i*(n+1)+1][2];
        DopSurf[k][2][0] = VertexArray[(i+1)*(n+1)+1][0]; DopSurf[k][2][1] = VertexArray[(i+1)*(n+1)+1][1]; DopSurf[k][2][2] = VertexArray[(i+1)*(n+1)+1][2];
        t = k;
    }
    t++;
    DopSurf[t][0][0] = 0; DopSurf[t][1][0] = 0; DopSurf[t][2][0] = 0;
    DopSurf[t][1][0] = VertexArray[0][0];       DopSurf[t][1][1] = VertexArray[0][1];   DopSurf[t][1][2] = VertexArray[0][2];
    DopSurf[t][2][0] = VertexArray[n+1][0];     DopSurf[t][2][1] = VertexArray[n+1][1]; DopSurf[t][2][2] = VertexArray[n+1][2];

    for (int i=1, k = t+1; i < n; i++, k++) {
        DopSurf[k][0][0] = 0; DopSurf[k][1][0] = 0; DopSurf[k][2][0] = 0;
        DopSurf[k][1][0] = VertexArray[i*(n+1)][0]; DopSurf[k][1][1] = VertexArray[i*(n+1)][1]; DopSurf[k][1][2] = VertexArray[i*(n+1)][2];
        DopSurf[k][2][0] = VertexArray[(i+1)*(n+1)][0]; DopSurf[k][2][1] = VertexArray[(i+1)*(n+1)][1]; DopSurf[k][2][2] = VertexArray[(i+1)*(n+1)][2];
        t = k;
    }
    t++;


    for (int i=n*n, k = t+1; i <= n*(n+1); i++, k++) {
        DopSurf[k][0][0] = 0; DopSurf[k][1][0] = 0; DopSurf[k][2][0] = 0;
        DopSurf[k][1][0] = VertexArray[i][0]; DopSurf[k][1][1] = VertexArray[i][1]; DopSurf[k][1][2] = VertexArray[i][2];
        DopSurf[k][2][0] = VertexArray[i+1][0]; DopSurf[k][2][1] = VertexArray[i+1][1]; DopSurf[k][2][2] = VertexArray[i+1][2];
        t = k;
    }

}

void scene3D::drawFigure()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    getVertexArray();
    getSurfArray();
    getDopSurf();

    for (int i=0; i < n*(2*(n-1)+1); i++) {
        glVertexPointer(3, GL_FLOAT, 0, SurfArray[i]);
        glNormalPointer(GL_FLOAT, 0, SurfArray[i]);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 3);
        glDrawArrays(GL_LINE_LOOP, 0, 3);
    }

    for (int i=0; i < 3*n+2; i++) {
        glVertexPointer(3, GL_FLOAT, 0, DopSurf[i]);
        glNormalPointer(GL_FLOAT, 0, DopSurf[i]);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 3);
        glDrawArrays(GL_LINE_LOOP, 0, 3);
    }
}

scene3D::~scene3D()
{
}



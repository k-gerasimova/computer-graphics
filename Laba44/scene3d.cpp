#include "scene3d.h"
#include <QtGui>
#include <math.h>

const static float pi = 3.141593, k=pi/180; //перевод из градусов в радианы

GLfloat VertexArray[10][3]; //массив вершин 20-количество вершин
GLfloat VertexArray2[10][3];
GLfloat ColorArray[20][3]; //массив цветов вершин
GLfloat SurfArray[12][4][3]; //массив поверхностей
GLfloat light2_position[4] = {1.2, 1.1, 1.2, 1};

scene3D::scene3D(QWidget *parent)
    : QGLWidget(parent)
{
    xRot = -90; yRot =0; zRot=0; zTra=0; nSca=1; //начальные значения полей

}

void scene3D::initializeGL()
{
    qglClearColor(Qt::black); //устанавливает белый экран
    glEnable(GL_DEPTH_TEST); //устанавливаем режим проверки глубины пикселей(чтобы
    //определять насколько далеко от нас поверхность
    //glShadeModel(GL_FLAT); //отключаем режим сглаживания цветов
    //glEnable(GL_CULL_FACE); //устанавливаем режим построения только внешних поверхностей

    glEnable(GL_LIGHTING); // включаем свет

    GLfloat light_ambient[3] = {3, 2, 2};

    glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
    glShadeModel(GL_SMOOTH);
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glEnable(GL_NORMALIZE);

   // getVertexArray(); //определяем массив вершин
    //getColorArray(); //определяем массив цветов вершин
   // getIndexArray(); //определяем массив индексов вершин

    //glEnableClientState(GL_VERTEX_ARRAY); //АКТИВИЗАЦИЯ МАССИВА ВЕРШИН,
    //т е разрешаем менять массив вершин пользователю
    //glEnableClientState(GL_COLOR_ARRAY); //активизация массива цветов
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

    GLfloat light2_diffuse[] = { 0.5, 0.5, 0.7 };

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
}




void scene3D::drawFigure()
{
    GLfloat R = 0.75;
    GLint n = 10;
    GLfloat a = 36*k;

//    GLfloat normal[] = {0, 0, 1,  1, 0, 0,  -1, 0, 0,  cos(2*a), sin(2*a), 0,
  //                      cos(a), sin(a), 0,   cos(a), -sin(a), 0,  cos(2*a), -sin(2*a), 0,
    //                   -cos(2*a), -sin(2*a), 0,   -cos(2*a), -sin(2*a), 0,  -cos(a), -sin(a), 0,
      //                 -cos(a), sin(a), 0,  -cos(2*a), sin(2*a), 0};

 //   glEnableClientState(GL_NORMAL_ARRAY);

   // glNormal3f(0, 0, 1);
   /* glNormal3f(0, 0, -1);
    glNormal3f(1, 0, 0);
    glNormal3f(-1, 0, 0);
    glNormal3f(cos(2*a), sin(2*a), 0);
    glNormal3f(cos(a), sin(a), 0);
    glNormal3f(cos(a), -sin(a), 0);
    glNormal3f(cos(2*a), -sin(2*a), 0);

    glNormal3f(-cos(2*a), -sin(2*a), 0);
    glNormal3f(-cos(a), -sin(a), 0);
    glNormal3f(-cos(a), sin(a), 0);
    glNormal3f(-cos(2*a), sin(2*a), 0);
*/



    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    VertexArray[0][0] = 0;          VertexArray[0][1] = R; VertexArray[0][2] = -0.5f; //1
    VertexArray[1][0] = R*sin(a);   VertexArray[1][1] = R*cos(a); VertexArray[1][2] = -0.5f; //2
    VertexArray[2][0] = R*sin(2*a); VertexArray[2][1] = R*cos(a*2); VertexArray[2][2] = -0.5f; //3
    VertexArray[3][0] = R*sin(a*2); VertexArray[3][1] = -R*cos(2*a); VertexArray[3][2] = -0.5f; //4
    VertexArray[4][0] = R*sin(a);   VertexArray[4][1] = -R*cos(a); VertexArray[4][2] = -0.5f; //5
    VertexArray[5][0] = 0;          VertexArray[5][1] = -R; VertexArray[5][2] = -0.5f; //6
    VertexArray[6][0] = -R*sin(a);  VertexArray[6][1] = -R*cos(a); VertexArray[6][2] = -0.5f; //7
    VertexArray[7][0] = -R*sin(a*2);VertexArray[7][1] = -R*cos(2*a); VertexArray[7][2] = -0.5f; //8
    VertexArray[8][0] = -R*sin(a*2);VertexArray[8][1] = R*cos(2*a); VertexArray[8][2] = -0.5f; //9
    VertexArray[9][0] = -R*sin(a);  VertexArray[9][1] = R*cos(a); VertexArray[9][2] = -0.5f;//10

    glColor3ub(155,100,0);
    glVertexPointer(3, GL_FLOAT, 0, VertexArray);
    glNormalPointer(GL_FLOAT, 0, VertexArray);


    glDrawArrays(GL_POLYGON, 0, 10);
 //   glColor3ub(0,0,0);
 //   glDrawArrays(GL_LINE_LOOP, 0, 10);

    VertexArray2[0][0] = 0;          VertexArray2[0][1] = R; VertexArray2[0][2] = 0.5f;
    VertexArray2[1][0] = R*sin(a);   VertexArray2[1][1] = R*cos(a); VertexArray2[1][2] = 0.5f;
    VertexArray2[2][0] = R*sin(2*a); VertexArray2[2][1] = R*cos(a*2); VertexArray2[2][2] = 0.5f;
    VertexArray2[3][0] = R*sin(a*2); VertexArray2[3][1] = -R*cos(2*a); VertexArray2[3][2] = 0.5f;
    VertexArray2[4][0] = R*sin(a);   VertexArray2[4][1] = -R*cos(a); VertexArray2[4][2] = 0.5f;
    VertexArray2[5][0] = 0;          VertexArray2[5][1] = -R; VertexArray2[5][2] = 0.5f;
    VertexArray2[6][0] = -R*sin(a);  VertexArray2[6][1] = -R*cos(a); VertexArray2[6][2] = 0.5f;
    VertexArray2[7][0] = -R*sin(a*2);VertexArray2[7][1] = -R*cos(2*a); VertexArray2[7][2] = 0.5f;
    VertexArray2[8][0] = -R*sin(a*2);VertexArray2[8][1] = R*cos(2*a); VertexArray2[8][2] = 0.5f;
    VertexArray2[9][0] = -R*sin(a);  VertexArray2[9][1] = R*cos(a); VertexArray2[9][2] = 0.5f;

    glColor3ub(155,100,0);
    glVertexPointer(3, GL_FLOAT, 0, VertexArray2);
    glNormalPointer(GL_FLOAT, 0, VertexArray2);
    glDrawArrays(GL_POLYGON, 0, 10);
  //  glColor3ub(0,0,0);
  //  glDrawArrays(GL_LINE_LOOP, 0, 10);


    for (int i=0; i<10; i++) {
        SurfArray[i][0][0] = VertexArray[i][0];         SurfArray[i][0][1] = VertexArray[i][1];         SurfArray[i][0][2] = VertexArray[i][2];
        SurfArray[i][1][0] = VertexArray[(i+1)%10][0];  SurfArray[i][1][1] = VertexArray[(i+1)%10][1];  SurfArray[i][1][2] = VertexArray[(i+1)%10][2];
        SurfArray[i][2][0] = VertexArray2[(i+1)%10][0]; SurfArray[i][2][1] = VertexArray2[(i+1)%10][1]; SurfArray[i][2][2] = VertexArray2[(i+1)%10][2];
        SurfArray[i][3][0] = VertexArray2[i][0];        SurfArray[i][3][1] = VertexArray2[i][1];        SurfArray[i][3][2] = VertexArray2[i][2];

    }



    for (int i=0; i < 10; i++) {

    //    glColor3ub(155,100,0);
        glVertexPointer(3, GL_FLOAT, 0, SurfArray[i]);

        glDrawArrays(GL_QUADS, 0, 4);
       // glColor3ub(0,0,0);
       // glDrawArrays(GL_LINE_LOOP, 0, 4);

    }





}


scene3D::~scene3D()
{
}


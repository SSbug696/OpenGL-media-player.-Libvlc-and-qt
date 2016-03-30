#include "VideoShow.h"
#include "Player.h"
#include "Shaders.h"
#include "Anaglyph.h"
#include <vlc/vlc.h>
#include <QOpenGLWidget>
#include <ctime>
#include <QGLShader>
#include <QtCore/QtGlobal>
#include <QFileDialog>
#include <QOpenGLShaderProgram>
#include <time.h>
#include <unistd.h>
#include <QtGui>
#include <QDebug>
#include <iostream>

#define OFFSET_BUFFER(bytes) ((GLfloat *)NULL + bytes)
float   scaleX = 1.0f, scaleY = 1.0f,
        coefX = 1.0,   coefY = 1.0;


GLuint Program;
// ID attribute
GLint  Attrib_vertex;
// ID uniform val of color
GLint  Unif_color;
// ID Vertex buffer object
GLuint VBO, VBO_2, VBOt;

// ! Вершина
struct vertex
{
  GLfloat x;
  GLfloat y;
};


float mVerticesData[] = { -1.f, 1.f, 0.0f, // Position 0
        0.0f, 0.0f, // TexCoord 0
        -1.f, -1.f, 0.0f, // Position 1
        0.0f, 1.0f, // TexCoord 1
        1.f, -1.f, 0.0f, // Position 2
        1.0f, 1.0f, // TexCoord 2
        1.f, 1.f, 0.0f, // Position 3
        1.0f, 0.0f // TexCoord 3
};

// Init VBO for 2.x textures
void VideoShow::initVBO(){
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(__vertex), __vertex,  GL_DYNAMIC_DRAW);

    glGenBuffers(1, &VBO_2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(__vertex_2), __vertex_2,  GL_DYNAMIC_DRAW);


    glGenBuffers(1, &VBOt);
    glBindBuffer(GL_TEXTURE_BUFFER, VBOt);
    glBufferData(GL_TEXTURE_BUFFER, sizeof(_textureCords), _textureCords,  GL_DYNAMIC_DRAW);
}

void VideoShow::modeAnaglyph(){
    active_anaglyph = active_anaglyph == 1.0 ? 0.0:1.0;
    qDebug() << "anaglyph;";
}


VideoShow::VideoShow(QWidget *parent) : QGLWidget(parent)
{
    std::cout << glGetString(GL_VERSION) <<  "test " << std::endl;
    active_anaglyph = 0.0;

    ptrPlayer = (Player*)parent;
    this->setGeometry(0, 0, 1200, 850);
    this->setBaseSize(QSize(1200, 850));
    this->setMouseTracking(true);
    this->setStyleSheet(QString("background-color: #000;width:100%;height:100%;"));
    this->setAutoFillBackground(true);


    _textureCords[0]=1.0f;
    _textureCords[1]=0.0f,
    _textureCords[2]=1.0f;
    _textureCords[3]=1.0f,
    _textureCords[4]=0.0f;
    _textureCords[5]=1.0f;
    _textureCords[6]=1.0f;
    _textureCords[7]=0.0f;


    __vertex[0] = 0.0;
    __vertex[1] = 1.0;
    __vertex[2] = 1.0;
    __vertex[3] = 1.0;
    __vertex[4] = 1.0;
    __vertex[5] = 0.0;
    __vertex[6] = 0.0;
    __vertex[7] = 0.0;

    _logoVertex[0][0] =  0.3f;
    _logoVertex[0][1] =  0.3f;
    _logoVertex[1][0] =  0.3f;
    _logoVertex[1][1] = -0.3f;
    _logoVertex[2][0] = -0.3f;
    _logoVertex[2][1] = -0.3f;
    _logoVertex[3][0] = -0.3f;
    _logoVertex[3][1] =  0.3f;

    _stereo = new Anaglyph(2000.0f,
                          0.025f,
                          1.1333f,
                          45.0f,
                          0.0f,
                          10000.0f
                          );

    this->show();
}






void VideoShow::initShader()
{
      GLuint vShader, fShader;

      // Init vertex and textures shaders
      vShader = glCreateShader(GL_VERTEX_SHADER);
      glShaderSource(vShader, 1, &YUV420P_VS, NULL);
      glCompileShader(vShader);

      fShader = glCreateShader(GL_FRAGMENT_SHADER);
      glShaderSource(fShader, 1, &FProgram, NULL);
      glCompileShader(fShader);

      prog = glCreateProgram();
      glAttachShader(prog, vShader);
      glAttachShader(prog, fShader);

      glLinkProgram(prog);
      glUseProgram(prog);



      glUniform1i(glGetUniformLocation(prog, "y_tex"), 0);
      glUniform1i(glGetUniformLocation(prog, "u_tex"), 1);
      glUniform1i(glGetUniformLocation(prog, "v_tex"), 2);
      glUniform1f(glGetUniformLocation(prog, "scale"), 5.0);

      const char* attr_name = "scale";
      Attrib_vertex = glGetUniformLocation(prog, attr_name);

      y_tex_s = glGetUniformLocation(prog, "y_tex");
      u_tex_s = glGetUniformLocation(prog, "u_tex");
      v_tex_s = glGetUniformLocation(prog, "v_tex");

      u_pos = glGetUniformLocation(prog, "position");
      pos_m = glGetUniformLocation(prog, "pos_m");
      tex_coord = glGetUniformLocation(prog, "tex");
      anaglyph = glGetUniformLocation(prog, "is_anaglyph");
      size_frame = glGetUniformLocation(prog, "size_frame");
}


void VideoShow::setupTextures() {

  glUseProgram(prog);

  glGenTextures(1, &y_tex);
  glBindTexture(GL_TEXTURE_2D, y_tex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA , _frame_width/2, _frame_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


  glGenTextures(1, &u_tex);
  glBindTexture(GL_TEXTURE_2D, u_tex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _frame_width/2, _frame_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

}


GLuint glTexture, y_tex, u_tex, v_tex;
void VideoShow::initializeGL(){
    initializeGLFunctions();
    //QOpenGLFunctions();
    //initializeOpenGLFunctions();
    //initializeOpenGLFunctions();
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glEnable( GL_TEXTURE_2D );
    logoInit();

}


void VideoShow::run(int frame_width, int frame_height){
    _data_frame = ptrPlayer->mediaPlayer->getPixels();

    _ratio_x =  1.0f;
    _ratio_y =  1.0f;

    _frame_width = frame_width;
    _frame_height= frame_height;

    _texture_height = frame_height;
    _texture_width  = frame_width;


    initVBO();
    initShader();
    setupTextures();
   // glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA , _texture_width, _texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _data_frame );
   // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    resizeGL(this->width(), this->height());
    //setup();
    ptrPlayer->mediaPlayer->state = 1;
}


void VideoShow::paintGL(){
    glUniform1f(anaglyph, active_anaglyph);
    if( ptrPlayer->mediaPlayer->state == 1){
        _stereo->ApplyLeftFrustum();
        videoRender();

        glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_TRUE);
        glClear(GL_DEPTH_BUFFER_BIT) ;

        _stereo->ApplyRightFrustum();
        videoRender();
        glColorMask(GL_FALSE, GL_TRUE, GL_TRUE, GL_FALSE);
    } else {
        showLogo();
    }
}


int width_, height_;

void VideoShow::videoRender()
{
    glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glUseProgram(prog);

    glUniform1f(Attrib_vertex, 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, y_tex);
    glUniform1i(y_tex_s, 0);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _frame_width/2, _frame_height, GL_RGBA, GL_UNSIGNED_BYTE, _data_frame);


    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glEnableVertexAttribArray(u_pos);
    glVertexPointer(2, GL_FLOAT, 0, 0);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glDrawArrays(GL_QUADS, 0, 4);

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);


    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, _vertex);
    glTexCoordPointer(2, GL_FLOAT, 0, &_textureCords);
    glDisableClientState(GL_VERTEX_TEXTURE);

    glDisableClientState(GL_VERTEX_TEXTURE);
    glDisableClientState(GL_VERTEX_ARRAY);
}


void VideoShow::changeRatio(){
/*
    if(_frame_height < _frame_width){
        _ratio_y = _frame_width/_frame_height;
        _ratio_x = 1.0;
    } else {
        _ratio_y = 1.0;
        _ratio_x = _frame_height/_frame_width;
    }
 */


    if(_frame_height<_frame_width){
        float _w = _frame_width;
        float _h = (_frame_width/16) * 9;
        _frame_width = _h * _frame_width/_frame_height;
        _ratio_y = _w/_h;
    } else {
        float _w = _frame_width;
        float _h = (_frame_width/16) * 9;
        _ratio_y = _h/_w;
    }

    resizeGL(this->width(), this->height());
}


void VideoShow::resizeGL(int width, int height){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float w = (float)width;
    float h = (float)height;

    float c_x = (float)w/(float)h;
    float c_y = (float)h/(float)w;


    _ratio_coeff_x=2.0 * _ratio_x,
    _ratio_coeff_y=2.0 * _ratio_y;


    if(w/h > (float)_frame_width/(float)_frame_height){
        _ratio_coeff_y =  _ratio_coeff_y/h * h;
        _ratio_coeff_x =  _ratio_coeff_x/w * (h * _frame_width/_frame_height);

    } else if(h/w >= c_y){
        _ratio_coeff_x = _ratio_coeff_x/w * w;
        _ratio_coeff_y = _ratio_coeff_y/h * (w * _frame_height/_frame_width);
    }

    qDebug() << _ratio_coeff_x << " " << _ratio_coeff_y;


    glUniform2f(size_frame, _ratio_coeff_x, _ratio_coeff_y);

    width_ = width;
    height_ = height;

    glViewport(0, 0, (GLint)width, (GLint)height);
}

// Keyboard control
void VideoShow::keyPressEvent(QKeyEvent* pe)
{
   switch (pe->key())
   {
      case Qt::Key_Z:

        changeRatio();
      break;

      case Qt::Key_X:
      break;

      case Qt::Key_Space:
      break;

      case Qt::Key_Escape:
      break;

       case Qt::Key_Plus:
       break;

       case Qt::Key_Equal:
       break;

       case Qt::Key_Minus:
       break;

       case Qt::Key_Up:
       break;

       case Qt::Key_Down:
       break;

       case Qt::Key_Left:
       break;

       case Qt::Key_Right:
       break;

   }

   //updateGL();
}


void VideoShow::logoInit(){
    QImage * img=new QImage(":img/logo.png");
    _img = img->convertToFormat(QImage::Format_RGBA8888);

    _logo_width = img->width();
    _logo_height= img->height();

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA , _logo_width, _logo_height, 0, GL_RGBA , GL_UNSIGNED_INT_8_8_8_8_REV, _img.bits());
    glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

}


void VideoShow::showLogo(){
    glTexSubImage2D ( GL_TEXTURE_2D, 0, 0, 0, _logo_width, _logo_height, GL_BGRA, GL_UNSIGNED_BYTE, _img.bits());
    glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glVertexPointer(2, GL_FLOAT, 0, _logoVertex);
    glTexCoordPointer(2, GL_FLOAT, 0, &_textureCords);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_POLYGON, 0, 4);
    glDisableClientState(GL_VERTEX_TEXTURE);
    glDisableClientState(GL_VERTEX_ARRAY);

}

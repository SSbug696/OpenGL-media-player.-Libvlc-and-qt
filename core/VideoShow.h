#ifndef VIDEOSHOW_H
#define VIDEOSHOW_H
#include <math.h>
#include <QObject>
#include <QWidget>
#include <QGLWidget>
#include <QTimer>

#include <QOpenGLTexture>
#include <QOpenGLWidget>
#include <QGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLFunctions_3_3_Compatibility>

class Player;
class StereoCamera;
class Anaglyph;


class VideoShow : public QGLWidget, protected QGLFunctions
{
    Q_OBJECT

protected:
    int _texture_width,
        _texture_height;

    Anaglyph * _stereo;

    void paintGL();
    void initializeGL();
    void resizeGL(int, int);
    void videoRender();
    void keyPressEvent(QKeyEvent *);
    void initShader();
    void initVBO();
    void freeShader();
    void freeVBO();
    void setupTextures();
    void setup();

    void setYPixels(uint8_t*, int);
    void setVPixels(uint8_t*, int);
    void setUPixels(uint8_t*, int);


  // Here you cant bind new keyboard events
  /*void mousePressEvent(QMouseEvent* pe);
    void mouseMoveEvent(QMouseEvent* pe);
    void mouseReleaseEvent(QMouseEvent* pe);
    void wheelEvent(QWheelEvent* pe);
    void keyPressEvent(QKeyEvent* pe);
*/

private:
    enum mode{ SIMPLE, STEREO, ANAGLYPH };
    enum ratioMode{DEFAULT, STANDART};
    mode _current_mode, _current_ratio;

    GLuint vao;
    uint8_t* y_pixels;
    uint8_t* u_pixels;
    uint8_t* v_pixels;



    GLuint y_tex_s;
    GLuint u_tex_s;
    GLuint v_tex_s;


    GLuint y_tex;
    GLuint sampler_texture;
    GLuint u_tex;
    GLuint v_tex;
    GLuint vert;
    GLuint frag;
    GLuint prog;
    GLint  u_pos, pos_m;
    GLint  size_frame;
    GLint  tex_coord;
    GLint anaglyph;
    float active_anaglyph;

    struct vertex
    {
      GLfloat x;
      GLfloat y;
    };

    GLfloat __vertex[8], __vertex_2[8];

    GLfloat _vertex[4][2], _logoVertex[4][2];
    float _textureCords[8];
    unsigned char * _data_frame;
    unsigned char data_logo[200*200*4];

    QImage  _logo_texture, _img;
    QOpenGLTexture * texture;

    int _frame_width, _frame_height;
    int _logo_width, _logo_height;
    float _ratio_coeff_x, _ratio_coeff_y;
    float _ratio_x, _ratio_y;

    void logoInit();
    void showLogo();

public:
    VideoShow(QWidget *parent);


    void run(int, int);
    void updateSizeVideoFrame();
    void changeRatio();
    void modeAnaglyph();

    Player * ptrPlayer;

signals:

public slots:

};

#endif // VIDEOSHOW_H

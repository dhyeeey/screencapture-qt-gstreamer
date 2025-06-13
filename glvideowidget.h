#ifndef GLVIDEOWIDGET_H
#define GLVIDEOWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLShader>
#include <QOpenGLExtraFunctions>
#include "screencaptureprocessor.h"

class GLVideoWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit GLVideoWidget(QWidget* parent = nullptr, FrameData *framedata=nullptr);
    ~GLVideoWidget();

    void updateframe();
    void setExternalBuffer(unsigned char* buffer, int width, int height);
    void setExternalBuffer(int width, int height);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

private:
    void updateTexture();

    FrameData *framedata;
    unsigned char* m_buffer = nullptr;
    int m_width = 0;
    int m_height = 0;
    int m_frame;

    QOpenGLShaderProgram m_program;
    QOpenGLBuffer m_vbo;
    GLuint m_texture = 0;

    GLint m_posAttr = -1;
    GLint m_texAttr = -1;
    GLint m_samplerUniform = -1;

    GLint m_imageAspectUniform = -1;
    GLint m_widgetAspectUniform = -1;
};

#endif // GLVIDEOWIDGET_H

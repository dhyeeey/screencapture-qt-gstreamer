#include "glvideowidget.h"

GLVideoWidget::GLVideoWidget(QWidget* parent, FrameData *framedata)
    : QOpenGLWidget(parent), m_vbo(QOpenGLBuffer::VertexBuffer), framedata(framedata) {

}

GLVideoWidget::~GLVideoWidget() {
    makeCurrent();
    glDeleteTextures(1, &m_texture);
    if (m_buffer) {
        delete[] m_buffer;
        m_buffer = nullptr;
    }
    m_vbo.destroy();
    doneCurrent();
}

void GLVideoWidget::setExternalBuffer(unsigned char* buffer, int width, int height) {
    m_buffer = buffer;
    m_width = width;
    m_height = height;
    m_frame = 0;
}

void GLVideoWidget::setExternalBuffer(int width, int height) {
    m_buffer = nullptr;
    m_width = width;
    m_height = height;
    m_frame = 0;
}

void GLVideoWidget::initializeGL() {
    initializeOpenGLFunctions();

    // Vertex and Fragment Shader
    m_program.addShaderFromSourceCode(QOpenGLShader::Vertex, R"(
        attribute vec2 position;
        attribute vec2 texcoord;
        varying vec2 v_texcoord;
        void main() {
            v_texcoord = texcoord;
            gl_Position = vec4(position, 0.0, 1.0);
        }
    )");

    m_program.addShaderFromSourceCode(QOpenGLShader::Fragment, R"(
        uniform sampler2D tex;
        varying vec2 v_texcoord;

        uniform float imageAspect;
        uniform float widgetAspect;

        void main() {
            float scaleX = 1.0;
            float scaleY = 1.0;

            vec2 uv = v_texcoord;

            // Correct UV coordinates based on aspect ratio
            if (imageAspect > widgetAspect) {
                scaleY = widgetAspect / imageAspect;
                uv.y = (uv.y - 0.5) / scaleY + 0.5;
            } else {
                scaleX = imageAspect / widgetAspect;
                uv.x = (uv.x - 0.5) / scaleX + 0.5;
            }

            // If outside bounds, discard the pixel (black bars)
            if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0)
                gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0); // Black bars
            else
                gl_FragColor = texture2D(tex, uv);
        }
    )");

    m_program.link();
    m_posAttr = m_program.attributeLocation("position");
    m_texAttr = m_program.attributeLocation("texcoord");
    m_samplerUniform = m_program.uniformLocation("tex");

    m_imageAspectUniform = m_program.uniformLocation("imageAspect");
    m_widgetAspectUniform = m_program.uniformLocation("widgetAspect");

    float vertices[] = {
        -1, -1, 0, 1,
        1, -1, 1, 1,
        -1,  1, 0, 0,
        1,  1, 1, 0,
    };

    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(vertices, sizeof(vertices));
    m_vbo.release();

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void GLVideoWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void GLVideoWidget::paintGL() {
    if (!framedata->data || framedata->width == 0 || framedata->height == 0)
        return;

    glClear(GL_COLOR_BUFFER_BIT);
    updateTexture();

    // Update aspect ratio uniforms
    m_program.bind();
    m_program.setUniformValue(m_imageAspectUniform, float(framedata->width) / float(framedata->height));
    m_program.setUniformValue(m_widgetAspectUniform, float(width()) / float(height()));

    m_program.setUniformValue(m_samplerUniform, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    m_vbo.bind();
    m_program.enableAttributeArray(m_posAttr);
    m_program.enableAttributeArray(m_texAttr);

    m_program.setAttributeBuffer(m_posAttr, GL_FLOAT, 0, 2, 4 * sizeof(float));
    m_program.setAttributeBuffer(m_texAttr, GL_FLOAT, 2 * sizeof(float), 2, 4 * sizeof(float));

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    m_program.disableAttributeArray(m_posAttr);
    m_program.disableAttributeArray(m_texAttr);

    m_vbo.release();
    glBindTexture(GL_TEXTURE_2D, 0);
    m_program.release();
}

// void GLVideoWidget::updateTexture() {
//     glBindTexture(GL_TEXTURE_2D, m_texture);
//     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_buffer);
// }

// void GLVideoWidget::updateTexture() {
//     glBindTexture(GL_TEXTURE_2D, m_texture);
//     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_BGRA, GL_UNSIGNED_BYTE, m_buffer);
// }

void GLVideoWidget::updateTexture() {
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, framedata->width, framedata->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, framedata->data);
    // qDebug() << "Update Texture called ...";
}

void GLVideoWidget::updateframe(){
    update();
}

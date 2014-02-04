#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtOpenGL>
#include <QGLWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    QGLFormat glf = QGLFormat::defaultFormat();
    glf.setRedBufferSize(8);
    glf.setGreenBufferSize(8);
    glf.setBlueBufferSize(8);
    glf.setAlphaBufferSize(8);
    glf.setSampleBuffers(true);
    glf.setSamples(8);
    glf.setDepth(true);
    glf.setDepthBufferSize(24);
    glf.setVersion(3, 3);
    glf.setProfile(QGLFormat::CompatibilityProfile);
    glf.setSwapInterval(1);

    QSharedPointer<QGLContext> gl_context = QSharedPointer<QGLContext>(new QGLContext(glf));
    gl_context->create();
    gl_context->makeCurrent();

}

MainWindow::~MainWindow() {
    delete ui;
}

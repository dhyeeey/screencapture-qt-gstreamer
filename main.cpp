#include <QApplication>
#include "screencaptureprocessor.h"
#include "glvideowidget.h"
#include <QThread>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    std::atomic_bool keepRunning = true;

    FrameData framedata;
    ScreenCaptureProcessor capturer;

    qDebug() << "Entered main method ...";

    if (!capturer.start()) {
        std::cerr << "Failed to start screen capture pipeline.\n";
        return -1;
    }

    GLVideoWidget widget(nullptr,&framedata);
    widget.resize(900, 700);
    widget.setExternalBuffer(1920, 1080);
    widget.show();

    QThread* thread = new QThread;

    QObject::connect(thread, &QThread::started,[&]{
        while(keepRunning){
            // qDebug() << "Started while loop ..";
            capturer.getNextFrame(framedata);
            widget.updateframe();
        }
    });

    QObject::connect(&a, &QApplication::aboutToQuit, [&](){
        qDebug() << "Application is quitting. Cleaning up...";
        keepRunning = false;
        thread->quit();
        thread->wait();

        capturer.stop();
    });

    thread->start();
    return a.exec();
}

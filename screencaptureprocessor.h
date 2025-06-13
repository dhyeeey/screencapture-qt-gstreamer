#ifndef SCREENCAPTUREPROCESSOR_H
#define SCREENCAPTUREPROCESSOR_H

#include <gstreamer-1.0/gst/gst.h>
#include <gstreamer-1.0/gst/app/gstappsink.h>
#include <cstdint>
#include <cstddef>
#include <iostream>
#include <QDebug>

using namespace std;

// Struct to hold frame data
struct FrameData {
    uchar* data = nullptr;
    size_t size = 0;
    int width = 0;
    int height = 0;

    ~FrameData() {
        if (data) {
            delete[] data;
            data = nullptr;
        }
    }
};

class ScreenCaptureProcessor {
public:
    ScreenCaptureProcessor();
    ~ScreenCaptureProcessor();

    bool start();                          // Initializes GStreamer and starts the pipeline
    void stop();                           // Stops the pipeline
    bool getNextFrame(FrameData& frame);   // Pulls the next frame and fills FrameData
    void releaseFrame(FrameData& frame);   // Unmaps and unreferences the sample

private:
    GstElement* pipeline;
    GstElement* appsink;

    void cleanup();                        // Internal resource cleanup
};

#endif // SCREENCAPTUREPROCESSOR_H

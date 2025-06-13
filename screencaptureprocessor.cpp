#include "screencaptureprocessor.h"
#include <QDebug>

ScreenCaptureProcessor::ScreenCaptureProcessor()
    : pipeline(nullptr), appsink(nullptr) {}

ScreenCaptureProcessor::~ScreenCaptureProcessor() {
    stop();
    cleanup();
}

bool ScreenCaptureProcessor::start() {
    gst_init(nullptr, nullptr);

    pipeline = gst_parse_launch(
        "d3d11screencapturesrc capture-api=dxgi show-cursor=true ! "
        "queue ! d3d11convert ! "
        "queue ! d3d11download ! "
        "queue ! videoconvert ! "
        "queue ! video/x-raw,format=RGBA ! "
        "appsink name=sink sync=false", NULL
    );

    if (!pipeline) {
        std::cerr << "Failed to create pipeline.\n";
        return false;
    }

    qDebug() << "Pipeline done ...";

    appsink = gst_bin_get_by_name(GST_BIN(pipeline), "sink");
    if (!appsink) {
        std::cerr << "Failed to get appsink from pipeline.\n";
        return false;
    }

    qDebug() << "appsink done ...";

    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    qDebug() << "gst_element_set_state done ...";
    qDebug() << "Exiting start method of screencapturer...";

    return true;
}

void ScreenCaptureProcessor::stop() {
    if (pipeline) {
        gst_element_set_state(pipeline, GST_STATE_NULL);
    }
}

bool ScreenCaptureProcessor::getNextFrame(FrameData& frame) {
    GstSample* sample = gst_app_sink_pull_sample(GST_APP_SINK(appsink));
    if (!sample) {
        std::cerr << "Failed to pull sample from appsink.\n";
        return false;
    }

    GstBuffer* buffer = gst_sample_get_buffer(sample);
    GstCaps* caps = gst_sample_get_caps(sample);
    GstStructure* s = gst_caps_get_structure(caps, 0);

    int width = 0, height = 0;
    gst_structure_get_int(s, "width", &width);
    gst_structure_get_int(s, "height", &height);

    GstMapInfo map;
    if (!gst_buffer_map(buffer, &map, GST_MAP_READ)) {
        std::cerr << "Failed to map buffer.\n";
        gst_sample_unref(sample);
        return false;
    }

    if (!frame.data || frame.size != map.size) {
        if(frame.size != map.size && frame.data != nullptr){
            delete[] frame.data;
        }
        frame.data = new uchar[map.size];
    }

    std::memcpy(frame.data, map.data, map.size);
    frame.size = map.size;
    frame.width = width;
    frame.height = height;

    // qDebug() << "Width : " << frame.width << "Height : " << frame.height << "Size : " << frame.size;

    gst_sample_unref(sample);
    gst_buffer_unmap(buffer,&map);

    return true;
}

void ScreenCaptureProcessor::cleanup() {
    if (appsink) {
        gst_object_unref(appsink);
        appsink = nullptr;
    }

    if (pipeline) {
        gst_object_unref(pipeline);
        pipeline = nullptr;
    }
}

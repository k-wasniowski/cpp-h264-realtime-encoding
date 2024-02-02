module;

#include <opencv2/opencv.hpp>

#include <thread>
#include <iostream>

export module Camera;

import VideoFrame;

[[noreturn]] void run_camera(cv::VideoCapture &&camera, VideoFrame::VideoFramesQueue &queue) {
    cv::Mat frame;

    while (true) {
        camera >> frame;

        if (frame.empty()) {
            std::cout << "Failed to capture frame" << std::endl;
            continue;
        }

        VideoFrame::Frame videoFrame{};
        if (frame.isContinuous()) {
            videoFrame.data.assign((uchar*)frame.datastart, (uchar*)frame.dataend);
        } else {
            continue;
        }

        queue.push(videoFrame);
    }
}

export namespace Camera {
    bool run(VideoFrame::VideoFramesQueue &queue) {
        cv::VideoCapture camera{};

        auto result = camera.open(0, cv::CAP_ANY);
        if (!result) {
            std::cout << "Failed to open camera" << std::endl;
            return false;
        }

        auto cameraThread = std::thread{run_camera, std::move(camera), std::ref(queue)};
        cameraThread.detach();

        return true;
    }
}

module;

#include <opencv2/opencv.hpp>

#include <thread>
#include <iostream>

export module Camera;

import VideoFrame;

[[noreturn]] void run_camera(cv::VideoCapture &&camera, VideoFrame::VideoFramesQueue &queue) {
    cv::Mat frame;

    std::string windowName = "Camera";

    while (true) {
        camera >> frame;

        if (frame.empty()) {
            std::cout << "Failed to capture frame" << std::endl;
            continue;
        }

        cv::Mat yuvFrame{};
        cv::cvtColor(frame, yuvFrame, cv::COLOR_BGR2YUV);

        cv::imshow(windowName, yuvFrame);

        cv::waitKey(25);

        std::vector<cv::Mat> channels;
        cv::split(yuvFrame, channels);

        VideoFrame::Frame videoFrame{};
        videoFrame.strideY = channels[0].step;
        videoFrame.strideU = channels[1].step;
        videoFrame.strideV = channels[2].step;
        videoFrame.dataY.assign(channels[0].datastart, channels[0].dataend);
        videoFrame.dataU.assign(channels[1].datastart, channels[1].dataend);
        videoFrame.dataV.assign(channels[2].datastart, channels[2].dataend);

        queue.push(videoFrame);
    }
}

export namespace Camera {
    struct Info {
        int width;
        int height;
    };

    std::optional<Info> run(VideoFrame::VideoFramesQueue &queue) {
        cv::VideoCapture camera{};

        auto result = camera.open(0, cv::CAP_ANY);
        if (!result) {
            std::cout << "Failed to open camera" << std::endl;
            return std::nullopt;
        }

        auto width = camera.get(cv::CAP_PROP_FRAME_WIDTH);
        auto height = camera.get(cv::CAP_PROP_FRAME_HEIGHT);

        std::cout << "Camera resolution: " << width << "x" << height << std::endl;

        auto cameraThread = std::thread{run_camera, std::move(camera), std::ref(queue)};
        cameraThread.detach();

        return Info{static_cast<int>(width), static_cast<int>(height)};
    }
}

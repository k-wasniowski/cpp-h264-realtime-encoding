module;

#include <iostream>

export module Encoder;

import VideoFrame;

export namespace Encoder {
    void run(VideoFrame::VideoFramesQueue &queue) {
        while (true) {
            auto frame = queue.pop();

            std::cout << "Frame received" << std::endl;

        }
    }
}
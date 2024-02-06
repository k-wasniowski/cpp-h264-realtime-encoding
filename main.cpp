import Camera;
import VideoFrame;
import Encoder;

#include <iostream>
#include <fstream>

int main() {
    VideoFrame::VideoFramesQueue queue{};

    auto oCameraInfo = Camera::run(queue);
    if (!oCameraInfo.has_value()) {
        std::cout << "Failed to start camera" << std::endl;
        return -1;
    }

    std::cout << "HET" << std::endl;

    VideoFrame::EncodedVideoFramesQueue encodedVideoFramesQueue{};

    Encoder::run(oCameraInfo.value(), queue, encodedVideoFramesQueue);

    std::ofstream myfile{};
    myfile.open("encoded.h264", std::ios::binary);

    while (true) {
        auto frame = encodedVideoFramesQueue.pop();

        std::cout << "Received encoded frame" << std::endl;

        myfile.write(reinterpret_cast<const char *>(frame.data.data()), frame.data.size());
    }

    return 0;
}
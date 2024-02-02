import Camera;
import VideoFrame;
import Encoder;

int main() {
    VideoFrame::VideoFramesQueue queue{};

    Camera::run(queue);

    Encoder::run(queue);

    return 0;
}
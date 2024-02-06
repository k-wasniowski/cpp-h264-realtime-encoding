#include <Modules/Camera.hpp>

#include <opencv2/opencv.hpp>

class CameraInfoBuilder
{
public:
    CameraInfoBuilder() = default;

    Modules::Camera::Info Build(int width, int height)
    {
        return Modules::Camera::Info{width, height};
    }

    ~CameraInfoBuilder() = default;

private:

};

namespace Modules
{
    Camera::Camera() = default;

    Camera::~Camera() = default;

    std::optional<Camera::Info> Camera::Initiate()
    {
        cv::VideoCapture camera{};

        auto result = camera.open(0, cv::CAP_ANY);
        if (!result) {
            std::cout << "Failed to open camera" << std::endl;
            return std::nullopt;
        }

        auto width = camera.get(cv::CAP_PROP_FRAME_WIDTH);
        auto height = camera.get(cv::CAP_PROP_FRAME_HEIGHT);

        Info info{};
        info.width = static_cast<int>(width);
        info.height = static_cast<int>(height);

        return Info{static_cast<int>(width), static_cast<int>(height)};
    }

    void Camera::CaptureLoop()
    {
        while(true)
        {

        }
    }
}

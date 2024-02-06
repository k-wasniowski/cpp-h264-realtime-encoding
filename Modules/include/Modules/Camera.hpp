#ifndef CPP_H264_REALTIME_ENCODING_CAMERA_HPP
#define CPP_H264_REALTIME_ENCODING_CAMERA_HPP

#include <optional>

namespace Modules
{
    class Camera
    {
    public:
        struct Info {
            int width;
            int height;
        };

        Camera();
        ~Camera();

        std::optional<Info> Initiate();

    private:
        void CaptureLoop();
    };
} // namespace Modules

#endif // CPP_H264_REALTIME_ENCODING_CAMERA_HPP

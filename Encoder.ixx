module;

#include <iostream>
#include <thread>
#include <vector>
#include <bit>

#include <wels/codec_api.h>
#include <wels/codec_app_def.h>

export module Encoder;

import VideoFrame;
import Camera;

void run_encoder(Camera::Info cameraInfo, VideoFrame::VideoFramesQueue &queue,
                 VideoFrame::EncodedVideoFramesQueue &encodedVideoFramesQueue) {
    ISVCEncoder *encoder = nullptr;
    WelsCreateSVCEncoder(&encoder);

    if (encoder == nullptr) {
        std::cout << "Failed to create encoder" << std::endl;
        return;
    }

    std::cout << "Encoder created" << std::endl;

    SEncParamExt encoderParams{};
    encoder->GetDefaultParams(&encoderParams);

    encoderParams.iUsageType = CAMERA_VIDEO_REAL_TIME;
    encoderParams.fMaxFrameRate = 30;
    encoderParams.iPicWidth = cameraInfo.width;
    encoderParams.iPicHeight = cameraInfo.height;
    encoderParams.iTargetBitrate = 500000;
    encoderParams.iMaxBitrate = UNSPECIFIED_BIT_RATE;
    encoderParams.iRCMode = RC_QUALITY_MODE;
    encoderParams.bEnableFrameSkip = true;
    encoderParams.uiIntraPeriod = 30;
    encoderParams.eSpsPpsIdStrategy = SPS_LISTING;
    encoderParams.uiMaxNalSize = 0;

    auto res = encoder->InitializeExt(&encoderParams);
    if (res != 0) {
        std::cout << "Failed to initialize encoder" << std::endl;
        return;
    }

    while (true) {
        auto frame = queue.pop();

        //auto yuv = Convert(frame.data, cameraInfo.width, cameraInfo.height);

        SSourcePicture sSourcePicture{};
        sSourcePicture.iPicWidth = cameraInfo.width;
        sSourcePicture.iPicHeight = cameraInfo.height;
        sSourcePicture.iColorFormat = EVideoFormatType::videoFormatI420;
        sSourcePicture.iStride[0] = frame.strideY;
        sSourcePicture.iStride[1] = frame.strideU;
        sSourcePicture.iStride[2] = frame.strideV;

        sSourcePicture.pData[0] = frame.dataY.data();
        sSourcePicture.pData[1] = frame.dataU.data();
        sSourcePicture.pData[2] = frame.dataV.data();

        SFrameBSInfo info;
        memset(&info, 0, sizeof(SFrameBSInfo));

        auto encodeResult = encoder->EncodeFrame(&sSourcePicture, &info);
        if (encodeResult != cmResultSuccess) {
            std::cout << "Failed to encode frame" << std::endl;
            continue;
        }

        if (info.eFrameType == videoFrameTypeSkip) {
            std::cout << "Frame skipped" << std::endl;
            continue;
        }

        VideoFrame::EncodedFrame encodedFrame{};
        encodedFrame.data = std::vector<uint8_t>(info.iFrameSizeInBytes);
        memcpy(encodedFrame.data.data(), info.sLayerInfo[0].pBsBuf, info.iFrameSizeInBytes);

        encodedVideoFramesQueue.push(encodedFrame);
    }
}

export namespace Encoder {
    void run(Camera::Info cameraInfo, VideoFrame::VideoFramesQueue &queue,
             VideoFrame::EncodedVideoFramesQueue &encodedVideoFramesQueue) {
        auto cameraThread = std::thread{run_encoder, cameraInfo, std::ref(queue), std::ref(encodedVideoFramesQueue)};
        cameraThread.detach();
    }
}
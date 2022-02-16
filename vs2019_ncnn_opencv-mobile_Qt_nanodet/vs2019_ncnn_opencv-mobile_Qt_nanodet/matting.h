#pragma once

#include"net.h"
#include<opencv2/opencv.hpp>

class Matting
{
public:
	Matting();
	~Matting();

	cv::Mat once(cv::Mat rgb);

private:
	ncnn::Net net;

	const float meanVals[3] = { 127.5f, 127.5f,  127.5f };
	const float normVals[3] = { 0.0078431f, 0.0078431f, 0.0078431f };
};
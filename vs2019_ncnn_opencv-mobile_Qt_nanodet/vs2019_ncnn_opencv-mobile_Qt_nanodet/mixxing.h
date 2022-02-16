#pragma once

#include"net.h"
#include<opencv2/opencv.hpp>

class Mixxing
{
public:
	Mixxing();
	~Mixxing();

	void calT(cv::Mat inp_img, cv::Mat ref_img);
	cv::Mat once(cv::Mat rgb);

private:
	ncnn::Net net;

	cv::Mat T;

	const float mean_vals[3] = { 127.5f, 127.5f, 127.5f };
	const float norm_vals[3] = { 1.f / 127.5f, 1.f / 127.5f, 1.f / 127.5f };

	const float post_mean_vals[3] = { -1.f, -1.f, -1.f };
	const float post_norm_vals[3] = { 0.5f, 0.5f, 0.5f };
};
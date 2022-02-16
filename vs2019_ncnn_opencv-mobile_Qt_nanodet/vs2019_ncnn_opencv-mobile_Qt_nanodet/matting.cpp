#include "matting.h"

Matting::Matting()
{
	net.clear();
	net.load_param("assets/matting.param");
	net.load_model("assets/matting.bin");
}

Matting::~Matting()
{
	net.clear();
}

cv::Mat Matting::once(cv::Mat rgb)
{
	ncnn::Mat in_resize = ncnn::Mat::from_pixels_resize(rgb.data, ncnn::Mat::PIXEL_RGB, rgb.cols, rgb.rows, 512, 512);
	in_resize.substract_mean_normalize(meanVals, normVals);
	ncnn::Mat out;
	{
		ncnn::Extractor ex = net.create_extractor();
		ex.input("input", in_resize);
		ex.extract("output", out);
	}
	cv::Mat mask(cv::Size(out.w,out.h), CV_32FC1, out.data);
	cv::resize(mask, mask, rgb.size());

	return mask;
}
#include"mixxing.h"

Mixxing::Mixxing()
{
	net.clear();
	net.load_param("assets/mixxing.param");
	net.load_model("assets/mixxing.bin");
}

Mixxing::~Mixxing()
{
	net.clear();
}

void Mixxing::calT(cv::Mat inp_img, cv::Mat ref_img)
{
	ncnn::Mat inp_img_mat = ncnn::Mat::from_pixels_resize(inp_img.data, ncnn::Mat::PIXEL_BGR2RGB, inp_img.cols, inp_img.rows, 256, 256);
	ncnn::Mat ref_img_mat = ncnn::Mat::from_pixels_resize(ref_img.data, ncnn::Mat::PIXEL_BGR2RGB, ref_img.cols, ref_img.rows, 256, 256);

	inp_img_mat.substract_mean_normalize(mean_vals, norm_vals);
	ref_img_mat.substract_mean_normalize(mean_vals, norm_vals);

	ncnn::Mat output;
	{
		ncnn::Extractor ex = net.create_extractor();
		ex.input("in0", inp_img_mat);
		ex.input("in1", ref_img_mat);
		ex.extract("out0", output);
	}

	output.substract_mean_normalize(post_mean_vals, post_norm_vals);
	inp_img_mat.substract_mean_normalize(post_mean_vals, post_norm_vals);

	output = output.reshape(256 * 256, 3, 1);
	inp_img_mat = inp_img_mat.reshape(256 * 256, 3, 1);

	cv::Mat I2(cv::Size(output.w, output.h), CV_32FC1, output.channel(0));
	cv::Mat I1(cv::Size(inp_img_mat.w, inp_img_mat.h), CV_32FC1, inp_img_mat.channel(0));

	T = (I1 * I1.t()).inv() * I1 * I2.t();
}

cv::Mat Mixxing::once(cv::Mat rgb)
{
	cv::Mat rgb_tmp = rgb.clone();
	for (int row = 0; row < rgb.rows; row++)
	{
		for (int col = 0; col < rgb.cols; col++)
		{
			cv::Vec3b& src = rgb_tmp.at<cv::Vec3b>(row, col);
			cv::Vec3b& dst = rgb.at<cv::Vec3b>(row, col);
			dst[0] = src[0] * T.at<float>(0, 0) + src[1] * T.at<float>(1, 0) + src[2] * T.at<float>(2, 0);
			dst[1] = src[0] * T.at<float>(0, 1) + src[1] * T.at<float>(1, 1) + src[2] * T.at<float>(2, 1);
			dst[2] = src[0] * T.at<float>(0, 2) + src[1] * T.at<float>(1, 2) + src[2] * T.at<float>(2, 2);
		}
	}
	return rgb;
}
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void Plot_img(Mat& img, vector<Point>& point, float& k, float& b);
vector<Point> mad(vector<Point>& point, float s, float& k, float& b);



int main()
{
	float k = 0.7, b = 5;
	float k_find = 0, b_find = 0;

	RNG rng;
	vector<Point> point;
	Mat img = Mat(1000, 1000, CV_8UC1);
	Mat img2 = Mat(1000, 1000, CV_8UC1);
	img = Scalar(0);
	img2 = Scalar(0);
	for (int i = 0; i < 15; i++)
	{
		float x = rng.uniform(0, 1000);
		float y = k*x +b+ rng.gaussian(10);
		if (i == 7) y += 200;
		if (i == 8) y += 500;
		point.push_back(Point(x, y));
		//显示
		circle(img, point[i], 5, (0, 0, 255), 0);
	}

	Plot_img(img, point, k_find, b_find);
	line(img, Point2i(0, b_find), Point2i(img.cols, int(k_find * img.cols + b_find)), Scalar(255, 0, 0), 2, LINE_AA);

	vector<Point> point_selsct = mad(point, 2, k_find, b_find);
	Plot_img(img, point_selsct, k_find, b_find);
	line(img2, Point2i(0, b_find), Point2i(img.cols, int(k_find * img.cols + b_find)), Scalar(255, 0, 0), 2, LINE_AA);
	for (int i = 0; i < point_selsct.size(); i++)
	{
		circle(img2, point_selsct[i], 5, (0, 0, 255), 0);
	}
	imshow("img", img);
	imshow("img_select", img2);
	imwrite("img_org.jpg", img);
	imwrite("img_new.jpg", img2);


	waitKey(0);
	return 0;
}

void Plot_img(Mat &img, vector<Point> &point, float& k, float& b)
{
	vector<float> x;
	vector<float> y;

	for (int i = 0; i < point.size(); i++)
	{
		x.push_back(point[i].x);
		y.push_back(point[i].y);
	}
	float t1 = 0, t2 = 0, t3 = 0, t4 = 0;
	for (unsigned int i = 0; i < x.size(); i++)
	{
		t1 += x[i] * x[i];
		t2 += x[i];
		t3 += x[i] * y[i];
		t4 += y[i];
	}
	k = (t3 * x.size() - t2 * t4) / (t1 * x.size() - t2 * t2);
	b = (t1 * t4 - t2 * t3) / (t1 * x.size() - t2 * t2);
	cout << k << endl;
	cout << b << endl;
}




vector<Point> mad(vector<Point> &point, float s, float& k, float& b)
{
	int n = point.size();
	if (n < 2)
	{
		return point;
	}
	vector<float> distance;
	vector<float> distance2;
	float x1 = 0, y1 = b, x2 = 100, y2 = 100 * k + b;
	for (int i = 0; i < n; i++)
	{
		distance.push_back(fabs(((y1 - y2) * point[i].x - (x1 - x2) * point[i].y + (x1 * y2 - x2 * y1)) / sqrt(pow(y1 - y2, 2) + pow(x1 - x2, 2))));
	}
	distance2 = distance;
	sort(distance.begin(), distance.end());

	float median = (n % 2 == 1) ? distance[n / 2] :
		(distance[n / 2] + distance[n / 2 - 1]) / 2;//中位数
	vector<float> deviations;
	vector<Point> new_nums;     //偏差值
	for (int i = 0; i < n; i++)
	{
		deviations.push_back(abs(distance[i] - median));
	}

	sort(deviations.begin(), deviations.end());
	float mad = (n % 2 == 1) ? deviations[n / 2] :
		(deviations[n / 2] + deviations[n / 2 - 1]) / 2;
	for (size_t i = 0; i < n; i++)
	{
		if (abs(distance2[i] - median) < s * mad)
			new_nums.push_back(point[i]);
	}
	return new_nums;
}

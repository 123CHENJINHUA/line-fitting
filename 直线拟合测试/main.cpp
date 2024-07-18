#include <iostream>
#include <opencv2/opencv.hpp>
#include<cmath>
#include<time.h>
#include <numeric>
#include <time.h>


using namespace std;
using namespace cv;



void testLineFitting(vector<int> &result,float r_step, float theta_step, int pnts_dist, int pnts_num, int outlier_num, int noise_range, int outlier_noise_range[]);
vector<Point> mad(vector<Point>& point, float s, float& k, float& b);


int main()
{
	int outlier_noise_range[2] = { 20,50 };
	vector<int> result;
	clock_t start, end;

	start = clock();
	testLineFitting(result,1, 0.1, 20, 20, 4, 5, outlier_noise_range);
	end = clock();
	cout << "time = " << int(double(end - start) / CLOCKS_PER_SEC) /60<< "min" << int(double(end - start) / CLOCKS_PER_SEC) % 60 << "s" << endl;

	cout << "0-1 : " << result[0] << endl;
	cout << "1-2 : " << result[1] << endl;
	cout << "2-3 : " << result[2] << endl;
	cout << "3-4 : " << result[3] << endl;
	cout << "4-5 : " << result[4] << endl;
	cout << "5-6 : " << result[5] << endl;
	cout << "6-7 : " << result[6] << endl;
	cout << "7-8 : " << result[7] << endl;
	cout << "8-9 : " << result[8] << endl;
	cout << "9-10 : " << result[9] << endl;
	cout << ">10 : " << result[10] << endl;

	cout << "测试数据总计：" << accumulate(result.begin(), result.end(), 0) << endl;
	return 0;
}



void testLineFitting(vector<int>& result,float r_step, float theta_step, int pnts_dist, int pnts_num, int outlier_num, int noise_range, int outlier_noise_range[])
{
	int row = 10000, col = 3600;
	float r = 0, theta = 0;
	int high = 10;
	vector<vector<vector<Point>>> vec(row, vector<vector<Point>>(col, vector<Point>(pnts_num)));

	float theta_rad=0, start_point=0, x_step=0, k_r=0, b_r=0, k_bar=0, b_bar=0;
	float theta_error = 0;
	float triangle = 0;

	int sum_0t1 = 0, sum_1t2 = 0, sum_2t3 = 0, sum_3t4 = 0, sum_4t5 = 0, sum_5t6 = 0, sum_6t7 = 0, sum_7t8 = 0, sum_8t9 = 0, sum_9t10 = 0, sum_10tm = 0;

	srand(time(0));

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			theta_rad = theta * 3.14159 / 180;
			start_point = r * cos(theta_rad) - pnts_num / 2 * pnts_dist * sin(theta_rad);
			x_step = pnts_dist * sin(theta_rad);
			k_r = -cos(theta_rad) / sin(theta_rad);
			b_r = r / sin(theta_rad);


			for (int k = 0; k < pnts_num; k++)
			{
				float x = 0;
				float y = 0;
				int noise = -noise_range + rand() % (2 * noise_range + 1);
				if (theta == 0 )
				{
					x = r+noise;
					y = -pnts_num / 2 * pnts_dist + k * pnts_dist ;
				}
				else if (theta == 180)
				{
					x = -r+noise;
					y = -pnts_num / 2 * pnts_dist + k * pnts_dist ;
				}
				else if (theta == 90)
				{
					x = -pnts_num / 2 * pnts_dist + k * pnts_dist;
					y = r + noise;
				}
				else if (theta == 270)
				{
					x = -pnts_num / 2 * pnts_dist + k * pnts_dist ;
					y = -r + noise;
				}
				else
				{
					x = start_point + k * x_step + noise * cos(theta_rad);
					y = k_r * x + b_r + noise * sin(theta_rad);
				}
				vec[i][j][k].x = x;
				vec[i][j][k].y = y;
			}
			
			random_shuffle(vec[i][j].begin(), vec[i][j].end());
			for (int z = 0; z < outlier_num; z++)
			{
				int outlier_noise = outlier_noise_range[0] + rand() % (outlier_noise_range[1] - outlier_noise_range[0] + 1);
				vec[i][j][z].x += outlier_noise * cos(theta_rad);
				vec[i][j][z].y += outlier_noise * sin(theta_rad);
				
			}

			Vec4f line_para;

			/*
			//剔除离群点
			fitLine(vec[i][j], line_para, cv::DIST_L2, 0, 1e-2, 1e-2);
			Point point0;
			vector<Point> vec2;
			point0.x = line_para[2];
			point0.y = line_para[3];
			k_bar = line_para[1] / line_para[0];
			b_bar = k_bar * (0 - point0.x) + point0.y ;
			vec2 = mad(vec[i][j], 2, k_bar, b_bar);
			fitLine(vec2, line_para, cv::DIST_L2, 0, 1e-2, 1e-2);
			k_bar = line_para[1] / line_para[0];
			*/
			
			
			//不剔除离群点
			fitLine(vec[i][j], line_para, cv::DIST_L2, 0, 1e-2, 1e-2);
			k_bar = line_para[1] / line_para[0];
			

			if (theta == 0 || theta == 180)
			{
				theta_error = 3.14159/2 - atan(k_bar);
			}
			else
			{
				theta_error = atan(fabs(k_r - k_bar) / fabs(1 + k_r * k_bar));
			}

			triangle = 2 * high * high * tan(theta_error / 2);
			if (triangle >= 0 && triangle < 1) sum_0t1++;
			else if (triangle >= 1 && triangle < 2) sum_1t2++;
			else if (triangle >= 2 && triangle < 3) sum_2t3++;
			else if (triangle >= 3 && triangle < 4) sum_3t4++;
			else if (triangle >= 4 && triangle < 5) sum_4t5++;
			else if (triangle >= 5 && triangle < 6) sum_5t6++;
			else if (triangle >= 6 && triangle < 7) sum_6t7++;
			else if (triangle >= 7 && triangle < 8) sum_7t8++;
			else if (triangle >= 8 && triangle < 9) sum_8t9++;
			else if (triangle >= 9 && triangle < 10) sum_9t10++;
			else sum_10tm++;
			
			
			theta += theta_step;
		}
		r += r_step;
		theta = 0;
	}
	result.push_back(sum_0t1);
	result.push_back(sum_1t2);
	result.push_back(sum_2t3);
	result.push_back(sum_3t4);
	result.push_back(sum_4t5);
	result.push_back(sum_5t6);
	result.push_back(sum_6t7);
	result.push_back(sum_7t8);
	result.push_back(sum_8t9);
	result.push_back(sum_9t10);
	result.push_back(sum_10tm);


	/*
	for (int k = 0; k < pnts_num; k++)
	{
		cout << vec[0][450][k].x<< endl;
	}
	cout << "---------" << endl;
	for (int k = 0; k < pnts_num; k++)
	{
		cout << vec[0][450][k].y << endl;
	}
	*/
}



vector<Point> mad(vector<Point>& point, float s, float& k, float& b)
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
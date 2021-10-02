#include <iostream>
#include <Eigen/Dense>
#include <vector>
#include <random>
#include <cmath>
struct line
{
	int16_t start_x;
	int16_t start_y;
	int16_t end_x;
	int16_t end_y;
	float length;
	line(int16_t startx,int16_t starty,int16_t endx,int16_t endy){
		start_x = startx;
		start_y = starty;
		end_x = endx;
		end_y = endy;
		length = sqrt((startx - endx) *  (startx - endx)+ (starty - endy) * (starty - endy));
	}
};
void detecting(){

}
void matching(){
	std::vector<line> local;
	std::vector<line> global;
	// basic parameter from the papers
	int16_t local_point_num = 20;
	int16_t local_line_num = 30;
	int16_t global_line_num = 6105;
	int16_t localmap_size = 512;
	int16_t globalmap_width = 3500;
	int16_t globalmap_height = 7300;
	int16_t E = 2;
	float smin = 0.85;
	float smax = 1.15;
	float thetamax = 15;
	// for the worst case, each combination between two local points can be a line segment
	std::random_device r;
	std::default_random_engine el(r());
	std::uniform_real_distribution<float> distrib(0,1);
	std::uniform_int_distribution<int> distrib_local(0, localmap_size-1);
	for (int16_t i = 0; i < local_line_num; i++)
	{
		local.push_back(line(distrib_local(el), distrib_local(el), distrib_local(el), distrib_local(el)));
		
	}
	std::uniform_int_distribution<int> distrib_x(0, globalmap_width-1);
	std::uniform_int_distribution<int> distrib_y(0, globalmap_height-1);
	// generate line segments in global map 
	for (int16_t i = 0; i < global_line_num; i++)
	{
		global.push_back(line(distrib_x(el), distrib_y(el), distrib_x(el), distrib_y(el)));
	}
	// finding corresponding line segments for each line segment in the local map
	// for the worst case, all line segments in the global map will correspond with each line segment in the local map
	// also, we confirm the transformation matrix
	for (int16_t i = 0; i < local_line_num; i++)
	{
		int16_t t = 0;
		Eigen::MatrixXd local_vector(2,1);
		for (int16_t j = 0; j < global_line_num; j++){
			// we assume every line segment in the global map will satisfy the restriction
			// if(global[j].length > smin * local[i].length && global[j].length < smax * local[i].length && angle < thetamax)
			Eigen::MatrixXd affine(2,2);
			// as I do not have real data, I just randomize the matrix
			// computing a affine transform only use a contant time, so the time cost is very similar
			affine(0,0) = distrib(el);
			affine(0,1) = distrib(el);
			affine(1,0) = distrib(el);
			affine(1,1) = distrib(el);
			for (int16_t k = 0; k < local_point_num; k++)
			{
				if (i == k)
					continue;
				Eigen::MatrixXd projection(2,1);
				Eigen::MatrixXd pos(2,1);
				pos(0,0) = local[k].end_x - local[k].start_x;
				pos(1,0) = local[k].end_y - local[k].start_y;
				projection = affine * pos;
				// if around E, there is a crater in the global map, it counts a valid transformation
				// if t > R, it will be a true tranformation
				for(int16_t e1 = 0; e1 < E; e1++)
					for(int16_t e2 = 0; e2 < E; e2++)
					{
						//if there is a crater
						//t += 1;
						//
					}

			}
		}
	}

}
int main(){
	matching();
}

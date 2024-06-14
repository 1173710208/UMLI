#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


#define MAX_DIS 99999999999999	/*1.7976931348623157E+308*/
#define UMLI_TEMPLATE template<class DATATYPE, class ELEMTYPE, int NUMDIMS>		// Define template of UMLIS
#define UMLI_QUAL UMLI<DATATYPE, ELEMTYPE, NUMDIMS>								// Define template use of UMLI




/**
* A class to represent UMLI, an updatable multi-dimensional learned index
* Example usage: For a 2-dimensional UMLI use 'UMLI<int, double, 2> umli;'
*
* DATATYPE Referenced data, such as int, void* or obj*
* ELEMTYPE Type of data attribute, such as int, float or double
* NUMDIMS Number of dimensions, such as 2 or 3
*/
UMLI_TEMPLATE
class UMLI
{

protected:

	/**
	* A structure to represent CDF
	*/
	struct APLF {
		vector<double> slope;			// The slope of the linear segment.
		vector<ELEMTYPE> x_intercept;	// The x-intercept of the lienar segment.
		vector<int> y_intercept;   // The y-intercept of the lienar segment.
	};

	/**
	* A structure to represent the data record in grid index
	*/
	struct Point {
		ELEMTYPE p_val[NUMDIMS];
		DATATYPE p_id;
	};

	/**
	* A structure to represent the cell in grid index
	*/
	struct Cell {
		int c_size;
		vector<Point> point_arr;

		Cell() {
			c_size = 0;
		}

		APLF c_aplf; /*APLF of this cell*/
	};





protected:

	int sort_dim;							// Sort dimension
	int error_bound;						// Target error bound
	double relearn_threshold;				// Threshold for triggering relearn

	int cell_num;							// Total number of cells
	int unit[NUMDIMS];						// Array to assist in calculating the cell's index
	int interval_num[NUMDIMS];				// Number of columns for each dimension

	vector<vector<ELEMTYPE>> linear_scales;	// Division points for each dimension
	Cell* grid_array;						// Array to store all cells

	// RMI of each dimension
	//using layer1_type = rmi::LinearSpline;
	//using layer2_type = rmi::LinearRegression;
	//rmi::RmiLAbs<ELEMTYPE, layer1_type, layer2_type> rmi[NUMDIMS];
	//int dim_sort[NUMDIMS];				/*Sort of dimensions*/

public:

	/**
	* Constructor without parameter
	*/
	UMLI();

	/**
	* Constructor with parameter to learn CDF and linear scales
	*
	* @param id_arr All points' id
	* @param data_arr All k-dimensional points represented by a vector
	* @param linear_scales Separation points for each dimension
	* @param error_bound Target error bound
	* @param relearn_thres Threshold for triggering relearn
	*/
	UMLI(vector<DATATYPE> id_arr, vector<vector<ELEMTYPE>> data_arr, vector<vector<ELEMTYPE>> linear_scales, int error_bound, double relearn_threshold);

	/**
	* Destructor
	*/
	~UMLI();

	/**
	* Output the size of each cell in UMLI
	*/
	void PrintGridSize();

	/**
	* Output the APLF of each cell in UMLI
	*/
	void PrintGridAPLF();

	/**
	* Calculate the memory footprint occupied by UMLI
	*
	* @return Number of bytes occupied by kd tree
	*/
	int Footprint();

	/**
	* Insert a new point
	*
	* @param a_point A k-dimensional point represented by an array
	* @param a_id ID of record
	*/
	void Insert(const ELEMTYPE* a_point, DATATYPE& a_id);

	/**
	* Delete a point
	*
	* @param a_point A k-dimensional point represented by an array
	* @return Whether the point is found and deleted
	*/
	void Delete(const ELEMTYPE* a_point);

	/**
	* Search a point
	*
	* @param Result A vector containing results of query
	* @param q_point A k-dimensional point represented by an array
	* @return Whether query point exist
	*/
	bool PointQuery(vector<DATATYPE>& Result, const ELEMTYPE* q_point);

	/**
	* Search all points within a hyper-rectangle range
	*
	* @param Result A vector containing results of query
	* @param q_min Min bounds of query rectangle
	* @param q_max Max bounds of query rectangle
	* @return Number of records met the query
	*/
	int RangeQuery(vector<DATATYPE>& Result, ELEMTYPE* q_min, ELEMTYPE* q_max);

	/**
	* Search k nearest neighbours of a point
	*
	* @param Result A vector containing results of query
	* @param q_point Queried k-dimensional point represented by an array
	* @param k Number of nearest neighbours
	*/
	void KNNQuery(DATATYPE* Result, ELEMTYPE* q_point, int k);

protected:

	int CalPointIndex(vector<ELEMTYPE> a_point);
	int CalPointIndex(const ELEMTYPE* a_point);
	void CalRangeIndex(int* cells_index, bool* is_fringe, int& write_index, const int cell_min_index[NUMDIMS], const int cell_max_index[NUMDIMS], int dim, int tmp_cell_index, bool tmp_fringe_flag);
	void CalKNNIndex(int* cells_index, double* cells_MINDIST, int& write_index, ELEMTYPE* q_point, const int q_cell_index_arr[NUMDIMS], const int cell_min_index[NUMDIMS], const int cell_max_index[NUMDIMS], int dim, int round, int tmp_cell_index, double tmp_mindist, int tmp_chebdis);

	void LearnAPLF(Cell& cell);
	int SearchAPLF(APLF aplf, ELEMTYPE key);

	double CalEuc2Dis(const ELEMTYPE* a_point, const ELEMTYPE* b_point);
	double CalMINDIST(const ELEMTYPE* a_point, const ELEMTYPE* b_min, const ELEMTYPE* b_max);
};


UMLI_TEMPLATE
UMLI_QUAL::UMLI()
{
	cout << "construct without parameter" << endl;
}


UMLI_TEMPLATE
UMLI_QUAL::UMLI(vector<DATATYPE> id_arr, vector<vector<ELEMTYPE>> data_arr, vector<vector<ELEMTYPE>> linear_scales, int error_bound, double relearn_threshold)
{
	this->sort_dim = 0;
	this->error_bound = error_bound;
	this->relearn_threshold = relearn_threshold;

	this->linear_scales = linear_scales;
	int num_cell_total = 1;
	for (int i = 0; i < linear_scales.size(); ++i) {
		this->interval_num[i] = linear_scales[i].size() - 1;
		num_cell_total *= linear_scales[i].size() - 1;
	}
	this->cell_num = num_cell_total;
	grid_array = new Cell[num_cell_total];
	for (int i = 0; i < NUMDIMS; ++i)
	{
		int unit_tmp = 1;
		for (int j = i; j < NUMDIMS; ++j) {
			unit_tmp *= interval_num[j];
		}
		unit_tmp /= interval_num[i];
		this->unit[i] = unit_tmp;
	}

	for (int i = 0; i < data_arr.size(); ++i)
	{
		int cell_index = CalPointIndex(data_arr[i]);

		Point p;
		for (int dim = 0; dim < NUMDIMS; ++dim)
		{
			p.p_val[dim] = data_arr[i][dim];
		}
		p.p_id = id_arr[i];
		grid_array[cell_index].point_arr.push_back(p);
		grid_array[cell_index].c_size++;
	}

	for (int i = 0; i < num_cell_total; ++i)
	{
		LearnAPLF(grid_array[i]);
	}
}


UMLI_TEMPLATE
UMLI_QUAL::~UMLI()
{
	if (grid_array != NULL)
	{
		delete[] grid_array;
		grid_array = NULL;
	}
}


UMLI_TEMPLATE
void UMLI_QUAL::PrintGridSize()
{
	for (int i = 0; i < cell_num; ++i)
	{
		cout << "number of points in cell" << i << ":\t" << grid_array[i].c_size << endl;
	}
}


UMLI_TEMPLATE
void UMLI_QUAL::PrintGridAPLF()
{
	for (int i = 0; i < cell_num; ++i)
	{
		cout << "APLF of points in cell" << i << ":" << endl;
		for (int j = 0; j < grid_array[i].c_aplf.slope.size(); ++j)
		{
			cout << "piecewise" << j << ":\t" << "x:" << grid_array[i].c_aplf.x_intercept[j] << "\t y:" << grid_array[i].c_aplf.y_intercept[j] << "\t slope:" << grid_array[i].c_aplf.slope[j] << endl;
		}
	}
}


UMLI_TEMPLATE
int UMLI_QUAL::Footprint()
{
	int Footprint = 0;
	Footprint += sizeof(*this);
	for (int i = 0; i < linear_scales.size(); i++)
	{
		Footprint += linear_scales[i].capacity() * sizeof(ELEMTYPE);

	}
	for (int i = 0; i < cell_num; i++) {
		int cellSize = sizeof(grid_array[i]) + grid_array[i].point_arr.capacity() * sizeof(Point);
		Footprint += cellSize;
		int aplfSize = grid_array[i].c_aplf.slope.size() * (sizeof(double) + sizeof(ELEMTYPE) + sizeof(int));
		Footprint += aplfSize;
	}

	return Footprint;
}


UMLI_TEMPLATE
void UMLI_QUAL::Insert(const ELEMTYPE* a_point, DATATYPE& a_id)
{
	//int cell_index = 0;
	//for (int dim = 0; dim < NUMDIMS; ++dim)
	//{
	//	for (int j = 0; j < linear_scales[dim].size() - 1; ++j)
	//	{
	//		if ((a_point[dim] >= linear_scales[dim][j]) && (a_point[dim] < linear_scales[dim][j + 1]))
	//		{
	//			cell_index += unit[dim] * j;
	//			break;
	//		}
	//	}
	//}

	//// check delete bucket
	//for (int i = 0; i < grid_array[cell_index].del_bucket.b_size; ++i) {
	//	bool ins_flag = true;
	//	for (int j = 0; j < NUMDIMS; ++j)
	//	{
	//		if (grid_array[cell_index].del_bucket.point_arr[i].p_val[j] != a_point[j])
	//		{
	//			ins_flag = 0;
	//			break;
	//		}
	//	}
	//	if (ins_flag == 1) {
	//		grid_array[cell_index].del_bucket.point_arr.erase(grid_array[cell_index].del_bucket.point_arr.begin() + i);
	//		grid_array[cell_index].del_bucket.b_size--;
	//		return;
	//	}
	//}

	//Point p;
	//for (int dim = 0; dim < NUMDIMS; ++dim)
	//{
	//	p.p_val[dim] = a_point[dim];
	//}
	//p.p_id = a_id;
	//grid_array[cell_index].ins_bucket.point_arr.push_back(p);
	//grid_array[cell_index].ins_bucket.b_size++;

	//// relearn
	//if (grid_array[cell_index].ins_bucket.b_size > relearn_threshold* grid_array[cell_index].c_size) {
	//	for (int i = 0; i < grid_array[cell_index].ins_bucket.b_size; i++)
	//	{
	//		if (grid_array[cell_index].ins_bucket.point_arr[i].p_val[sort_dim] < grid_array[cell_index].point_arr[0].p_val[sort_dim])
	//		{
	//			grid_array[cell_index].point_arr.insert(grid_array[cell_index].point_arr.begin(), grid_array[cell_index].ins_bucket.point_arr[i]);
	//			grid_array[cell_index].c_size++;
	//		}
	//		else if (grid_array[cell_index].ins_bucket.point_arr[i].p_val[sort_dim] > grid_array[cell_index].point_arr.back().p_val[sort_dim])
	//		{
	//			grid_array[cell_index].point_arr.push_back(grid_array[cell_index].ins_bucket.point_arr[i]);
	//		}
	//		else
	//		{
	//			for (int j = 0; j < grid_array[cell_index].c_size; j++)
	//			{
	//				if (grid_array[cell_index].ins_bucket.point_arr[i].p_val[sort_dim] >= grid_array[cell_index].point_arr[j].p_val[sort_dim] && grid_array[cell_index].ins_bucket.point_arr[i].p_val[sort_dim] <= grid_array[cell_index].point_arr[j + 1].p_val[sort_dim])
	//				{
	//					grid_array[cell_index].point_arr.insert(grid_array[cell_index].point_arr.begin() + j + 1, grid_array[cell_index].ins_bucket.point_arr[i]);
	//					grid_array[cell_index].c_size++;
	//				}
	//			}
	//		}
	//	}
	//	LearnAPLF(grid_array[cell_index]);
	//	grid_array[cell_index].ins_bucket.point_arr.clear();
	//	grid_array[cell_index].ins_bucket.b_size = 0;
	//}
}


UMLI_TEMPLATE
void UMLI_QUAL::Delete(const ELEMTYPE* a_point)
{

	//int cell_index = 0;
	//for (int dim = 0; dim < NUMDIMS; ++dim)
	//{
	//	for (int j = 0; j < linear_scales[dim].size() - 1; ++j)
	//	{
	//		if ((a_point[dim] >= linear_scales[dim][j]) && (a_point[dim] < linear_scales[dim][j + 1]))
	//		{
	//			cell_index += unit[dim] * j;
	//			break;
	//		}
	//	}
	//}

	//// check insert bucket
	//for (int i = 0; i < grid_array[cell_index].ins_bucket.b_size; ++i) {
	//	bool ins_flag = true;
	//	for (int j = 0; j < NUMDIMS; ++j)
	//	{
	//		if (grid_array[cell_index].ins_bucket.point_arr[i].p_val[j] != a_point[j])
	//		{
	//			ins_flag = 0;
	//			break;
	//		}
	//	}
	//	if (ins_flag == 1) {
	//		grid_array[cell_index].ins_bucket.point_arr.erase(grid_array[cell_index].ins_bucket.point_arr.begin() + i);
	//		grid_array[cell_index].ins_bucket.b_size--;
	//		return;
	//	}
	//}

	//Point p;
	//for (int dim = 0; dim < NUMDIMS; ++dim)
	//{
	//	p.p_val[dim] = a_point[dim];
	//}
	//p.p_id = NULL;
	//grid_array[cell_index].del_bucket.point_arr.push_back(p);
	//grid_array[cell_index].del_bucket.b_size++;

	//// relearn
	//if (grid_array[cell_index].del_bucket.b_size > relearn_threshold * grid_array[cell_index].c_size) {
	//	for (int i = 0; i < grid_array[cell_index].del_bucket.b_size; i++)
	//	{
	//		for (int j = 0; j < grid_array[cell_index].c_size; j++)
	//		{
	//			bool eqal_flag = true;
	//			for (int k = 0; k < NUMDIMS; k++)
	//			{
	//				if (grid_array[cell_index].del_bucket.point_arr[i].p_val[k] != grid_array[cell_index].point_arr[j].p_val[k])
	//				{
	//					eqal_flag = false;
	//				}
	//			}
	//			if (eqal_flag == true)
	//			{
	//				grid_array[cell_index].point_arr.erase(grid_array[cell_index].point_arr.begin()+j);
	//				grid_array[cell_index].c_size--;
	//			}
	//		}
	//	}
	//	LearnAPLF(grid_array[cell_index]);
	//	grid_array[cell_index].del_bucket.point_arr.clear();
	//	grid_array[cell_index].del_bucket.b_size = 0;
	//}
}


UMLI_TEMPLATE
bool UMLI_QUAL::PointQuery(vector<DATATYPE>& Result, const ELEMTYPE* q_point)
{
	int cell_index = 0;
	for (int dim = 0; dim < NUMDIMS; ++dim)
	{
		for (int j = 0; j < linear_scales[dim].size() - 1; ++j)
		{
			if ((q_point[dim] >= linear_scales[dim][j]) && (q_point[dim] < linear_scales[dim][j + 1]))
			{
				cell_index += unit[dim] * j;
				break;
			}
		}
	}


	// get predicted position
	/*int pre_pos = SearchAPLF(grid_array[cell_index].c_aplf, q_point[sort_dim]);*/
	int pre_pos;
	if (q_point[sort_dim] >= grid_array[cell_index].c_aplf.x_intercept.front() && q_point[sort_dim] < grid_array[cell_index].c_aplf.x_intercept.back()) {
		for (int i = 0; i < grid_array[cell_index].c_aplf.slope.size() - 1; ++i)
		{
			if (q_point[sort_dim] >= grid_array[cell_index].c_aplf.x_intercept[i] && q_point[sort_dim] < grid_array[cell_index].c_aplf.x_intercept[i + 1])
			{
				pre_pos = grid_array[cell_index].c_aplf.y_intercept[i] + grid_array[cell_index].c_aplf.slope[i] * (q_point[sort_dim] - grid_array[cell_index].c_aplf.x_intercept[i]);
			}
		}
	}
	else if (q_point[sort_dim] >= grid_array[cell_index].c_aplf.x_intercept.back())
	{
		pre_pos = grid_array[cell_index].c_aplf.y_intercept.back() + grid_array[cell_index].c_aplf.slope.back() * (q_point[sort_dim] - grid_array[cell_index].c_aplf.x_intercept.back());

	}
	else
	{
		return false;
	}
	if (pre_pos > grid_array[cell_index].c_size)
	{
		pre_pos = grid_array[cell_index].c_size;
	}


	// search for query point
	for (int i = 0; ((pre_pos - i) >= 0) || ((pre_pos + i) < grid_array[cell_index].c_size); i++)
	{
		if ((pre_pos + i) < grid_array[cell_index].c_size) {
			int flag_up = 1;
			for (int j = 0; j < NUMDIMS; ++j)
			{
				if (grid_array[cell_index].point_arr[pre_pos + i].p_val[j] != q_point[j])
				{
					flag_up = 0;
					break;
				}
			}
			if (flag_up == 1) {
				int id = grid_array[cell_index].point_arr[pre_pos + i].p_id;
				Result.push_back(id);
				break;
			}
		}

		if ((pre_pos - i) >= 0)
		{
			int flag_down = 1;
			for (int j = 0; j < NUMDIMS; ++j)
			{
				if (grid_array[cell_index].point_arr[pre_pos - i].p_val[j] != q_point[j])
				{
					flag_down = 0;
					break;
				}
			}
			if (flag_down == 1) {
				int id = grid_array[cell_index].point_arr[pre_pos - i].p_id;
				Result.push_back(id);
				break;
			}
		}
	}

	// return
	return Result.size();

}


UMLI_TEMPLATE
int UMLI_QUAL::RangeQuery(vector<DATATYPE>& Result, ELEMTYPE* q_min, ELEMTYPE* q_max)
{
	//int foundCount = 0;

	//int cell_min_index[NUMDIMS];
	//int cell_max_index[NUMDIMS];

	//// Find index of Cells that intersect with the range query
	//for (int dim = 0; dim < NUMDIMS; dim++) {
	//	for (int i = 0; i < interval_num[dim]; i++) {
	//		if ((q_min[dim] > linear_scales[dim][i]) && (q_min[dim] <= linear_scales[dim][i + 1])) {
	//			cell_min_index[dim] = i;
	//		}
	//		if ((q_max[dim] > linear_scales[dim][i]) && (q_max[dim] <= linear_scales[dim][i + 1])) {
	//			cell_max_index[dim] = i;
	//			break;
	//		}
	//	}
	//}

	//// calculate the number of the cells that will be searched
	//int intersect_num = 1;
	//for (int dim = 0; dim < NUMDIMS; dim++) {
	//	intersect_num *= cell_max_index[dim] - cell_min_index[dim] + 1;
	//}

	//int* cells_index = new int[intersect_num];
	//bool* is_fringe = new bool[intersect_num];
	//int write_index = 0;
	//CalRangeIndex(cells_index, is_fringe, write_index, cell_min_index, cell_max_index, 0, 0, false);

	//for (int i = 0; i < intersect_num; i++)
	//{
	//	int index = cells_index[i];


	//	// Check inner cells
	//	if (is_fringe[i] == false)
	//	{
	//		// check cell
	//		for (int j = 0; j < grid_array[index].c_size; ++j)
	//		{
	//			int id = grid_array[index].point_arr[j].p_id;
	//			Result.push_back(id);
	//			foundCount++;
	//		}
	//	}
	//	// Check fringe cells
	//	else
	//	{
	//		// check cell

	//		int upper_index = grid_array[index].c_size - 1;
	//		if (q_max[sort_dim] < grid_array[index].point_arr.back().p_val[sort_dim])
	//		{
	//			if (q_max[sort_dim] >= grid_array[index].aplf.x_intercept.front() && q_max[sort_dim] < grid_array[index].c_aplf.x_intercept.back()) {
	//				for (int iii = 0; iii < grid_array[index].c_aplf.slope.size() - 1; ++iii)
	//				{
	//					if (q_max[sort_dim] >= grid_array[index].c_aplf.x_intercept[iii] && q_max[sort_dim] < grid_array[index].c_aplf.x_intercept[iii + 1])
	//					{
	//						upper_index = grid_array[index].c_aplf.y_intercept[iii] + grid_array[index].c_aplf.slope[iii] * (q_max[sort_dim] - grid_array[index].c_aplf.x_intercept[iii]);
	//						upper_index += error_bound;
	//					}
	//				}
	//			}
	//			else if (q_max[sort_dim] >= grid_array[index].c_aplf.x_intercept.back())
	//			{
	//				upper_index = grid_array[index].c_aplf.y_intercept.back() + grid_array[index].c_aplf.slope.back() * (q_max[sort_dim] - grid_array[index].c_aplf.x_intercept.back());
	//				upper_index += error_bound;
	//			}
	//			if (upper_index >= grid_array[index].c_size)
	//			{
	//				upper_index = grid_array[index].c_size-1;
	//			}
	//		}
	//		
	//		int lower_index = 0;
	//		if (q_min[sort_dim] > grid_array[index].point_arr[0].p_val[sort_dim])
	//		{
	//			if (q_min[sort_dim] >= grid_array[index].c_aplf.x_intercept.front() && q_min[sort_dim] < grid_array[index].c_aplf.x_intercept.back()) {
	//				for (int iii = 0; iii < grid_array[index].c_aplf.slope.size() - 1; ++iii)
	//				{
	//					if (q_min[sort_dim] >= grid_array[index].c_aplf.x_intercept[iii] && q_min[sort_dim] < grid_array[index].c_aplf.x_intercept[iii + 1])
	//					{
	//						lower_index = grid_array[index].c_aplf.y_intercept[iii] + grid_array[index].c_aplf.slope[iii] * (q_min[sort_dim] - grid_array[index].c_aplf.x_intercept[iii]);
	//						lower_index -= error_bound;
	//					}
	//				}
	//			}
	//			else if (q_min[sort_dim] >= grid_array[index].c_aplf.x_intercept.back())
	//			{
	//				lower_index = grid_array[index].c_aplf.y_intercept.back() + grid_array[index].c_aplf.slope.back() * (q_min[sort_dim] - grid_array[index].c_aplf.x_intercept.back());
	//				lower_index -= error_bound;
	//			}
	//			if (lower_index < 0)
	//			{
	//				lower_index = 0;
	//			}
	//		}

	//		
	//		for (int j = lower_index; j <= upper_index; ++j) {
	//			bool flag = true;
	//			for (int dim = 0; dim < NUMDIMS; ++dim)
	//			{
	//				if (grid_array[index].point_arr[j].p_val[dim] < q_min[dim] || grid_array[index].point_arr[j].p_val[dim] > q_max[dim])
	//				{
	//					flag = false;
	//					break;
	//				}
	//			}
	//			if (flag == true) {
	//				int id = grid_array[index].point_arr[j].p_id;
	//				Result.push_back(id);
	//				foundCount++;
	//			}
	//		}

	//	}
	//

	//}

	//delete[] cells_index;
	//delete[] is_fringe;

	//return foundCount;
	return 0;
}


UMLI_TEMPLATE
void UMLI_QUAL::KNNQuery(DATATYPE* Result, ELEMTYPE* q_point, int k)
{
	//int q_cell_index = 0;
	//int q_cell_index_arr[NUMDIMS];

	//double* NearestDis = new double[k];
	//for (int i = 0; i < k; i++) {
	//	NearestDis[i] = MAX_DIS;
	//}

	//// Find index of the Cell where query point locates
	//for (int dim = 0; dim < NUMDIMS; ++dim)
	//{
	//	for (int j = 0; j < linear_scales[dim].size() - 1; ++j)
	//	{
	//		if ((q_point[dim] >= linear_scales[dim][j]) && (q_point[dim] < linear_scales[dim][j + 1]))
	//		{
	//			q_cell_index_arr[dim] = j;
	//			q_cell_index += unit[dim] * j;
	//			break;
	//		}
	//	}
	//}

	//// Search in cells
	//int round = 0;
	//double min = MAX_DIS;
	//do
	//{
	//	int cell_min_index[NUMDIMS];
	//	int cell_max_index[NUMDIMS];
	//	for (int dim = 0; dim < NUMDIMS; dim++) {
	//		cell_min_index[dim] = q_cell_index_arr[dim] - round >= 0 ? (q_cell_index_arr[dim] - round) : 0;
	//		cell_max_index[dim] = q_cell_index_arr[dim] + round <= (linear_scales[dim].size() - 2) ? (q_cell_index_arr[dim] + round) : (linear_scales[dim].size() - 2);
	//	}

	//	// calculate the number of the cells that will be searched
	//	int search_num = 1;
	//	if (round == 0) {
	//		search_num = 1;
	//	}
	//	else {
	//		int last_cell_min_index[NUMDIMS];
	//		int last_cell_max_index[NUMDIMS];
	//		for (int dim = 0; dim < NUMDIMS; dim++) {
	//			last_cell_min_index[dim] = q_cell_index_arr[dim] - (round - 1) >= 0 ? (q_cell_index_arr[dim] - (round - 1)) : 0;
	//			last_cell_max_index[dim] = q_cell_index_arr[dim] + (round - 1) <= (linear_scales[dim].size() - 2) ? (q_cell_index_arr[dim] + (round - 1)) : (linear_scales[dim].size() - 2);
	//		}
	//		for (int dim = 0; dim < NUMDIMS; dim++) {
	//			search_num *= cell_max_index[dim] - cell_min_index[dim] + 1;
	//		}

	//		int last_search_num = 1;
	//		for (int dim = 0; dim < NUMDIMS; dim++) {
	//			last_search_num *= last_cell_max_index[dim] - last_cell_min_index[dim] + 1;
	//		}
	//		search_num = search_num - last_search_num;
	//	}

	//	// calculate the index of the cells that will be searched
	//	int* cells_index = new int[search_num];
	//	double* cells_MINDIST = new double[search_num];
	//	int write_index = 0;
	//	CalKNNIndex(cells_index, cells_MINDIST, write_index, q_point, q_cell_index_arr, cell_min_index, cell_max_index, 0, round, 0, 0, 0);

	//	// Search in the Cell where query point locates
	//	for (int i = 0; i < search_num; i++) {
	//		int index = cells_index[i];
	//		if (cells_MINDIST[i] < NearestDis[k - 1]) {
	//			// check cell
	//			for (int j = 0; j < grid_array[index].c_size; ++j)
	//			{

	//				double distance = CalEuc2Dis(q_point, grid_array[index].point_arr[j].p_val);
	//				if (distance < NearestDis[k - 1]) {
	//					bool flag = false;
	//					for (int del_i = 0; del_i < grid_array[index].del_bucket.b_size; del_i++)
	//					{
	//						bool del_flag = true;
	//						for (int del_j = 0; del_j < NUMDIMS; ++del_j)
	//						{
	//							if (grid_array[index].del_bucket.point_arr[del_i].p_val[del_j] != q_point[del_j])
	//							{
	//								del_flag = 0;
	//								break;
	//							}
	//						}
	//						if (del_flag == 1) {
	//							flag = true;
	//						}
	//					}
	//					if (flag == false)
	//					{
	//						NearestDis[k - 1] = distance;
	//						Result[k - 1] = grid_array[index].point_arr[j].p_id;
	//						for (int x = 0; x < k - 1; x++) {
	//							for (int y = 0; y < k - 1 - x; y++) {
	//								if (NearestDis[y] > NearestDis[y + 1]) {
	//									double tmp = NearestDis[y];
	//									NearestDis[y] = NearestDis[y + 1];
	//									NearestDis[y + 1] = tmp;
	//									int tmpo = Result[y];
	//									Result[y] = Result[y + 1];
	//									Result[y + 1] = tmpo;
	//								}
	//							}
	//						}
	//					}
	//				}
	//			}
	//			// check ins_bucket
	//			for (int j = 0; j < grid_array[index].ins_bucket.b_size; ++j)
	//			{
	//				double distance = CalEuc2Dis(q_point, grid_array[index].ins_bucket.point_arr[j].p_val);
	//				if (distance < NearestDis[k - 1]) {
	//					NearestDis[k - 1] = distance;
	//					Result[k - 1] = grid_array[index].ins_bucket.point_arr[j].p_id;
	//					for (int x = 0; x < k - 1; x++) {
	//						for (int y = 0; y < k - 1 - x; y++) {
	//							if (NearestDis[y] > NearestDis[y + 1]) {
	//								double tmp = NearestDis[y];
	//								NearestDis[y] = NearestDis[y + 1];
	//								NearestDis[y + 1] = tmp;
	//								int tmpo = Result[y];
	//								Result[y] = Result[y + 1];
	//								Result[y + 1] = tmpo;
	//							}
	//						}
	//					}
	//				}
	//			}

	//		}
	//	}

	//	// calculate min distance from query point to Cell border
	//	min = MAX_DIS;
	//	for (int dim = 0; dim < NUMDIMS; dim++)
	//	{
	//		int border_index_lower = q_cell_index_arr[dim] - round;
	//		int border_index_upper = q_cell_index_arr[dim] + round + 1;
	//		if (border_index_lower > 0)
	//		{
	//			double dis2min = (q_point[dim] - linear_scales[dim][border_index_lower]) * (q_point[dim] - linear_scales[dim][border_index_lower]);
	//			if (dis2min < min)
	//			{
	//				min = dis2min;
	//			}
	//		}
	//		if (border_index_upper < linear_scales[dim].size() - 1)
	//		{
	//			double dis2max = (q_point[dim] - linear_scales[dim][border_index_upper]) * (q_point[dim] - linear_scales[dim][border_index_upper]);
	//			if (dis2max < min)
	//			{
	//				min = dis2max;
	//			}
	//		}
	//	}

	//	delete[] cells_index;
	//	delete[] cells_MINDIST;

	//	round++;


	//} while (min < NearestDis[k - 1]);

}





/**
* Given a point, find the index of the Cell where this point locates
*
* @param a_point A point represented by an array
* @return Index of the Cell where this point locates
*/
UMLI_TEMPLATE
int UMLI_QUAL::CalPointIndex(vector<ELEMTYPE> a_point)
{
	int cell_index = 0;
	for (int dim = 0; dim < NUMDIMS; ++dim)
	{
		for (int j = 0; j < linear_scales[dim].size() - 1; ++j)
		{
			if ((a_point[dim] >= linear_scales[dim][j]) && (a_point[dim] < linear_scales[dim][j + 1]))
			{
				cell_index += unit[dim] * j;
				break;
			}
		}
	}

	return cell_index;
}


/**
* Given a point, find the index of the Cell where this point locates
*
* @param a_point A point represented by an array
* @return Index of the Cell where this point locates
*/
UMLI_TEMPLATE
int UMLI_QUAL::CalPointIndex(const ELEMTYPE* a_point)
{
	int cell_index = 0;
	for (int dim = 0; dim < NUMDIMS; ++dim)
	{
		for (int j = 0; j < linear_scales[dim].size() - 1; ++j)
		{
			if ((a_point[dim] >= linear_scales[dim][j]) && (a_point[dim] < linear_scales[dim][j + 1]))
			{
				cell_index += unit[dim] * j;
				break;
			}
		}
	}

	return cell_index;
}


/**
* Knowing the index range of range query for each dimension, recursively find the index of all intersecting Cells
*
* @param cells_index Index of all intersecting Cells
* @param is_fringe An array indicating whether the Cell is on an edge or not
* @param write_index Write position of cells_index
* @param cell_min_index Start of index range
* @param cell_max_index End of index range
* @param dim Current dimension
* @param tmp_cell_index Current calculated index
* @param tmp_fringe_flag Whether the current Cell is on the edge
*/
UMLI_TEMPLATE
void UMLI_QUAL::CalRangeIndex(int* cells_index, bool* is_fringe, int& write_index, const int cell_min_index[NUMDIMS], const int cell_max_index[NUMDIMS], int dim, int tmp_cell_index, bool tmp_fringe_flag)
{
	for (int i = cell_min_index[dim]; i <= cell_max_index[dim]; ++i)
	{
		bool new_tmp_fringe_flag = tmp_fringe_flag;
		if (new_tmp_fringe_flag == false)
		{
			if (i == cell_min_index[dim] || i == cell_max_index[dim])
			{
				new_tmp_fringe_flag = true;
			}
		}

		int new_tmp_cell_index = tmp_cell_index + i * unit[dim];
		if (dim + 1 == NUMDIMS)
		{
			cells_index[write_index] = new_tmp_cell_index;
			is_fringe[write_index] = new_tmp_fringe_flag;
			write_index++;
		}
		else
		{
			CalRangeIndex(cells_index, is_fringe, write_index, cell_min_index, cell_max_index, dim + 1, new_tmp_cell_index, new_tmp_fringe_flag);
		}
	}
}


/**
* Knowing the index range of range query for each dimension, recursively find the index of all intersecting Cells
*
* @param cells_index Index of all Cells that will be searched
* @param cells_MINDIST MINDIST between Cells and the query point
* @param write_index Write position of cells_index
* @param q_point Query point
* @param q_cell_index_arr Index of the query point
* @param cell_min_index Start of index range
* @param cell_max_index End of index range
* @param dim Current dimension
* @param round Round of KNN search
* @param tmp_cell_index Current calculated index
* @param tmp_mindist Current calculated MINDIST
* @param tmp_chebdis Current calculated Chebyshev distance
*/
UMLI_TEMPLATE
void UMLI_QUAL::CalKNNIndex(int* cells_index, double* cells_MINDIST, int& write_index, ELEMTYPE* q_point, const int q_cell_index_arr[NUMDIMS], const int cell_min_index[NUMDIMS], const int cell_max_index[NUMDIMS], int dim, int round, int tmp_cell_index, double tmp_mindist, int tmp_chebdis)
{
	for (int i = cell_min_index[dim]; i <= cell_max_index[dim]; i++)
	{
		int new_tmp_cell_index = tmp_cell_index + i * unit[dim];

		double new_tmp_mindist = tmp_mindist;
		if (i < q_cell_index_arr[dim]) {
			new_tmp_mindist += (q_point[dim] - linear_scales[dim][i + 1]) * (q_point[dim] - linear_scales[dim][i + 1]);
		}
		else if (i == q_cell_index_arr[dim]) {
			new_tmp_mindist += 0;
		}
		else {
			new_tmp_mindist += (q_point[dim] - linear_scales[dim][i]) * (q_point[dim] - linear_scales[dim][i]);
		}
		int new_tmp_chebdis = fabs(i - q_cell_index_arr[dim]) > tmp_chebdis ? fabs(i - q_cell_index_arr[dim]) : tmp_chebdis;

		if (dim + 1 == NUMDIMS)
		{
			if (new_tmp_chebdis == round) {
				cells_index[write_index] = new_tmp_cell_index;
				cells_MINDIST[write_index] = new_tmp_mindist;
				write_index++;
			}
		}
		else
		{
			CalKNNIndex(cells_index, cells_MINDIST, write_index, q_point, q_cell_index_arr, cell_min_index, cell_max_index, dim + 1, round, new_tmp_cell_index, new_tmp_mindist, new_tmp_chebdis);
		}
	}
}


/**
* Learn the APLF of the given Cell over the sort dimension
*
* @param a_point A point represented by an array
* @return Index of the Cell where this point locates
*/
UMLI_TEMPLATE
void UMLI_QUAL::LearnAPLF(Cell& cell)
{
	cell.c_aplf.slope.clear();
	cell.c_aplf.x_intercept.clear();
	cell.c_aplf.y_intercept.clear();
	if (cell.point_arr.size() == 0)
	{
		return;
	}
	else
	{
		cell.c_aplf.slope.push_back(0);
		cell.c_aplf.x_intercept.push_back(cell.point_arr[0].p_val[sort_dim]);
		cell.c_aplf.y_intercept.push_back(0);
		for (int i = 1; i < cell.c_size; ++i)
		{
			double tmp_slope;
			double tmp_xdiff = cell.point_arr[i].p_val[sort_dim] - cell.c_aplf.x_intercept.back();
			double tmp_ydiff = i - cell.c_aplf.y_intercept.back();
			if (tmp_xdiff != 0)
			{
				tmp_slope = tmp_ydiff / tmp_xdiff;
			}
			else
			{
				tmp_slope = 1.7976931348623157E+308;
			}
			bool flag = true;
			for (int j = cell.c_aplf.y_intercept.back() + 1; j < i; ++j)
			{
				double err = abs(j - (tmp_slope * (cell.point_arr[j].p_val[sort_dim] - cell.c_aplf.x_intercept.back())));
				if (err >= error_bound) {
					flag = false;
					break;
				}
			}
			if (flag == true)
			{
				cell.c_aplf.slope.back() = tmp_slope;
			}
			else
			{
				cell.c_aplf.slope.push_back(0);
				cell.c_aplf.x_intercept.push_back(cell.point_arr[i].p_val[sort_dim]);
				cell.c_aplf.y_intercept.push_back(i);
			}
		}

	}
}


/**
* Caluculate the predicted position of a given key
*
* @param a_point A point represented by an array
* @param a_point A point represented by an array
* @return Index of the Cell where this point locates
*/
UMLI_TEMPLATE
int UMLI_QUAL::SearchAPLF(APLF aplf, ELEMTYPE key)
{
	for (int i = 0; i < aplf.slope.size() - 1; ++i)
	{
		if (key >= aplf.x_intercept[i] && key < aplf.x_intercept[i + 1])
		{
			int predict = aplf.y_intercept[i] + aplf.slope[i] * (key - aplf.x_intercept[i]);
			return predict;
		}
	}

	if (key >= aplf.x_intercept.back())
	{
		int predict = aplf.y_intercept.back() + aplf.slope.back() * (key - aplf.x_intercept.back());
		return predict;
	}

	if (key < aplf.x_intercept.front())
	{
		return 0;
	}
}


/**
* Calculate the Squared Euclidean distance between 2 points
*
* @param a_point A k-dimensional point represented by an array
* @param b_point A k-dimensional point represented by an array
* @return distance between 2 points
*/
UMLI_TEMPLATE
double UMLI_QUAL::CalEuc2Dis(const ELEMTYPE* a_point, const ELEMTYPE* b_point)
{
	double distance = 0;
	for (int i = 0; i < NUMDIMS; i++)
	{
		distance += (a_point[i] - b_point[i]) * (a_point[i] - b_point[i]);
	}
	return distance;
}


/**
* Calculate the MINDIST distance between a point and a rectangle
*
* @param a_point A k-dimensional point represented by an array
* @param b_min Lower bounds array of rectangle
* @param b_max Upper bounds array of rectangle
* @return distance between a point and a rectangle
*/
UMLI_TEMPLATE
double UMLI_QUAL::CalMINDIST(const ELEMTYPE* a_point, const ELEMTYPE* b_min, const ELEMTYPE* b_max)
{
	double distance = 0;
	for (int i = 0; i < NUMDIMS; i++)
	{
		if (a_point[i] < b_min[i]) {
			distance += (a_point[i] - b_min[i]) * (a_point[i] - b_min[i]);
		}
		else if (a_point[i] > b_max[i]) {
			distance += (a_point[i] - b_max[i]) * (a_point[i] - b_max[i]);
		}
		else {
			distance += 0;
		}
	}
	return distance;
}


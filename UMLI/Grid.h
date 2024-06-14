#pragma once

#include <iostream>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

#define MAX_DIS 99999999999999 /*1.7976931348623157E+308*/
#define GRID_TEMPLATE template<class DATATYPE, class ELEMTYPE, int NUMDIMS>		// Define template of kd tree
#define GRID_QUAL Grid<DATATYPE, ELEMTYPE, NUMDIMS>								// Define template use of kd tree


/**
* A class to represent a grid index
* Example usage: For a 2-dimensional grid use 'Grid<int, double, 2> grid;'
*
* DATATYPE Referenced data, such as int, void* or obj*
* ELEMTYPE Type of data attribute, such as int, float or double
* NUMDIMS Number of dimensions, such as 2 or 3
*/
GRID_TEMPLATE
class Grid 
{

protected:

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
	};

protected:

	int cell_num;							// Total number of cells
	int unit[NUMDIMS];						// Array to assist in calculating the cell's index
	int interval_num[NUMDIMS];				// Number of columns for each dimension
	vector<vector<ELEMTYPE>> linear_scales;	// Division points for each dimension
	Cell* grid_array;						// Array to store all cells

public:

	/**
	* Constructor without parameter
	*/
	Grid();

	/**
	* Constructor wwith parameter
	* 
	* @param linear_scales Separation points for each dimension
	*/
	Grid(vector<vector<ELEMTYPE>> linear_scales);

	/**
	* Destructor
	*/
	~Grid();

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
	* Calculate the memory footprint occupied by Grid
	*
	* @return Number of bytes occupied by kd tree
	*/
	int Footprint();

	/**
	* Output the size of each cell in the grid index
	*/
	void PrintGridSize();

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

	int CalPointIndex(const ELEMTYPE* a_point);
	void CalRangeIndex(int* cells_index, bool* is_fringe, int& write_index, const int cell_min_index[NUMDIMS], const int cell_max_index[NUMDIMS], int dim, int tmp_cell_index, bool tmp_fringe_flag);
	void CalKNNIndex(int* cells_index, double* cells_MINDIST, int& write_index, ELEMTYPE* q_point, const int q_cell_index_arr[NUMDIMS], const int cell_min_index[NUMDIMS], const int cell_max_index[NUMDIMS], int dim, int round, int tmp_cell_index, double tmp_mindist, int tmp_chebdis);

	double CalEuc2Dis(const ELEMTYPE* a_point, const ELEMTYPE* b_point);
	double CalMINDIST(const ELEMTYPE* a_point, const ELEMTYPE* b_min, const ELEMTYPE* b_max);


};


GRID_TEMPLATE
GRID_QUAL::Grid() {}


GRID_TEMPLATE
GRID_QUAL::Grid(vector<vector<ELEMTYPE>> linear_scales)
{
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
}


GRID_TEMPLATE
GRID_QUAL::~Grid() 
{
	if (grid_array != NULL)
	{
		delete[] grid_array;
		grid_array = NULL;
	}
}


GRID_TEMPLATE
void GRID_QUAL::Insert(const ELEMTYPE* a_point, DATATYPE& a_id)
{
	int cell_index = CalPointIndex(a_point);

	Point p;
	for (int dim = 0; dim < NUMDIMS; ++dim)
	{
		p.p_val[dim] = a_point[dim];
	}
	p.p_id = a_id;
	grid_array[cell_index].point_arr.push_back(p);
	grid_array[cell_index].c_size++;
}


GRID_TEMPLATE
void GRID_QUAL::Delete(const ELEMTYPE* a_point)
{
	int cell_index = CalPointIndex(a_point);
	for (int i = 0; i < grid_array[cell_index].c_size; i++)
	{
		bool flag = true;
		for (int d = 0; d < NUMDIMS; d++)
		{
			if (grid_array[cell_index].point_arr[i].p_val[d] != a_point[d])
			{
				flag = false;
			}
		}
		if (flag == true)
		{
			grid_array[cell_index].c_size--;
			grid_array[cell_index].point_arr.erase(grid_array[cell_index].point_arr.begin() + i);
		}
	}
}


GRID_TEMPLATE
int GRID_QUAL::Footprint()
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
	}

	return Footprint;
}


GRID_TEMPLATE
void GRID_QUAL::PrintGridSize()
{
	int interval_nums = 1;
	for (int i = 0; i < NUMDIMS; ++i)
	{
		interval_nums *= interval_num[i];
	}
	for (int i = 0; i < interval_nums; ++i)
	{
		cout << "number of points in cell" << i << ":\t" << grid_array[i].c_size << endl;
	}
}


GRID_TEMPLATE
bool GRID_QUAL::PointQuery(vector<DATATYPE>& Result, const ELEMTYPE* q_point)
{
	//cout << endl;
	//cout << "Grid PointQuery Time: ";
	//auto start1 = high_resolution_clock::now();


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

	//auto stop1 = high_resolution_clock::now();
	//auto duration1 = duration_cast<microseconds>(stop1 - start1);
	//cout << duration1.count();
	//cout << "+";
	//auto start2 = high_resolution_clock::now();


	for (int i = 0; i < grid_array[cell_index].c_size; i++) {
		int flag = 1;
		for (int j = 0; j < NUMDIMS; ++j)
		{
			if (grid_array[cell_index].point_arr[i].p_val[j] != q_point[j])
			{
				flag = 0;
				break;
			}
		}
		if (flag==1) {
			int id = grid_array[cell_index].point_arr[i].p_id;
			Result.push_back(id);
			break;
		}
	}

	//auto stop2 = high_resolution_clock::now();
	//auto duration2 = duration_cast<microseconds>(stop2 - start2);
	//cout << duration2.count();
	//cout << "(us)" << endl;


	return Result.size();
}


GRID_TEMPLATE
int GRID_QUAL::RangeQuery(vector<DATATYPE>& Result, ELEMTYPE* q_min, ELEMTYPE* q_max)
{
	int foundCount = 0;

	int cell_min_index[NUMDIMS];
	int cell_max_index[NUMDIMS];

	// Find index of Cells that intersect with the range query
	for (int dim = 0; dim < NUMDIMS; dim++) {
		for (int i = 0; i < interval_num[dim]; i++) {
			if ((q_min[dim] > linear_scales[dim][i]) && (q_min[dim] <= linear_scales[dim][i + 1])) {
				cell_min_index[dim] = i;
			}
			if ((q_max[dim] > linear_scales[dim][i]) && (q_max[dim] <= linear_scales[dim][i + 1])) {
				cell_max_index[dim] = i;
				break;
			}
		}
	}

	// calculate the number of the cells that will be searched
	int intersect_num = 1;
	for (int dim = 0; dim < NUMDIMS; dim++) {
		intersect_num *= cell_max_index[dim] - cell_min_index[dim] + 1;
	}

	int* cells_index = new int[intersect_num];
	bool* is_fringe = new bool[intersect_num];
	int write_index = 0;
	CalRangeIndex(cells_index, is_fringe, write_index, cell_min_index, cell_max_index, 0, 0, false);


	for (int i = 0; i < intersect_num; i++)
	{
		int index = cells_index[i];
		if (is_fringe[i] == false)
		{
			for (int j = 0; j < grid_array[index].c_size; ++j)
			{
				int id = grid_array[index].point_arr[j].p_id;
				Result.push_back(id);
				foundCount++;
			}
		}
		else
		{
			for (int j = 0; j < grid_array[index].c_size; ++j) {
				bool flag = true;
				for (int dim = 0; dim < NUMDIMS; ++dim)
				{
					if (grid_array[index].point_arr[j].p_val[dim] < q_min[dim] || grid_array[index].point_arr[j].p_val[dim] > q_max[dim])
					{
						flag = false;
						break;
					}
				}
				if (flag == true) {
					int id = grid_array[index].point_arr[j].p_id;
					Result.push_back(id);
					foundCount++;
				}
			}
		}
	

	}

	delete[] cells_index;
	delete[] is_fringe;

	return foundCount;
}


GRID_TEMPLATE
void GRID_QUAL::KNNQuery(DATATYPE* Result, ELEMTYPE* q_point, int k)
{
	int q_cell_index = 0;
	int q_cell_index_arr[NUMDIMS];

	double* NearestDis = new double[k];
	for (int i = 0; i < k; i++) {
		NearestDis[i] = MAX_DIS;
	}

	// Find index of the Cell where query point locates
	for (int dim = 0; dim < NUMDIMS; ++dim)
	{
		for (int j = 0; j < linear_scales[dim].size() - 1; ++j)
		{
			if ((q_point[dim] >= linear_scales[dim][j]) && (q_point[dim] < linear_scales[dim][j + 1]))
			{
				q_cell_index_arr[dim] = j;
				q_cell_index += unit[dim] * j;
				break;
			}
		}
	}

	// Search in cells
	int round = 0;
	double min = MAX_DIS;
	do
	{
		int cell_min_index[NUMDIMS];
		int cell_max_index[NUMDIMS];
		for (int dim = 0; dim < NUMDIMS; dim++) {
			cell_min_index[dim] = q_cell_index_arr[dim] - round >= 0 ? (q_cell_index_arr[dim] - round) : 0;
			cell_max_index[dim] = q_cell_index_arr[dim] + round <= (linear_scales[dim].size() - 2) ? (q_cell_index_arr[dim] + round) : (linear_scales[dim].size() - 2);
		}

		// calculate the number of the cells that will be searched
		int search_num = 1;
		if (round == 0) {
			search_num = 1;
		}
		else {
			int last_cell_min_index[NUMDIMS];
			int last_cell_max_index[NUMDIMS];
			for (int dim = 0; dim < NUMDIMS; dim++) {
				last_cell_min_index[dim] = q_cell_index_arr[dim] - (round - 1) >= 0 ? (q_cell_index_arr[dim] - (round - 1)) : 0;
				last_cell_max_index[dim] = q_cell_index_arr[dim] + (round - 1) <= (linear_scales[dim].size() - 2) ? (q_cell_index_arr[dim] + (round - 1)) : (linear_scales[dim].size() - 2);
			}
			for (int dim = 0; dim < NUMDIMS; dim++) {
				search_num *= cell_max_index[dim] - cell_min_index[dim] + 1;
			}

			int last_search_num = 1;
			for (int dim = 0; dim < NUMDIMS; dim++) {
				last_search_num *= last_cell_max_index[dim] - last_cell_min_index[dim] + 1;
			}
			search_num = search_num - last_search_num;
		}

		// calculate the index of the cells that will be searched
		int* cells_index = new int[search_num];
		double* cells_MINDIST = new double[search_num];
		int write_index = 0;
		CalKNNIndex(cells_index, cells_MINDIST, write_index, q_point, q_cell_index_arr, cell_min_index, cell_max_index, 0, round, 0, 0, 0);

		// Search in the Cell where query point locates
		for (int i = 0; i < search_num; i++) {
			int index = cells_index[i];
			if (cells_MINDIST[i] < NearestDis[k - 1]) {

				for (int j = 0; j < grid_array[index].c_size; ++j)
				{
					double distance = CalEuc2Dis(q_point, grid_array[index].point_arr[j].p_val);
					if (distance < NearestDis[k - 1]) {
						NearestDis[k - 1] = distance;
						Result[k - 1] = grid_array[index].point_arr[j].p_id;
						for (int x = 0; x < k - 1; x++) {
							for (int y = 0; y < k - 1 - x; y++) {
								if (NearestDis[y] > NearestDis[y + 1]) {
									double tmp = NearestDis[y];
									NearestDis[y] = NearestDis[y + 1];
									NearestDis[y + 1] = tmp;
									int tmpo = Result[y];
									Result[y] = Result[y + 1];
									Result[y + 1] = tmpo;
								}
							}
						}
					}
				}
			}			
		}

		// calculate min distance from query point to Cell border
		min = MAX_DIS;
		for (int dim = 0; dim < NUMDIMS; dim++)
		{
			int border_index_lower = q_cell_index_arr[dim] - round;
			int border_index_upper = q_cell_index_arr[dim] + round + 1;
			if (border_index_lower > 0)
			{
				double dis2min = (q_point[dim] - linear_scales[dim][border_index_lower]) * (q_point[dim] - linear_scales[dim][border_index_lower]);
				if (dis2min < min)
				{
					min = dis2min;
				}
			}
			if (border_index_upper < linear_scales[dim].size()-1)
			{
				double dis2max = (q_point[dim] - linear_scales[dim][border_index_upper]) * (q_point[dim] - linear_scales[dim][border_index_upper]);
				if (dis2max < min)
				{
					min = dis2max;
				}
			}
		}

		delete[] cells_index;
		delete[] cells_MINDIST;

		round++;


	} while (min < NearestDis[k - 1]);

}




/**
* Given a point, find the index of the Cell where this point locates
*
* @param a_point A point represented by an array
* @return Index of the Cell where this point locates
*/
GRID_TEMPLATE
int GRID_QUAL::CalPointIndex(const ELEMTYPE* a_point)
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
GRID_TEMPLATE
void GRID_QUAL::CalRangeIndex(int* cells_index, bool* is_fringe, int& write_index, const int cell_min_index[NUMDIMS], const int cell_max_index[NUMDIMS], int dim, int tmp_cell_index, bool tmp_fringe_flag)
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
			CalRangeIndex(cells_index, is_fringe, write_index, cell_min_index, cell_max_index, dim+1, new_tmp_cell_index, new_tmp_fringe_flag);
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
GRID_TEMPLATE
void GRID_QUAL::CalKNNIndex(int* cells_index, double* cells_MINDIST, int& write_index, ELEMTYPE* q_point, const int q_cell_index_arr[NUMDIMS], const int cell_min_index[NUMDIMS], const int cell_max_index[NUMDIMS], int dim, int round, int tmp_cell_index, double tmp_mindist, int tmp_chebdis)
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
* Calculate the Squared Euclidean distance between 2 points
*
* @param a_point A k-dimensional point represented by an array
* @param b_point A k-dimensional point represented by an array
* @return distance between 2 points
*/
GRID_TEMPLATE
double GRID_QUAL::CalEuc2Dis(const ELEMTYPE* a_point, const ELEMTYPE* b_point)
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
GRID_TEMPLATE
double GRID_QUAL::CalMINDIST(const ELEMTYPE* a_point, const ELEMTYPE* b_min, const ELEMTYPE* b_max)
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
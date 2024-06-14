#pragma once

#include <vector>

using namespace std;

#define MAX_DIS 99999999999999 /*1.7976931348623157E+308*/
#define FULLSCAN_TEMPLATE template<class DATATYPE, class ELEMTYPE, int NUMDIMS>     // Define template of kd tree
#define FULLSCAN_QUAL FullScan<DATATYPE, ELEMTYPE, NUMDIMS>							// Define template use of kd tree


/**
* A class to perform a full scan of the dataset
* Example usage: For 2-dimensional dataset use 'FullScan<int, double, 2> fullscan;'
*
* DATATYPE Referenced data, such as int, void* or obj*
* ELEMTYPE Type of data attribute, such as int, float or double
* NUMDIMS Number of dimensions, such as 2 or 3
*/
FULLSCAN_TEMPLATE
class FullScan
{

protected:

    /**
    * A structure to represent the data record in grid index
    */
    struct Point {
        ELEMTYPE p_val[NUMDIMS];
        DATATYPE p_id;
    };

protected:

    int m_size;
	vector<Point> data_arr; // all data

public:
	
    /**
    * Constructor without parameter
    */
    FullScan();

    /**
    * Insert a new point
    *
    * @param a_point A k-dimensional point represented by an array
    */
    void Insert(const ELEMTYPE* a_point, DATATYPE& a_id);

    /**
    * Delete a point
    *
    * @param a_point A k-dimensional point represented by an array
    * @return Whether the point is found and deleted
    */
    bool Delete(const ELEMTYPE* a_point);

    /**
    * Search a point
    *
    * @param Result A vector containing results of query
    * @param q_point A k-dimensional point represented by an array
    * @return Whether query point exist
    */
	bool PointQuery(vector<DATATYPE>& Result, ELEMTYPE* q_point);

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
    * @param a_point Queried k-dimensional point represented by an array
    * @param k Number of nearest neighbours
    */
	void KNNQuery(DATATYPE* Result, double* q_point, int& k);

protected:

	double CalEuc2Dis(const ELEMTYPE* a_point, const ELEMTYPE* b_point);

};



FULLSCAN_TEMPLATE
FULLSCAN_QUAL::FullScan()
{
    m_size = 0;
}


FULLSCAN_TEMPLATE
void FULLSCAN_QUAL::Insert(const ELEMTYPE* a_point, DATATYPE& a_id)
{
    Point p;
    for (int dim = 0; dim < NUMDIMS; ++dim)
    {
        p.p_val[dim] = a_point[dim];
    }
    p.p_id = a_id;
    data_arr.push_back(p);
    m_size++;
}


FULLSCAN_TEMPLATE
bool FULLSCAN_QUAL::Delete(const ELEMTYPE* a_point)
{
    for (int i = 0; i < m_size; ++i)
    {
        bool flag = 1;
        for (int j = 0; j < NUMDIMS; ++j)
        {
            if (data_arr[i].p_val[j] != a_point[j])
            {
                flag = 0;
                break;
            }
        }
        if (flag == 1)
        {
            this->data_arr.erase(this->data_arr.begin() + i);
            m_size--;
            return true;
        }
    }
    return false;
}


FULLSCAN_TEMPLATE
bool FULLSCAN_QUAL::PointQuery(vector<DATATYPE>& Result, ELEMTYPE* q_point) 
{
    int foundCount = 0;
    for (int index = 0; index < m_size; index++) {
        bool flag = 1;
        for (int dim = 0; dim < NUMDIMS; dim++) {
            if (data_arr[index].p_val[dim] != q_point[dim]) {
                flag = 0;
                break;
            }
        }
        if (flag == 1) {
            foundCount++;
            Result.push_back(data_arr[index].p_id);
        }
    }
    return foundCount;
}


FULLSCAN_TEMPLATE
int FULLSCAN_QUAL::RangeQuery(vector<DATATYPE>& Result, ELEMTYPE* q_min, ELEMTYPE* q_max) 
{
    int foundCount = 0;
    for (int index = 0; index < m_size; index++) {
        bool flag = 1;
        for (int dim = 0; dim < NUMDIMS; dim++) {
            if ((data_arr[index].p_val[dim] < q_min[dim]) || (data_arr[index].p_val[dim] > q_max[dim])) {
                flag = 0;
                break;
            }
        }
        if (flag == 1) {
            foundCount++;
            Result.push_back(data_arr[index].p_id);
        }
    }
    return foundCount;
}


FULLSCAN_TEMPLATE
void FULLSCAN_QUAL::KNNQuery(DATATYPE* Result, double* q_point, int& k) 
{
    double* NearestDis = new double[k];
    for (int i = 0; i < k; i++) {
        NearestDis[i] = MAX_DIS;
    }
    for (int index = 0; index < m_size; index++) {
        double Dis = CalEuc2Dis(q_point, data_arr[index].p_val);
        if (Dis < NearestDis[k - 1]) {
            NearestDis[k - 1] = Dis;
            Result[k - 1] = data_arr[index].p_id;
            for (int x = 0; x < k - 1; x++) {
                for (int y = 0; y < k - 1 - x; y++) {
                    if (NearestDis[y] > NearestDis[y + 1]) {
                        double tmp = NearestDis[y];
                        NearestDis[y] = NearestDis[y + 1];
                        NearestDis[y + 1] = tmp;
                        DATATYPE tmpo = Result[y];
                        Result[y] = Result[y + 1];
                        Result[y + 1] = tmpo;
                    }
                }
            }
        }
    }
    return;
}


/**
* Calculate the Squared Euclidean distance between 2 points
*
* @param a_point A k-dimensional point represented by an array
* @param b_point A k-dimensional point represented by an array
* @return distance between 2 points
*/
FULLSCAN_TEMPLATE
double FULLSCAN_QUAL::CalEuc2Dis(const ELEMTYPE* a_point, const ELEMTYPE* b_point) {
    double distance = 0;
    for (int i = 0; i < NUMDIMS; i++)
    {
        distance += (a_point[i] - b_point[i]) * (a_point[i] - b_point[i]);
    }
    return distance;
}
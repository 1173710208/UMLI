#pragma once

#include <iostream>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

#define MAX_DIS 99999999999999 /*1.7976931348623157E+308*/
#define KDTREE_TEMPLATE template<class DATATYPE, class ELEMTYPE, int NUMDIMS>		// Define template of kd tree
#define KDTREE_QUAL KDTree<DATATYPE, ELEMTYPE, NUMDIMS>								// Define template use of kd tree


/**
* A class to represent a kd tree
* Example usage: For a 2-dimensional tree use 'KDTree<int, double, 2> kdtree;'
*
* DATATYPE Referenced data, such as int, void* or obj*
* ELEMTYPE Type of data attribute, such as int, float or double
* NUMDIMS Number of dimensions, such as 2 or 3
*/
KDTREE_TEMPLATE
class KDTree
{
protected:

	/**
	* A structure to represent node of kd tree
	*/
	struct Node
	{
		DATATYPE m_id;					// Record ID
		ELEMTYPE m_point[NUMDIMS];		// Values of stored k-dimensional data point: 
		Node* m_lchild;					// Left child node: NULL
		Node* m_rchild;					// Right child node: NULL

		bool IsLchildNUll() { return (m_lchild == NULL); }	// Judge whether a node's left child is NULL
		bool IsRchildNUll() { return (m_rchild == NULL); }	// Judge whether a node's right child is NULL
	};

protected:

	Node* m_root;	//Root of kd tree

public:

	/**
	* Constructor without parameter
	*/
	KDTree();

	/**
	* Destructor
	*/
	~KDTree();

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
	* Calculate the memory footprint occupied by kd tree
	*
	* @return Number of bytes occupied by kd tree
	*/
	int Footprint();

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
	int RangeQuery(vector<DATATYPE>& Result, const ELEMTYPE* q_min, const ELEMTYPE* q_max);

	/**
	* Search k nearest neighbours of a point
	*
	* @param Result A vector containing results of query
	* @param a_point Queried k-dimensional point represented by an array
	* @param k Number of nearest neighbours
	*/
	void KNNQuery(DATATYPE* Result, const ELEMTYPE* q_point, int& k);

protected:

	void RemoveRec(Node* a_node);

	Node* InsertRec(Node* a_node, const ELEMTYPE* a_point, DATATYPE& a_id, int depth);
	Node* NewNode(const ELEMTYPE* a_point, DATATYPE& a_id);

	Node* DeleteRec(Node* root, const ELEMTYPE* a_point, int depth);
	Node* MinNode(Node* x, Node* y, Node* z, int d);
	Node* FindMin(Node* root, int d, int depth);
	void CopyPoint(ELEMTYPE* point1, ELEMTYPE* point2);

	int FootprintRec(Node* a_node);

	bool PointQueryRec(vector<DATATYPE>& Result, const ELEMTYPE* q_point, Node* a_node, int depth);
	bool SamePoint(const ELEMTYPE* point1, const ELEMTYPE* point2);

	int RangeQueryRec(vector<DATATYPE>& Result, const ELEMTYPE* q_min, const ELEMTYPE* q_max, Node* a_node, int depth);
	bool InRect(Node* a_node, const ELEMTYPE* a_min, const ELEMTYPE* a_max);

	void KNNQueryRec(DATATYPE* Result, const ELEMTYPE* q_point, int& k, Node* a_node, int depth, const ELEMTYPE* b_min, const ELEMTYPE* b_max, double* NearestDis);
	double CalEuc2Dis(const ELEMTYPE* a_point, const ELEMTYPE* b_point);
	double CalMINDIST(const ELEMTYPE* a_point, const ELEMTYPE* b_min, const ELEMTYPE* b_max);

};


KDTREE_TEMPLATE
KDTREE_QUAL::KDTree()
{
	m_root = NULL;
}


KDTREE_TEMPLATE
KDTREE_QUAL::~KDTree()
{
	RemoveRec(m_root);
}


KDTREE_TEMPLATE
void KDTREE_QUAL::Insert(const ELEMTYPE* a_point, DATATYPE& a_id)
{
	m_root = InsertRec(m_root, a_point, a_id, 0);
}


KDTREE_TEMPLATE
void KDTREE_QUAL::Delete(const ELEMTYPE* a_point)
{
	DeleteRec(m_root, a_point, 0);
}

KDTREE_TEMPLATE
int KDTREE_QUAL::Footprint()
{
	int Footprint = 0;
	Footprint += sizeof(KDTree);
	Footprint += FootprintRec(m_root);
	return Footprint;
}


KDTREE_TEMPLATE
bool KDTREE_QUAL::PointQuery(vector<DATATYPE>& Result, const ELEMTYPE* q_point)
{
	//cout << "KD PointQuery Time: ";
	//auto start = high_resolution_clock::now();

	bool flag = PointQueryRec(Result, q_point, m_root, 0);

	//auto stop = high_resolution_clock::now();
	//auto duration = duration_cast<microseconds>(stop - start);
	//cout << duration.count();
	//cout << "(us)" << endl;

	return flag;

}


KDTREE_TEMPLATE
int KDTREE_QUAL::RangeQuery(vector<DATATYPE>& Result, const ELEMTYPE* q_min, const ELEMTYPE* q_max) {
	int foundCount = 0;
	foundCount = RangeQueryRec(Result, q_min, q_max, m_root, 0);
	return foundCount;
}


KDTREE_TEMPLATE
void KDTREE_QUAL::KNNQuery(DATATYPE* Result, const ELEMTYPE* q_point, int& k)
{
	// the k closest distances encountered at any phase of search
	double* NearestDis = new double[k];
	for (int i = 0; i < k; i++) {
		NearestDis[i] = MAX_DIS;
	}

	// upper & lower bounds array
	ELEMTYPE* b_min = new ELEMTYPE[NUMDIMS];
	ELEMTYPE* b_max = new ELEMTYPE[NUMDIMS];
	for (int i = 0; i < NUMDIMS; i++)
	{
		b_min[i] = -MAX_DIS;
		b_max[i] = MAX_DIS;
	}

	KNNQueryRec(Result, q_point, k, m_root, 0, b_min, b_max, NearestDis);
}


/**
* Recursively remove all childern nodes of a node
*
* @param a_node Pointer to a node
*/
KDTREE_TEMPLATE
void KDTREE_QUAL::RemoveRec(Node* a_node)
{
	if (!(a_node->IsLchildNUll()))
	{
		RemoveRec(a_node->m_lchild);
	}
	if (!a_node->IsRchildNUll())
	{
		RemoveRec(a_node->m_rchild);
	}
	delete a_node;
}


/**
* Recursively insert a new node
*
* @param a_node Pointer to the root of a tree or subtree
* @param a_point A k-dimensional point represented by an array
* @param a_id ID of record
* @param depth Depth of a_node
* @return Pointer to the root of modified tree
*/
KDTREE_TEMPLATE
typename KDTREE_QUAL::Node* KDTREE_QUAL::InsertRec(Node* a_node, const ELEMTYPE* a_point, DATATYPE& a_id, int depth)
{
	// if Tree is empty
	if (a_node == NULL)
		return NewNode(a_point, a_id);

	// calculate current dimension of comparison: Discriminator
	int Disc = depth % NUMDIMS;

	// compare the new point with a_node on current dimension and decide the left or right subtree
	if (a_point[Disc] < (a_node->m_point[Disc]))
	{
		a_node->m_lchild = InsertRec(a_node->m_lchild, a_point, a_id, depth + 1);
	}
	else
	{
		a_node->m_rchild = InsertRec(a_node->m_rchild, a_point, a_id, depth + 1);
	}

	return a_node;
}


/**
* Delete a given point from tree with root
* 
* @param root Root of the current tree
* @param a_point A k-dimensional point represented by an array
* @param depth Current depth
* @return root of the modified tree
*/
KDTREE_TEMPLATE
typename KDTREE_QUAL::Node* KDTREE_QUAL::DeleteRec(Node* root, const ELEMTYPE* a_point, int depth)
{
	// Given point is not present
	if (root == NULL)
		return NULL;

	// Find dimension of current node
	int Disc = depth % NUMDIMS;

	// If the point to be deleted is present at root
	if (SamePoint(root->m_point, a_point))
	{
		// 2.b) If right child is not NULL
		if (!root->IsRchildNUll())
		{
			// Find minimum of root's dimension in right subtree
			Node* min = FindMin(root->m_rchild, Disc, 0);

			// Copy the minimum to root
			CopyPoint(root->m_point, min->m_point);

			// Recursively delete the minimum
			root->m_rchild = DeleteRec(root->m_rchild, min->m_point, depth + 1);
		}
		else if (!root->IsLchildNUll()) // same as above
		{
			Node* min = FindMin(root->m_lchild, Disc, 0);
			CopyPoint(root->m_point, min->m_point);
			root->m_rchild = DeleteRec(root->m_lchild, min->m_point, depth + 1);
		}
		else // If node to be deleted is leaf node
		{
			delete root;
			return NULL;
		}
		return root;
	}

	// 2) If current node doesn't contain point, search downward
	if (a_point[Disc] < root->m_point[Disc])
		root->m_lchild = DeleteRec(root->m_lchild, a_point, depth + 1);
	else
		root->m_rchild = DeleteRec(root->m_rchild, a_point, depth + 1);
	return root;
}


/**
* Create and initialize a node of kd tree
*
* @param a_point A k-dimensional point represented by an array where last item is id
* @param a_id ID of record
* @return Pointer to the new node
*/
KDTREE_TEMPLATE
typename KDTREE_QUAL::Node* KDTREE_QUAL::NewNode(const ELEMTYPE* a_point, DATATYPE& a_id)
{
	Node* newNode = new Node;
	newNode->m_id = a_id;
	for (int i = 0; i < NUMDIMS; i++) {
		newNode->m_point[i] = a_point[i];
	}
	newNode->m_lchild = NULL;
	newNode->m_rchild = NULL;
	return newNode;
}


/**
* A utility function to find minimum of three integers
*
* @param x Node1
* @param y Node2
* @param z Node3
* @param d Discriminator
* @return minimum of three nodes
*/
KDTREE_TEMPLATE
typename KDTREE_QUAL::Node* KDTREE_QUAL::MinNode(Node* x, Node* y, Node* z, int d)
{
	Node* res = x;
	if (y != NULL && y->m_point[d] < res->m_point[d])
		res = y;
	if (z != NULL && z->m_point[d] < res->m_point[d])
		res = z;
	return res;
}


/**
* Recursively finds minimum of d'th dimension in KD tree
* 
* @param root Root of the current tree
* @param d Discriminator
* @param depth Depth of current root
*/
KDTREE_TEMPLATE
typename KDTREE_QUAL::Node* KDTREE_QUAL::FindMin(Node* root, int d, int depth)
{
	// Base cases
	if (root == NULL)
		return NULL;

	// Current dimension is computed using current depth and total dimensions (k)
	unsigned Disc = depth % NUMDIMS;

	// Compare point with root with respect to Disc (Current dimension)
	if (Disc == d)
	{
		if (root->m_lchild == NULL)
			return root;
		return FindMin(root->m_lchild, d, depth + 1);
	}

	// If current dimension is different then minimum can be anywhere in this subtree
	return MinNode(root,
		FindMin(root->m_lchild, d, depth + 1),
		FindMin(root->m_rchild, d, depth + 1), d);
}


/**
* Copies point p2 to p1
* 
* @param point1 A k-dimensional point represented by an array
* @param point2 A k-dimensional point represented by an array
*/
KDTREE_TEMPLATE
void KDTREE_QUAL::CopyPoint(ELEMTYPE* point1, ELEMTYPE* point2)
{
	for (int i = 0; i < NUMDIMS; i++)
		point1[i] = point2[i];
}


/**
* Recursively calculate the memory space occupied by a tree
*
* @param a_node Pointer to the root of a tree or subtree
* @return Number of bytes occupied by a tree
*/
KDTREE_TEMPLATE
int KDTREE_QUAL::FootprintRec(Node* a_node)
{
	int Footprint = 0;

	// base case
	if (a_node == NULL)
	{
		Footprint += 0;
	}
	Footprint += sizeof(*a_node);

	if (a_node->m_lchild != NULL)
	{
		Footprint += FootprintRec(a_node->m_lchild);

	}
	if (a_node->m_rchild != NULL)
	{
		Footprint += FootprintRec(a_node->m_rchild);
	}

	return Footprint;
}


/**
* Recursively search a point
*
* @param Result A vector containing results of query
* @param q_point Queried k-dimensional point represented by an array
* @param a_node Pointer to the root of a tree or subtree
* @param depth Depth of a_node
* @return Whether query point exist
*/
KDTREE_TEMPLATE
bool KDTREE_QUAL::PointQueryRec(vector<DATATYPE>& Result, const ELEMTYPE* q_point, Node* a_node, int depth)
{
	// base cases
	if (a_node == NULL)
	{
		return false;
	}
	if (SamePoint(a_node->m_point, q_point))
	{
		Result.push_back(a_node->m_id);
		return true;
	}

	// calculate current dimension of comparison: Discriminator
	int Disc = depth % NUMDIMS;

	if (q_point[Disc] < a_node->m_point[Disc])
		return PointQueryRec(Result, q_point, a_node->m_lchild, depth + 1);

	return PointQueryRec(Result, q_point, a_node->m_rchild, depth + 1);
}


/**
* Determine whether two Points are same
*
* @param point1 A k-dimensional point represented by an array
* @param point2 A k-dimensional point represented by an array
* @return Whether two Points are same
*/
KDTREE_TEMPLATE
bool KDTREE_QUAL::SamePoint(const ELEMTYPE* point1, const ELEMTYPE* point2)
{
	for (int i = 0; i < NUMDIMS; i++)
	{
		if (point1[i] != point2[i])
		{
			return false;
		}
	}
	return true;
}


/**
* Recursively search all points within a hyper-rectangle range
*
* @param Result A vector containing results of query
* @param q_min Min bounds of query rectangle
* @param q_max Max bounds of query rectangle
* @param a_node Pointer to the root of a tree or subtree
* @param depth Depth of a_node
* @return Number of records met the query
*/
KDTREE_TEMPLATE
int KDTREE_QUAL::RangeQueryRec(vector<DATATYPE>& Result, const ELEMTYPE* q_min, const ELEMTYPE* q_max, Node* a_node, int depth)
{
	int foundCount = 0;
	// determine current node meet the query (R1)
	if (InRect(a_node, q_min, q_max)) {
		Result.push_back(a_node->m_id);
		foundCount += 1;
	}

	// calculate current dimension of comparison: Discriminator
	int Disc = depth % NUMDIMS;

	// search subtrees
	if ((!(a_node->IsLchildNUll())) && (q_min[Disc] < a_node->m_point[Disc]))
	{
		foundCount += RangeQueryRec(Result, q_min, q_max, a_node->m_lchild, depth + 1);
	}
	if ((!(a_node->IsRchildNUll())) && (q_max[Disc] >= a_node->m_point[Disc]))
	{
		foundCount += RangeQueryRec(Result, q_min, q_max, a_node->m_rchild, depth + 1);
	}
	return foundCount;
}


/**
* Determine whether a point is inside a rectangle
*
* @param a_node Pointer to the root of a tree or subtree
* @param a_min Min bounds of query rectangle
* @param a_max Max bounds of query rectangle
* @return True if the point is inside the rectangle
*/
KDTREE_TEMPLATE
bool KDTREE_QUAL::InRect(Node* a_node, const ELEMTYPE* a_min, const ELEMTYPE* a_max) {
	for (int i = 0; i < NUMDIMS; i++)
	{
		if ((a_node->m_point[i] > a_max[i]) || (a_node->m_point[i] < a_min[i]))
		{
			return false;
		}
	}
	return true;
}


/**
* Recursively search k nearest neighbours of a point
*
* @param Result A vector containing results of query
* @param q_point Queried k-dimensional point represented by an array
* @param k Number of nearest neighbours
* @param a_node Pointer to the root of a tree or subtree
* @param depth Depth of a_node
* @param b_min Lower bounds array
* @param b_max Upper bounds array
* @param NearestDis Closest distances array
*/
KDTREE_TEMPLATE
void KDTREE_QUAL::KNNQueryRec(DATATYPE* Result, const ELEMTYPE* q_point, int& k, Node* a_node, int depth, const ELEMTYPE* b_min, const ELEMTYPE* b_max, double* NearestDis) {
	// square of bounds-overlap-ball's radius 
	double r_2 = NearestDis[k - 1];

	// examine current node 
	double dist = CalEuc2Dis(a_node->m_point, q_point);
	if (dist < NearestDis[k - 1])
	{
		NearestDis[k - 1] = dist;
		Result[k - 1] = a_node->m_id;
		for (int i = 0; i < k - 1; i++) {
			for (int j = 0; j < k - 1 - i; j++) {
				if (NearestDis[j] > NearestDis[j + 1]) {
					double tmp = NearestDis[j];
					NearestDis[j] = NearestDis[j + 1];
					NearestDis[j + 1] = tmp;
					DATATYPE tmpo = Result[j];
					Result[j] = Result[j + 1];
					Result[j + 1] = tmpo;
				}
			}
		}
	}

	// calculate current dimension of comparison: Discriminator
	int Disc = depth % NUMDIMS;

	// recursive call on closer child
	ELEMTYPE* new_b_min = new ELEMTYPE[NUMDIMS];
	ELEMTYPE* new_b_max = new ELEMTYPE[NUMDIMS];
	for (int i = 0; i < NUMDIMS; i++)
	{
		new_b_min[i] = b_min[i];
		new_b_max[i] = b_max[i];
	}
	new_b_min[Disc] = a_node->m_point[Disc];
	new_b_max[Disc] = a_node->m_point[Disc];
	double l_dist = CalMINDIST(q_point, b_min, new_b_max);
	double r_dist = CalMINDIST(q_point, new_b_min, b_max);

	if (l_dist < r_dist)
	{
		if (l_dist <= r_2 && !a_node->IsLchildNUll())
		{
			KNNQueryRec(Result, q_point, k, a_node->m_lchild, depth + 1, b_min, new_b_max, NearestDis);
		}
		if (r_dist <= r_2 && !a_node->IsRchildNUll())
		{
			KNNQueryRec(Result, q_point, k, a_node->m_rchild, depth + 1, new_b_min, b_max, NearestDis);
		}
	}
	else
	{
		if (r_dist <= r_2 && !a_node->IsRchildNUll())
		{
			KNNQueryRec(Result, q_point, k, a_node->m_rchild, depth + 1, new_b_min, b_max, NearestDis);
		}
		if (l_dist <= r_2 && !a_node->IsLchildNUll())
		{
			KNNQueryRec(Result, q_point, k, a_node->m_lchild, depth + 1, b_min, new_b_max, NearestDis);
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
KDTREE_TEMPLATE
double KDTREE_QUAL::CalEuc2Dis(const ELEMTYPE* a_point, const ELEMTYPE* b_point)
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
KDTREE_TEMPLATE
double KDTREE_QUAL::CalMINDIST(const ELEMTYPE* a_point, const ELEMTYPE* b_min, const ELEMTYPE* b_max)
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



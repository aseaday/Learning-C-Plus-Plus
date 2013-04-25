#ifndef _RANDFOREST_H_
#define _RANDFOREST_H_

//..
#include "randFeature.h"

using namespace cv;

#define LUT_BIN_NUM    SKE_JOINT_NUM
#define EPS            1e-8

#define TREE_MAX_LEVEL 8		    //max level of tree

#define TREE_MIN_POINT_NUM   1000   //the min number of point for each node,
                                    //if the point number less than it, the node 
									//will not split

#define TREE_ENTROPY_THRESH  0.5    //if the entropy is less than this value, the
									//node will not split

/*
  Node structure for the random tree
*/
typedef struct pslNode{

	int nodeID;
	int   feaIdx;
	pslFeature fea;
	float LUT[LUT_BIN_NUM];	//the probability distribution
	float entropy;				//the entropy of the LUT
	int   pointNum;				//the number of pixels in this subtree
	int   level;				//the level of current node
	int  isLeaf;				//flag to show if the node is a leaf node
	int   pointLink;			//the link to the points list of this subtree
	                            //is valid only for the leaf node

	pslNode* Child0;
	pslNode* Child1;

	pslNode(){
		feaIdx = -1;
		memset(LUT, 0, LUT_BIN_NUM*sizeof(float));
		entropy = 0.f;
		pointNum = 0;
		level = -1;
		Child0 = 0;
		Child1 = 0;
		isLeaf = 1;
		pointLink = -1;
	}
} pslNode;

typedef vector<pslNode*> pslNodeList;


//....
extern pslFeatureList  *featureList;
extern pslKeypointList *keypointList;
extern bitsMat   *feaMat;  
extern pslNode *gbTree;
//....



/* STEP: 3
   Init Random tree
   Note: before run this program, the feature list and keypoint list should
   be ready.
*/
int initRandomTree();

/* STEP: 4
   Build Random tree
   Note: build the random tree, the root of the tree is: _tree_
*/
int buildRandomTree();

/* STEP: 5
   Clear the random tree
*/
int releaseRandomTree();


/*
   pridict the point value by rand tree
*/
int doRandomTreePredict(pslKeypoint* kp, pslNode* rndTree);

//..for debug only
int dummyTest();
#endif
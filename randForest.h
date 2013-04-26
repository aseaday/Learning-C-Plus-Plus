/*
  Briefly, this and its corresponding randForest.cpp
  file do one thing---->Construct a tree (a very important
  & MUST-KNOW data structure), and when some requirements
  are met, say one tree's point number is smaller than 1,000,
  this tree does NOT have to split into two.
  
  Or, when the Entropy (熵, as you have heard of in Math, I guess
  it's the probability course.) is less than 0.5, the tree does
  not have to split into two.
  
  Otherwise, it has to split. And  the CORE function in randForest.cpp
  ----int SplitCurrentNode()----
  does this job.
  
  20130426 XY511 10:25
  AbrahamX @ NWPU
 */

/*
  What disgusted me was that YOUR team members do not
  follow those good naming or definition tranditions 
  in the Programming field, so it seems weird the
  first few times I read the codes. Though your algorithms
  in AI may have special namings (VERY common among the science
  field......Have to admit. 就是，各个学科对同一事物的命名不一样，
  体现在乱七八糟的术语上面..........)
 */
#ifndef _RANDFOREST_H_
#define _RANDFOREST_H_

//..
#include "randFeature.h"

using namespace cv;


/* Some macros are defined below. */
#define LUT_BIN_NUM    SKE_JOINT_NUM
#define EPS            1e-8

/* 
  Three distinct & INDEPENDENT requirements,
  (Level, Point number & Entropy threshold)either one of 
  them is met, the TREE will split into two.
 */
#define TREE_MAX_LEVEL 8		    //max level of tree

#define TREE_MIN_POINT_NUM   1000   //the min number of point for each node,
                                    //if the point number less than it, the node 
									//will not split

#define TREE_ENTROPY_THRESH  0.5    //if the entropy is less than this value, the
									//node will not split

/*
  Node structure for the random tree
*/

/*
  Suggestion: POWER-OFF your laptop and DRAW a picture of this structure
  pslNode. This is Extremely important for you to FULLY understand
  the structure being used, in this program.
 */
typedef struct pslNode{

	int nodeID;
	int feaIdx;  /* Idx means Index */
	pslFeature fea;
	float LUT[LUT_BIN_NUM];	    //the probability distribution
	float entropy;				//the entropy of the LUT
	int   pointNum;				//the number of pixels in this subtree
	int   level;				//the level of current node
	int  isLeaf;				//flag to show if the node is a leaf node
	int   pointLink;			//the link to the points list of this subtree
	                            //is valid only for the leaf node

	pslNode* Child0;  /* They should have names like left/right child */
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
	}  /* Destructor (or the Initialization
	    ), I spuppose. For we don't have C++ course*/
} pslNode;

typedef vector<pslNode*> pslNodeList;


/* Reference, to be used in this program */
//....
extern pslFeatureList  *featureList;
extern pslKeypointList *keypointList;
extern bitsMat   *feaMat;  
extern pslNode *gbTree;
//....

/* They are for you to accomplish, maybe. */

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
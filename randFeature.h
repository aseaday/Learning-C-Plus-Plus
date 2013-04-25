#ifndef _RANDFEATURE_H_
#define _RANDFEATURE_H_

//..
#include "..\SkeletonProcessor\skeletonData.hpp"
#include "..\sdk\commonLib.h"
//..

#define MAX_FEA_NUM    2000//12000		    //number of features for each points
#define ALPHA          280000

/*
  Structure for point feature compare
*/
typedef struct pslFeature{
    int xPos;				//the x coord of the pixel
	int yPos;				//the y coord of the pixel
	int idx;                //idx = y*width+x

	pslFeature(){xPos=yPos=idx=0;};
}pslFeature;
typedef vector<pslFeature*> pslFeatureList;

/*
  Structure for key points (labeled joint points)
*/
typedef struct pslKeypoint{

	int   label;			//the ground truth of current image
	char imageName[128];	//the name of current image
	int link;
	pslKeypoint(){link = -1;};
} pslKeypoint;
typedef vector<pslKeypoint*> pslKeypointList;

/* STEP: 0
   Generate feature list

   Output: feature list: the number of feature (size of the feature list) is M = 784
*/
void generateFeatureList();

/* STEP: 1
   Generate sample/keypoint list

   Output: keypoint(sample) list: the number of sample (size of the keypoint list) is N = 60000
*/
void generateKeypointList();

/* STEP: 2
   Do feature extraction
   Input: 1) feature list: the number of feature (size of the feature list) is M = 784
          2) keypoint(sample) list: the number of sample (size of the keypoint list) is N = 60000

   Output: the M x N matrix, each entry of the matrix is 1 or 0, using bitsMat structure to save memory requirement
*/
void doFeatureExtraction();

#endif
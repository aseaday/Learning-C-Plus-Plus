#include "randForest.h"

//..
pslFeatureList*  featureList = 0;		// the feature list 
pslKeypointList* keypointList= 0;       // the keypoint list
bitsMat*   feaMat = 0;                  // the size should be featureList.size() x keypointList.size()
                                        // the space should be allocated in doFeatureExtraction() function

/* STEP: 0
   Generate feature list

   Output: feature list: the number of feature (size of the feature list) is M = 784
*/
void generateFeatureList(){

};

/* STEP: 1
   Generate sample/keypoint list

   Output: keypoint(sample) list: the number of sample (size of the keypoint list) is N = 60000
*/
void generateKeypointList(){
	
};

/* STEP: 2
   Do feature extraction
   Input: 1) feature list: the number of feature (size of the feature list) is M = 784
          2) keypoint(sample) list: the number of sample (size of the keypoint list) is N = 60000

   Output: the M x N matrix, each entry of the matrix is 1 or 0, using bitsMat structure to save memory requirement
*/
void doFeatureExtraction(){
	
};
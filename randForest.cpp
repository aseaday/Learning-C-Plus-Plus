/*
  This is a big boy~ Aha do not worry, if you would
  like to be a programmer (software engineer), tis just 
  the beginning.
 */

#include "randForest.h"




//..
pslNodeList* nodeList = 0;       // the node list of the tree
//..
pslNode *gbTree = 0;

//extern unsigned char* feaMat;


/*
  You should visit sites like http://github.com more 
  often to figure out how those great hackers write codes.
  When dealing with pointers, the if (!XXX) is never a good 
  practice. Some bugs may occur once your program goes bigger.
 */
pslNode* allocateNode(){

	if(!nodeList)
		nodeList = new pslNodeList;

	pslNode* node = new pslNode;
	node->nodeID = (int)nodeList->size();
	nodeList->push_back(node);
	return (*nodeList)[nodeList->size()-1];
	/*
	  To better understand those codes, read some sample
	  codes on LinkList & Tree. (in Data Structure books)
	 */
};
float getEntropyLUT(float* LUT, int len){
	float entropy = 0.f;

	for(int i = 0; i < len; i++){
		entropy += (float) (LUT[i]*log(LUT[i]+EPS)/log(2.0));
	}  /* Just the implementation of pre-defined get-entropy algo. */

	return -entropy;
};

float getEntropyLUT(float* LUT, int len, float sum){
	float entropy = 0.f;

	for(int i = 0; i < len; i++){
		LUT[i] = LUT[i]/sum;
		entropy += (float)(LUT[i]*log(LUT[i]+EPS)/log(2.0));
	}

	return -entropy;
};
/*
*/
int getNodeEntropy(pslNode* node){

	//..create the histogram first
	node->pointNum = 0;
	memset(node->LUT, 0, LUT_BIN_NUM*sizeof(float));
	int pointIdx = node->pointLink;
	while (pointIdx >= 0){  
		int label = (*keypointList)[pointIdx]->label;
		node->LUT[label]++;
		node->pointNum++;
		/* After the following line of code, pointIdx should be (-1).
           But there is no sign of it......
		   Ask your advisor to CONFIRM this.
		 */
		 /* Definition in the initRandomTree, */
		 /* Clue: (*keypointList)[noOfPoint-1]->link = -1; */
		pointIdx = (*keypointList)[pointIdx]->link;  /* Then pointIdx == -1 */
	}
    //..
#if 0
	float entropy = 0.f;

	for(int i = 0; i < HIST_BIN_NUM; i++){
		//normalized first
		node->LUT[i] = node->LUT[i]/node->pointNum;

		entropy += (node->LUT[i])*log((node->LUT[i]+EPS))/log(2.0);
	}
#endif

	node->entropy = getEntropyLUT(node->LUT, LUT_BIN_NUM, (float)node->pointNum);

	return 1;
};


int initRandomTree(){

	//..init the point list first
	/* noOfPoint ----> Number of Points..... */
	/* Should have a more clearer description!!! */
	int noOfPoint = (int)keypointList->size();  /* Get the number */
    for(int i = 0; i <noOfPoint-1; i++){
		(*keypointList)[i]->link = i+1;
	};
	(*keypointList)[noOfPoint-1]->link = -1;

	//..init the tree Ye, Initialization.
	gbTree = allocateNode();
	gbTree->level    = 0;
	gbTree->pointNum = (int)keypointList->size();
	gbTree->pointLink= 0;
	getNodeEntropy(gbTree);

	return 1;
};

/*
  Core function of these files. The princile is easy to understand,
  you might as well draw some graphs BY-HAND to help
  figure the process out.
 */

int splitCurrentNode(pslNode* node){

    /* Display which node to be split */
	printf("split node:%05d\r", node->nodeID);
    
	/* Get the number of Feature, why would he use no == num */
	int noOfFeature = (int)featureList->size();

	//..Record the best feature for split
	/* Record and later write these data into the new node */
	float maxEntropyGain = -100000.f;
	int   maxIdx;
	float maxLUT0[LUT_BIN_NUM]; 
	float maxLut0Cnt;
	float maxLUT1[LUT_BIN_NUM]; 
	float maxLut1Cnt;
	float maxEntropy0;
	float maxEntropy1;

	//..find the best feature for split
	/* When dealing with Data Structure like this,
       MATLAB would be a much better choice.
	 */
	for(int i = 0; i < noOfFeature; i++){
	    /* Initialization */
		float LUT0[LUT_BIN_NUM]; 
		float lut0Cnt = 0;
        float LUT1[LUT_BIN_NUM]; 
		float lut1Cnt = 0;
		memset(LUT0, 0, LUT_BIN_NUM*sizeof(float));
		memset(LUT1, 0, LUT_BIN_NUM*sizeof(float));
        
		/* Beginning of the Split process */
		int pointIdx = node->pointLink;
		while(pointIdx >= 0){
		/* Save those data, load later */
			int featureIdx = i;

			//unsigned char feaVal = feaMat[pointIdx*noOfFeature+featureIdx];
			unsigned char feaVal = feaMat->get(pointIdx, featureIdx);

			int label = (*keypointList)[pointIdx]->label;

			if(feaVal ==1 ){
				LUT1[label]++;lut1Cnt++;
			}else{
				LUT0[label]++;lut0Cnt++;
			}
			pointIdx = (*keypointList)[pointIdx]->link;
		}

		float entropy0 = getEntropyLUT(LUT0, LUT_BIN_NUM, lut0Cnt);
		float entropy1 = getEntropyLUT(LUT1, LUT_BIN_NUM, lut1Cnt);
		float allCnt   = (float)node->pointNum;
		float entropyGain = node->entropy - ((lut0Cnt/allCnt)*entropy0 + (lut1Cnt/allCnt)*entropy1);
        
		/* Compare the requirements (or rather standards) to see */
		/* whether to split or not                               */
		if(entropyGain > maxEntropyGain){
			maxEntropyGain = entropyGain;
			maxIdx = i;
			maxLut0Cnt = lut0Cnt;
			maxLut1Cnt = lut1Cnt;
			memcpy(maxLUT0, LUT0, sizeof(float)*LUT_BIN_NUM);
			memcpy(maxLUT1, LUT1, sizeof(float)*LUT_BIN_NUM);
			maxEntropy0 = entropy0;
			maxEntropy1 = entropy1;
		}
	}

	//..if no entropygain, do not split
	if(maxEntropyGain <= 0) return 0;

	//..get the points for two childs
	//int* lut0p = new int[(int)maxLut0Cnt]; int lut0idx = 0;
	int* lut0p = (int*)malloc(((int)maxLut0Cnt)*sizeof(int)); 
	int lut0idx = 0;
	//int* lut1p = new int[(int)maxLut1Cnt]; int lut1idx = 0;
	int* lut1p = (int*)malloc(((int)maxLut1Cnt)*sizeof(int)); 
	int lut1idx = 0;

	int pointIdx = node->pointLink;
	int featureIdx = maxIdx;
	while(pointIdx >= 0){

		//unsigned char feaVal = feaMat[pointIdx*noOfFeature+featureIdx];
		unsigned char feaVal = feaMat->get(pointIdx,featureIdx);
		int label = (*keypointList)[pointIdx]->label;

		if(feaVal ==0 ){
			lut0p[lut0idx] = pointIdx;
			lut0idx++;
		}else{
			lut1p[lut1idx] = pointIdx;
			lut1idx++;
		}
		pointIdx = (*keypointList)[pointIdx]->link;
	}

	//..split current node into 2 node
	//..update the parent node
	pslNode* child0 = allocateNode();	//for 0 branch
	pslNode* child1 = allocateNode();	//for 1 branch
	
	node->feaIdx = maxIdx;
	node->fea    = *((*featureList)[maxIdx]);

	node->Child0 = child0;
	node->Child1 = child1;
	node->isLeaf = 0;
	node->pointLink = -1;
	
	
	/*
	  My suggestion: Find some other easier/similar codes on
	  TREE (the specific data structure), or http://baike.baidu.com
	  or WiKi it for some more information before Reviewing these
	  following codes.
	 */
	 /* 
	   The above one block and the update child code block are
	   the VIPs of this function. Read 3-5 more times to digest it.
	   It's my advice.
	  */
	//..update the child0
	child0->level = node->level+1;
    memcpy(child0->LUT, maxLUT0, sizeof(float)*LUT_BIN_NUM);
	child0->entropy = maxEntropy0;
	child0->pointNum = (int)maxLut0Cnt;
	child0->pointLink= lut0p[0];
	for(int i = 0; i < (int)maxLut0Cnt-1; i++){
		(*keypointList)[lut0p[i]]->link = lut0p[i+1];
	}
	
	(*keypointList)[lut0p[(int)maxLut0Cnt-1]]->link = -1;

	//..update the child1
	child1->level = node->level+1;
	memcpy(child1->LUT, maxLUT1, sizeof(float)*LUT_BIN_NUM);
	child1->entropy = maxEntropy1;
	child1->pointNum = (int)maxLut1Cnt;
	child1->pointLink= lut1p[0];
	for(int i = 0; i < (int)maxLut1Cnt-1; i++){
		(*keypointList)[lut1p[i]]->link = lut1p[i+1];
	}
	(*keypointList)[lut1p[(int)maxLut1Cnt-1]]->link = -1;


	//delete []lut0p;
	free(lut0p);lut0p = 0;
	//delete []lut1p;
	free(lut1p);lut1p = 0;
	return 1;
};



int splitNode(pslNode* node){
    
	/* Conditions for not splitting. */
	if(!node||node->level >= TREE_MAX_LEVEL||
		node->pointNum <= TREE_MIN_POINT_NUM||
		node->entropy  <= TREE_ENTROPY_THRESH
		){
			return 0;
	}
    
	/* Otherwise splitNode */
	/* 这里面是一个函数的递归调用 */
	/* 递归程序的经典就是：汉诺塔了...... */
	//..split current node
	if(splitCurrentNode(node)){
		//..split the child node
		/* Recursion */
		splitNode(node->Child0);
		splitNode(node->Child1);
	}

	return 1;
};
/* STEP: 4
   Build Random tree
   Note: build the random tree, the root of the tree is: _tree_
*/
int buildRandomTree(){
    /* 
	  gbTree seems to be the raw material, and 
	  calling function splitNode() could generate a 
	  random tree. (This is your algorithm, I've no clue.)
	 */
	splitNode(gbTree);
	return 1;
};

/* This is some clean & clear with memory */
/* STEP: 5
   Clear the random tree
*/
int releaseRandomTree(){
	
	//..release feature list
	if(featureList){
		for(int i = 0; i < featureList->size(); i++){
			delete (*featureList)[i];
			(*featureList)[i]=0;
		}
		featureList->clear();
		delete featureList;featureList=0;
	}
	//..release point list
	if(keypointList){
		for(int i = 0; i < keypointList->size(); i++){
			delete (*keypointList)[i];
			(*keypointList)[i]=0;
		}
		keypointList->clear();
		delete keypointList;keypointList=0;
	}

	//..release point list
	if(nodeList){
		for(int i = 0; i < nodeList->size(); i++){
			delete (*nodeList)[i];
			(*nodeList)[i]=0;
		}
		nodeList->clear();
		delete nodeList;nodeList=0;
	}

	gbTree = 0;
	
	if(feaMat){
		delete feaMat;feaMat=0;
	}

	return 1;
};


int dummyTest(){

	return 1;
};


/* File operations. To save/load the data computed. */
bool saveFeatureList(pslFeatureList* feaList ,char* filename){
	FILE* fw = fopen(filename, "wb");

	int noOfFea = (int)feaList->size();
	fwrite(&noOfFea, sizeof(int), 1, fw);

	for(int i = 0; i < feaList->size(); i++){
		fwrite((*feaList)[i], sizeof(pslFeature), 1, fw);
	}
	fclose(fw);

	return true;
};
pslFeatureList* loadFeatureList(char* filename){

	pslFeatureList* feaList = new pslFeatureList;
	int noOfFea;

	FILE* fr = fopen(filename, "rb");
	fread(&noOfFea, sizeof(int), 1, fr);

	for(int i = 0; i < noOfFea; i++){
		pslFeature* fea = new pslFeature;
		fread(fea,sizeof(pslFeature), 1,fr);
		feaList->push_back(fea);
	}
	fclose(fr);
	return feaList;
};

bool saveKeypointList(pslKeypointList* kpList ,char* filename){
	FILE* fw = fopen(filename, "wb");

	int noOfKp = (int)kpList->size();
	fwrite(&noOfKp, sizeof(int), 1, fw);

	for(int i = 0; i < kpList->size(); i++){
		fwrite((*kpList)[i], sizeof(pslKeypoint), 1, fw);
	}
	fclose(fw);

	return true;
};
pslKeypointList* loadKeypointList(char* filename){

	pslKeypointList* kpList = new pslKeypointList;
	int noOfKp;

	FILE* fr = fopen(filename, "rb");
	fread(&noOfKp, sizeof(int), 1, fr);

	for(int i = 0; i < noOfKp; i++){
		pslKeypoint* kp = new pslKeypoint;
		fread(kp,sizeof(pslKeypoint), 1,fr);
		kpList->push_back(kp);
	}
	fclose(fr);
	return kpList;
};
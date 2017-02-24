#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <stack>
#include "../../../../yengine/scene/CCamera.h"
#include "HeightMap.h"
#include <algorithm>

class QuadTreeNode 
{
public :

	int mapSize;
	int currentRange;
	int posX;
	int posY;
	int lodLevel;
	bool fullResolution;
	float maxHeight;
	float minHeight;

private :

	QuadTreeNode *mTopLeft;
	QuadTreeNode *mTopRight;
	QuadTreeNode *mBottomLeft;
	QuadTreeNode *mBottomRight;

public:
		
	QuadTreeNode(HeightMap *heightMap, int size, int lodDepth, int x, int y);

	~QuadTreeNode();

	bool inSphere(float radius, glm::vec3 position);

	bool inFrustum(CCamera *cam);

	bool LODSelect(std::vector<int> &ranges, int lodlevel, CCamera *cam, std::stack<QuadTreeNode*> &drawStack);

};

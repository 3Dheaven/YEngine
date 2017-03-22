#include "QuadTreeNode.h"

QuadTreeNode::QuadTreeNode(HeightMap *heightMap, int size, int lodDepth, int x, int y) 
{

	posX = x;
	posY = y;
	mapSize = size;
	lodLevel = lodDepth;

	if (lodDepth == 0) 
	{

		mTopLeft = NULL;
		mTopRight = NULL;
		mBottomLeft = NULL;
		mBottomRight = NULL;

		maxHeight = heightMap->getMaxValueInArea(x, y, size, size);
		minHeight = heightMap->getMinValueInArea(x, y, size, size);

	}
	else
	{

		int halfNodeSize = size / 2;

		mTopLeft = new QuadTreeNode(heightMap, halfNodeSize, lodDepth - 1, x, y);
		mTopRight = new QuadTreeNode(heightMap, halfNodeSize, lodDepth - 1, x + halfNodeSize, y);
		mBottomLeft = new QuadTreeNode(heightMap, halfNodeSize, lodDepth - 1, x, y + halfNodeSize);
		mBottomRight = new QuadTreeNode(heightMap, halfNodeSize, lodDepth - 1, x + halfNodeSize, y + halfNodeSize);

		maxHeight = std::max(std::max(mTopLeft->maxHeight, mTopRight->maxHeight),
			std::max(mBottomLeft->maxHeight, mBottomRight->maxHeight));

		minHeight = std::min(std::min(mTopLeft->minHeight, mTopRight->minHeight),
			std::min(mBottomLeft->minHeight, mBottomRight->minHeight));
	}

}

QuadTreeNode::~QuadTreeNode() 
{

	if (mTopLeft != NULL) delete mTopLeft;
	if (mTopRight != NULL) delete mTopRight;
	if (mBottomLeft != NULL) delete mBottomLeft;
	if (mBottomRight != NULL) delete mBottomRight;

}

bool 
QuadTreeNode::inSphere(float radius, glm::vec3 position) 
{

	float r2 = radius*radius;
	glm::vec3 c1 = glm::vec3(posX, 0, posY);
	glm::vec3 c2 = glm::vec3(posX + mapSize, 0, posY + mapSize);
	glm::vec3 distV;

	if (position.x < c1.x) distV.x = (position.x - c1.x);
	else if (position.x > c2.x) distV.x = (position.x - c2.x);
	if (position.y < c1.y) distV.y = (position.y - c1.y);
	else if (position.y > c2.y) distV.y = (position.y - c2.y);
	if (position.z < c1.z) distV.z = (position.z - c1.z);
	else if (position.z > c2.z) distV.z = (position.z - c2.z);

	float dist2 = glm::dot(distV, distV);

	return dist2 <= r2;
}

bool 
QuadTreeNode::inFrustum(CCamera *cam) 
{

	glm::vec3 min_v = glm::vec3(posX, 0, posY);
	glm::vec3 max_v = glm::vec3(posX + mapSize, 0, posY + mapSize);

	return cam->AABBInsideFrustum(max_v, min_v);
}

bool 
QuadTreeNode::LODSelect(std::vector<int> &ranges, int lodlevel, std::shared_ptr<CCamera> cam, std::stack<QuadTreeNode*> &drawStack)
{

	currentRange = ranges[lodlevel];

	// Test if camera's sphere intersects the current node.
	// If not : avoid to render current node.
	glm::vec3 fpos = glm::vec3(0.0, 0.0, 0.0);//glm::vec3(96.0,0.0,64.0);
	if (!inSphere(ranges[lodlevel], cam->mPosition)) 
	{
		return false;
	}

	// Frustrum culling
	/*if (!inFrustum(cam)) {
	return true;
	}*/

	if (lodlevel == 0) 
	{
		fullResolution = true;
		drawStack.push(this);
		return true;

	}
	else 
	{

		if (!inSphere(ranges[lodlevel - 1], cam->mPosition)) 
		{
			fullResolution = true;
			drawStack.push(this);
		}
		else {

			QuadTreeNode *child;

			child = mTopLeft;
			if (!child->LODSelect(ranges, lodlevel - 1, cam, drawStack)) 
			{
				child->fullResolution = false;
				child->currentRange = currentRange;
				drawStack.push(child);
			}

			child = mTopRight;
			if (!child->LODSelect(ranges, lodlevel - 1, cam, drawStack)) 
			{
				child->fullResolution = false;
				child->currentRange = currentRange;
				drawStack.push(child);
			}

			child = mBottomLeft;
			if (!child->LODSelect(ranges, lodlevel - 1, cam, drawStack)) 
			{
				child->fullResolution = false;
				child->currentRange = currentRange;
				drawStack.push(child);
			}

			child = mBottomRight;
			if (!child->LODSelect(ranges, lodlevel - 1, cam, drawStack)) 
			{
				child->fullResolution = false;
				child->currentRange = currentRange;
				drawStack.push(child);
			}


		}

		return true;

	}
}
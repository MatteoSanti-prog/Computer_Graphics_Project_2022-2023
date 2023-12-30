class Collider {
private:
	float right;
	float left;
	float bottom;
	float top;
	float height;
public:
	Collider() {}

	Collider(float right, float left, float top, float bottom, float height, float scalingFactor) {
		this->right = scalingFactor * right;
		this->left = scalingFactor * left;
		this->top = scalingFactor * top;
		this->bottom = scalingFactor * bottom;
		this->height = scalingFactor * height;
	}

	bool isInside(glm::vec3 pos) {
		if (pos.x < right && pos.x > left && pos.z < top && pos.z > bottom && pos.y < height)
			return true;

		return false;
	}
};

bool validPosition(glm::vec3 pos, float scalingFactor) {
	float border = 15.0f;

	Collider mapLimits { border, -border, border, -border, 10.0f, scalingFactor };
	Collider sectionGFHE { 7.0f, 5.0f, 7.0f, -1.0f, 1.5f, scalingFactor };
	Collider sectionRJQG { 4.0f, -5.0f, 0, -2.0f, 1.5f, scalingFactor };
	Collider sectionOPKN { -5.0f, -8.0f, 3.0f, -7.0f, 1.5f, scalingFactor };
    //Collider Test {0.5f, -0.5f, 3.0f, 2.0f, 1.0f, 3.0f};
	if (sectionGFHE.isInside(pos))
		return false;
	if (sectionRJQG.isInside(pos))
		return false;
	if (sectionOPKN.isInside(pos))
		return false;
	if (mapLimits.isInside(pos))
		return true;
	return false;
}

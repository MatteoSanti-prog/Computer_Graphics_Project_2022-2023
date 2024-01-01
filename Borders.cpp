#include "Borders.hpp"
#include "Constants.hpp"

Collider::Collider() {}

Collider::Collider(float right, float left, float top, float bottom, float height) {
    this->right = scalingFactor * right;
    this->left = scalingFactor * left;
    this->top = scalingFactor * top;
    this->bottom = scalingFactor * bottom;
    this->height = scalingFactor * height;
}

bool Collider::isInside(glm::vec3 pos) {
    if (pos.x < right && pos.x > left && pos.z < top && pos.z > bottom && pos.y < height)
        return true;

    return false;
}

bool validPosition(glm::vec3 pos) {
	float border = 15.0f;

    Collider mapLimits { border, -border, border, -border, border};
    Collider sectionGFHE { 7.0f, 5.0f, 7.0f, -1.0f, 2.0f};
    Collider sectionRJQG { 4.0f, -4.0f, 0, -2.0f, 2.0f};
    Collider sectionOPKN { -5.0f, -8.0f, 3.0f, -7.0f, 2.0f};
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

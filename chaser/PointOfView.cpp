#include "PointOfView.h"

#include <stdio.h>

Matrix4f PointOfView::getProjectionMatrix(Matrix4f *projMatrix)
{
	Matrix4f m;

	m = Matrix4f::frustumProjectionMatrix(-0.025, -0.1, 0.025, 0.1, 0.01, 50);
	if (projMatrix != NULL) *projMatrix = m;
	
	//m = m * Matrix4f::symmetricPerspectiveProjectionMatrix(fieldOfView, aspectRatio, nearPlane, farPlane);

	return(m);
	//return Matrix4f::identity();
}
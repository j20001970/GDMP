#include "vision_task.h"

ImageProcessingOptions make_image_processing_options(Rect2 roi, int rotation_degrees) {
	ImageProcessingOptions image_processing_options;
	Vector2 roi_start = roi.get_position();
	Vector2 roi_end = roi.get_end();
	image_processing_options.region_of_interest->left = roi_start.x;
	image_processing_options.region_of_interest->top = roi_start.y;
	image_processing_options.region_of_interest->right = roi_end.x;
	image_processing_options.region_of_interest->bottom = roi_end.y;
	image_processing_options.rotation_degrees = rotation_degrees;
	return image_processing_options;
}

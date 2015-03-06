#include "opencv2/opencv.hpp"
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

#define KERNEL 5


void morph(Mat localFrameMask, Mat localFrameKernel)
{
	morphologyEx(localFrameMask, localFrameMask, MORPH_DILATE, localFrameKernel, Point(-1, -1), 2, BORDER_CONSTANT, morphologyDefaultBorderValue());
	morphologyEx(localFrameMask, localFrameMask, MORPH_DILATE, localFrameKernel, Point(-1, -1), 2, BORDER_CONSTANT, morphologyDefaultBorderValue());
	morphologyEx(localFrameMask, localFrameMask, MORPH_DILATE, localFrameKernel, Point(-1, -1), 2, BORDER_CONSTANT, morphologyDefaultBorderValue());
	morphologyEx(localFrameMask, localFrameMask, MORPH_DILATE, localFrameKernel, Point(-1, -1), 2, BORDER_CONSTANT, morphologyDefaultBorderValue());

	morphologyEx(localFrameMask, localFrameMask, MORPH_ERODE, localFrameKernel, Point(-1, -1), 2, BORDER_CONSTANT, morphologyDefaultBorderValue());
	morphologyEx(localFrameMask, localFrameMask, MORPH_CLOSE, localFrameKernel, Point(-1, -1), 2, BORDER_CONSTANT, morphologyDefaultBorderValue());
}



int main(int argc, char** argv)
{
	VideoCapture originalVideo(argv[1]);

	if (!originalVideo.isOpened())
	{
		cout << "Video not loaded correctly!" << endl;
		return -20;
	}

	double FPS = originalVideo.get(CV_CAP_PROP_FPS);
	int originalVideoDurationInFrames = (int)originalVideo.get(CV_CAP_PROP_FRAME_COUNT);
	int frameWidth = (int)originalVideo.get(CV_CAP_PROP_FRAME_WIDTH);
	int frameHeight = (int)originalVideo.get(CV_CAP_PROP_FRAME_HEIGHT);

	Mat currentFrame;
	Mat previousFrame;
	Mat adjacentFramesDifference;
	Mat frameMask; 
	Mat morphologicalFrame;
	Mat finalFrame;
	Mat frameKernel = getStructuringElement(MORPH_RECT, Size(KERNEL, KERNEL), Point(-1, -1));

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	if (!originalVideo.read(previousFrame))
	{
		cout << "Video not read correctly!" << endl;
		return -10;
	}
	
	namedWindow("Morphological Operations", WINDOW_NORMAL);
	namedWindow("End Video", WINDOW_NORMAL);	

	VideoWriter morphVideo("Morphological_Operations_Video.avi", CV_FOURCC('X', 'V', 'I', 'D'), FPS, Size(frameWidth, frameHeight));
	VideoWriter endVideo("End_Video.avi", CV_FOURCC('X', 'V', 'I', 'D'), FPS, Size(frameWidth, frameHeight));

//	Mat frameKernel = getStructuringElement(MORPH_RECT, Size(KERNEL, KERNEL), Point(-1, -1));	

//	vector<vector<Point>> contours;
//	vector<Vec4i> hierarchy;

	for (int i = 0; i < originalVideoDurationInFrames; i++)
	{
		if (!originalVideo.read(currentFrame))
		{
			break;
		}

		// Difference between frame & binary thresholding
		absdiff(currentFrame, previousFrame, adjacentFramesDifference);
		cvtColor(adjacentFramesDifference, adjacentFramesDifference, CV_RGB2GRAY);
		threshold(adjacentFramesDifference, frameMask, 25, 255, THRESH_BINARY);

		// Morphological operations
		frameMask.copyTo(morphologicalFrame);
		morph(frameMask, frameKernel);		
				
		// Finding contours
		findContours(frameMask, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

		// Approximate contours to polygons + Finding bounding rectangles for all contours
		vector<vector<Point>> contoursOfPolygons(contours.size());
		vector<Rect> localBoundingRectangle(contours.size());

		currentFrame.copyTo(finalFrame);

		for (int i = 0; i < contours.size(); i++)
		{
			approxPolyDP(Mat(contours[i]), contoursOfPolygons[i], 3, true);
			localBoundingRectangle[i] = boundingRect(Mat(contoursOfPolygons[i]));

			// Drawing the bounding boxes around contours that satisfy the threshold
			if (contourArea(contours[i]) >= 900)
			{
				rectangle(finalFrame, localBoundingRectangle[i].tl(), localBoundingRectangle[i].br(), Scalar(0, 0, 255), 2, 8, 0);
			}
		}
	
		imshow("Morphological Operations", morphologicalFrame);
		imshow("End Video", finalFrame);

		//converting back from grayscale to RGB and saving the videos
		cvtColor(morphologicalFrame, morphologicalFrame, CV_GRAY2BGR);
		morphVideo << morphologicalFrame;
		endVideo << finalFrame;

		waitKey(5);
		
		currentFrame.copyTo(previousFrame);
	}
	return 0;
}
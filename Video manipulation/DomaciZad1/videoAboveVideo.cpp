#include "videoAboveVideo.h"

int videoAboveVideo(VideoCapture inputVideo1, VideoCapture inputVideo2, int inputDuration1, int inputDuration2, double outputFPS, char* name)
{
	Mat inputFrame1, inputFrame2;

	inputVideo1.set(CV_CAP_PROP_POS_FRAMES, 0);
	inputVideo2.set(CV_CAP_PROP_POS_FRAMES, 0);

	if (!inputVideo1.read(inputFrame1))
	{
		cout << "Greska pri ucitavanju videa #1" << endl;
		return -1;
	}

	if (!inputVideo2.read(inputFrame2))
	{
		cout << "Greska pri ucitavanju videa #2" << endl;
		return -1;
	}

	Size outputSize = Size(max(inputFrame1.cols, inputFrame2.cols), (inputFrame1.rows + inputFrame2.rows));
	Mat outputFrame(outputSize, inputFrame1.type());
	outputFrame.setTo(0);

	VideoWriter outputVid(name, CV_FOURCC('X', 'V', 'I', 'D'), outputFPS, outputSize);
	if (!outputVid.isOpened())
	{
		cout << "Video fajl u kome je video iznad videa nije kreiran" << endl;
		return -1;
	}
	
	inputVideo1.set(CV_CAP_PROP_POS_FRAMES, 0);
	inputVideo2.set(CV_CAP_PROP_POS_FRAMES, 0);

	namedWindow("videoAboveVideo", WINDOW_NORMAL);

	int minDuration = min(inputDuration1, inputDuration2);
	for (int i = 0; i < minDuration; i++)
	{
		if (!inputVideo1.read(inputFrame1))
		{
			break;
		}

		if (!inputVideo2.read(inputFrame2))
		{
			inputVideo1.set(CV_CAP_PROP_POS_FRAMES, inputVideo1.get(CV_CAP_PROP_POS_FRAMES) - 1);
			break;
		}

		inputFrame1.copyTo(outputFrame(Rect((outputFrame.cols - inputFrame1.cols)/2, 0, inputFrame1.cols, inputFrame1.rows)));
		inputFrame2.copyTo(outputFrame(Rect((outputFrame.cols - inputFrame2.cols)/2, inputFrame1.rows, inputFrame2.cols, inputFrame2.rows)));
		outputVid << outputFrame;

		imshow("videoAboveVideo", outputFrame);
		waitKey(30);
	}

	int absoluteValue = abs(inputDuration1 - inputDuration2);
	for (int i = 0; i < absoluteValue; i++)
	{
		if (inputDuration1 > inputDuration2)
		{
			if (!inputVideo1.read(inputFrame1))
				break;

			inputFrame1.copyTo(outputFrame(Rect((outputFrame.cols - inputFrame1.cols)/2, 0, inputFrame1.cols, inputFrame1.rows)));
			outputVid << outputFrame;

			imshow("videoAboveVideo", outputFrame);
			waitKey(30);
		}

		else
		{
			if (!inputVideo2.read(inputFrame2))
				break;

			inputFrame2.copyTo(outputFrame(Rect(0, inputFrame1.rows, inputFrame2.cols, inputFrame2.rows)));
			outputVid << outputFrame;

			imshow("videoAboveVideo", outputFrame);
			waitKey(30);
		}
	}

	destroyAllWindows();
	return 1;
}
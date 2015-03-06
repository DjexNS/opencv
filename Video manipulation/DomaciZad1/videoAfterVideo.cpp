#include "videoAfterVideo.h"

int videoAfterVideo(VideoCapture inputVideo1, VideoCapture inputVideo2, int inputDuration1, int inputDuration2, double outputFPS, char* name)
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

	Size outputSize = Size(max(inputFrame1.cols, inputFrame2.cols), max(inputFrame1.rows, inputFrame2.rows));
	Mat outputFrame(outputSize, inputFrame1.type());
	outputFrame.setTo(0);

	VideoWriter outputVid2(name, CV_FOURCC('X', 'V', 'I', 'D'), outputFPS, outputSize);
	if (!outputVid2.isOpened())
	{
		cout << "Video fajl u kome je video posle videa nije kreiran" << endl;
		return -1;
	}

	inputVideo1.set(CV_CAP_PROP_POS_FRAMES, 0);
	inputVideo2.set(CV_CAP_PROP_POS_FRAMES, 0);

	namedWindow("videoAfterVideo", WINDOW_NORMAL);

	for (int i = 0; i < inputDuration1; i++)
	{
		if (!inputVideo1.read(inputFrame1))
			break;

		inputFrame1.copyTo(outputFrame(Rect((outputFrame.cols - inputFrame1.cols) / 2, (outputFrame.rows - inputFrame1.rows) / 2, inputFrame1.cols, inputFrame1.rows)));
		outputVid2 << outputFrame;

		imshow("videoAfterVideo", outputFrame);
		waitKey(30);
	}

	outputFrame.setTo(0);
	for (int i = 0; i < inputDuration2; i++)
	{
		if (!inputVideo2.read(inputFrame2))
			break;

		inputFrame2.copyTo(outputFrame(Rect((outputFrame.cols - inputFrame2.cols) / 2, (outputFrame.rows - inputFrame2.rows) / 2, inputFrame2.cols, inputFrame2.rows)));
		outputVid2 << outputFrame;

		imshow("videoAfterVideo", outputFrame);
		waitKey(30);
	}

	destroyAllWindows();
	return 1;
}
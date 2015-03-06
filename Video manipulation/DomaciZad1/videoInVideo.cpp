#include "videoInVideo.h"

int videoInVideo(VideoCapture inputVideo1, VideoCapture inputVideo2, int inputDuration1, int inputDuration2, double outputFPS, char* name)
{
	Mat inputFrame1, inputFrame2, temp;

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

	Size outputSize = Size(inputFrame1.cols, inputFrame1.rows);
	Mat outputFrame(outputSize, inputFrame1.type());
	outputFrame.setTo(0);

	VideoWriter outputVid(name, CV_FOURCC('X', 'V', 'I', 'D'), outputFPS, outputSize);
	if (!outputVid.isOpened())
	{
		cout << "Video fajl u kome je video integrisan u video nije kreiran" << endl;
		return -1;
	}

	inputVideo1.set(CV_CAP_PROP_POS_FRAMES, 0);
	inputVideo2.set(CV_CAP_PROP_POS_FRAMES, 0);

	namedWindow("videoInVideo", WINDOW_NORMAL);

	int minInputDuration = min(inputDuration1, inputDuration2);
	for (int i = 0; i < minInputDuration; i++)
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

		inputFrame1.copyTo(outputFrame);
		resize(inputFrame2, temp, Size(inputFrame1.cols/3, inputFrame1.rows/3), 0, 0, INTER_LINEAR);
		temp.copyTo(outputFrame(Rect(outputFrame.cols - inputFrame1.cols/3, outputFrame.rows - inputFrame1.rows/3, temp.cols, temp.rows)));
		outputVid << outputFrame;

		imshow("videoInVideo", outputFrame);
		waitKey(30);
	}

	int absValue = abs(inputDuration1 - inputDuration2);
	for (int i = 0; i < absValue; i++)
	{
		if (inputDuration1 > inputDuration2)
		{
			if (!inputVideo1.read(inputFrame1))
			{
				break;
			}

			inputFrame1.copyTo(outputFrame);
			temp.copyTo(outputFrame(Rect(outputFrame.cols - inputFrame1.cols/3, outputFrame.rows - inputFrame1.rows/3, temp.cols, temp.rows)));
			outputVid << outputFrame;

			imshow("videoInVideo", outputFrame);
			waitKey(30);
		}

		else
		{
			if (!inputVideo2.read(inputFrame2))
			{
				break;
			}

			resize(inputFrame2, temp, Size(inputFrame1.cols / 3, inputFrame1.rows / 3), 0, 0, INTER_LINEAR);
			temp.copyTo(outputFrame(Rect(outputFrame.cols - inputFrame1.cols / 3, outputFrame.rows - inputFrame1.rows / 3, temp.cols, temp.rows)));
			outputVid << outputFrame;

			imshow("videoInVideo", outputFrame);
			waitKey(30);
		}
	}

	destroyAllWindows();
	return 1;
}

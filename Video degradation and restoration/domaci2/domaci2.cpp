#include "Header.h"



int main(int argc, char* argv[])
{
	if (argc != argNumber)
	{
		cout << "5 arguments required!" << endl;
		return -1;
	}

	VideoCapture video(argv[1]);
	double FPS = video.get(CV_CAP_PROP_FPS);  //broj frejmova u sekundi	

	if (!video.isOpened())
	{
		cout << "Error!" << endl;
	}

	/*
	TO DO: Nesto ne radi ovo, moram da vidim kasnije
	----------------------------------------------
	cvGetCaptureProperty(CvCapture* capture,int property_id);

	property_id options are below with definitions :

	CV_CAP_PROP_POS_MSEC 0

		CV_CAP_PROP_POS_FRAME 1

		CV_CAP_PROP_POS_AVI_RATIO 2

		CV_CAP_PROP_FRAME_WIDTH 3

		CV_CAP_PROP_FRAME_HEIGHT 4

		CV_CAP_PROP_FPS 5

		CV_CAP_PROP_FOURCC 6

		CV_CAP_PROP_FRAME_COUNT 7
	*/	

	int width = (int)video.get(CV_CAP_PROP_FRAME_WIDTH);
	int height = (int)video.get(CV_CAP_PROP_FRAME_HEIGHT);

	Mat tempG0, tempG1, tempG2;
	Mat tempL0, tempL1;
	Mat previousResolutionL0, previousResolutionL1;
	Mat resultV;
	Mat tempVideo0, tempVideo1, tempVideo2;
	Mat lShift;
	char pushButton;

	namedWindow("G0", WINDOW_NORMAL);
	namedWindow("Rezultat", WINDOW_NORMAL);	
	
	VideoWriter G2(argv[2], CV_FOURCC('W', 'M', 'V', '1'), FPS, Size(width / 4, height / 4));
	VideoWriter L1(argv[3], CV_FOURCC('W', 'M', 'V', '1'), FPS, Size(width / 2, height / 2));
	VideoWriter L0(argv[4], CV_FOURCC('W', 'M', 'V', '1'), FPS, Size(width, height));
	VideoWriter result(argv[5], CV_FOURCC('W', 'M', 'V', '1'), FPS, Size(width, height));
	
	for (;;)
	{
		while (video.grab())
		{
			video.retrieve(tempG0);
			imshow("G0", tempG0);
			
			/**********************************/
			//prvi nivo gausove piramide - tempG0
			pyrDown(tempG0, tempG1, Size(tempG0.cols / 2, tempG0.rows / 2));
			
			//drugi nivo gausove piramide - temp G1
			pyrDown(tempG1, tempG2, Size(tempG1.cols / 2, tempG1.rows / 2));			
			
			//smestamo drugi nivo gausove piramide u G2
			G2 << tempG2;
			/**********************************/


			/**********************************/
			//upsampling
			pyrUp(tempG2, previousResolutionL1, Size(tempG2.cols * 2, tempG2.rows * 2));
			
			//laplasova piramida (razlika frejmova na toj rezoluciji) - tempL1
			subtract(tempG1, previousResolutionL1, tempL1, noArray(), CV_8SC3);
			tempL1.convertTo(lShift, CV_8UC3, 1, 128);
			L1 << lShift;			
			/**********************************/


			/**********************************/
			//laplasova piramida (razlika frejmova na toj rezoluciji) - tempL0
			pyrUp(tempG1, previousResolutionL0, Size(width, height));
			subtract(tempG0, previousResolutionL0, tempL0, noArray(), CV_8SC3);
			tempL0.convertTo(lShift, CV_8UC3, 1, 128);
			//smestamo laplasovu piramidu u L0
			L0 << lShift;
			/**********************************/
			
			
			/**********************************/
			//ovde vrsimo  videa, frejm po frejm
			pyrUp(tempG2, tempVideo2, Size(tempG2.cols * 2, tempG2.rows * 2));
			add(tempVideo2, tempL1, tempVideo1, noArray(), CV_8UC3);
			pyrUp(tempVideo1, tempVideo0, Size(tempVideo2.cols * 2, tempVideo2.rows * 2));
			add(tempVideo0, tempL0, resultV, noArray(), CV_8UC3);
			/**********************************/

			result << resultV;
			imshow("Rezultat", resultV);

			pushButton = waitKey((int)FPS);

			if (pushButton == 30)
				break;
		}
		break;
	}
}
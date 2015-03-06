#include "Program.h"
#include "setFPS.h"
#include "videoAfterVideo.h"
#include "videoAboveVideo.h"
#include "videoInVideo.h"



int main(int argc, char* argv[])
{

	if (argc != 6)
	{
		cout << "Nedovoljan broj parametara" << endl;
		return -1;
	}
		
	VideoCapture inputVideo1(argv[1]);
	if (!inputVideo1.isOpened())
	{
		cout << "Video 1 nije uspesno ucitan" << endl;
		return -1;
	}
	
	VideoCapture inputVideo2(argv[2]);
	if (!inputVideo2.isOpened())
	{
		cout << "Video 2 nije uspesno ucitan" << endl;
		return -1;
	}
	
	double outputFPS = setFPS(inputVideo1.get(CV_CAP_PROP_FPS), inputVideo2.get(CV_CAP_PROP_FPS));

	int inputDuration1 = (int)min(outputFPS * LIMIT, inputVideo1.get(CV_CAP_PROP_FRAME_COUNT));
	int inputDuration2 = (int)min(outputFPS * LIMIT, inputVideo2.get(CV_CAP_PROP_FRAME_COUNT));

	// postavljanje prvog videa iznad drugog videa
	cout << "videoAboveVideo..." << endl;
	if (videoAboveVideo(inputVideo1, inputVideo2, inputDuration1, inputDuration2, outputFPS, argv[3]) == 1)
	{
		cout << "Uspesno postavljanje videa iznad videa" << endl << endl;
	}
	else
	{
		cout << "Neuspesno postavljanje videa iznad videa" << endl << endl;
	}

	// postavljanje prvog videa nakon drugog videa
	cout << "videoAfterVideo..." << endl;
	if (videoAfterVideo(inputVideo1, inputVideo2, inputDuration1, inputDuration2, outputFPS, argv[4]) == 1)
	{
		cout << "Uspesno postavljanje videa nakon videa" << endl << endl;
	}
	else
	{
		cout << "Neuspesno postavljanje videa nakon videa" << endl << endl;
	}

	// postavljanje drugog videa u prvi video
	cout << "videoInVideo..." << endl;
	if (videoInVideo(inputVideo1, inputVideo2, inputDuration1, inputDuration2, outputFPS, argv[5]) == 1)
	{
		cout << "Uspesno postavljanje videa u video" << endl << endl;
	}
	else
	{
		cout << "Neuspesno postavljanje videa u video" << endl << endl;
	}
	
	return 0;
}

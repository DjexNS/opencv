#include "setFPS.h"

double setFPS(double fps1, double fps2)
{
	if (fps1 != fps2)
	{
		cout << "Framerate videa nije isti - odaberite jedan od fps-a:" << endl << "[1] fps1 = " << fps1 << endl << "[2] fps2 = " << fps2<<endl << "[3] fps3 = proizvoljni" << endl;
		int option;

		while (true)		
		{ 
			cin >> option;
			switch (option)
			{
				case 1:
					return fps1;
				case 2:
					return fps2;
				case 3:
					double unos;
					cout << "Unesite zeljeni fps iz opsega 1 do 100)" << endl;
					cin >> unos;
					while (unos < 1 || unos >= 100)
					{
						cout << "Pogresan unos, pokusajte ponovo" << endl;
						cin >> unos;
					}
					return unos;
				default:
				{
					cout << "Pogresan unos, probajte ponovo" << endl;
					break;
				}
			}
		}
	}
	else
	{
		return fps1;
	}
}
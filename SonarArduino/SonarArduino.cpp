// SonarArduino.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <locale.h>

#include "SerialGate.h"
#include "GUISonarArduino.h"

using namespace std;
using namespace guiSAhH;

//Проверка контрольной суммы
bool checkControlSum(int angle, int d1, int d2,  int control)
{
	int sum = angle + d1 + d2;

	/*т.к. передача идет в типе char,
	максимальнная вместимость бита - 256 значений, а сумма может быть больше,
	обрезается еще перед передачей, проверим это
	*/
	while (sum / 256 >= 1)
		sum -= 256;

	if (sum != control)
		return false;

	return true;
}

//Инициализируем радар
SerialGate port(2, 9600, 40, 5);

int dataPort[4];
int *dataPortPtr = dataPort;

void Update(int)
{
	while (true)
	{
		//Получение данных
		port.GetSeparationData(dataPortPtr, 4);
		int oldAngle = 0;
		
		//Если поступили новые данные
		if (dataPort[0] != oldAngle)
		{
			//Если это не помехи и данные целые (контрольная сумма совпадает)
			if (checkControlSum(dataPort[0], dataPort[1], dataPort[2], dataPort[3]))
			{
				//cout << dataPort[0] << " | " << dataPort[1] << " | " << dataPort[2] << " | "  << dataPort[3] << endl;
				oldAngle = dataPort[0];
				angle = dataPort[0];
				guiSAhH::distance = dataPort[1];

				objectAngleOld[angle] = objectAngle[angle];

				if (angle > angleSonar)
					angle = 0;

				objectAngle[angle] = guiSAhH::distance;

				if (angleSonar == coverage360)
				{
					int angleNew = angle + 180;

					objectAngleOld[angleNew] = objectAngle[angleNew];
					objectAngle[angleNew] = dataPort[2];
				}
				//Обновление экрана
				display();
			}
		}
	}
}

int main(int argc, char * argv[])
{
	setlocale(LC_ALL, "Russian");

	//Открытие порта для работы с радаром
	port.Open(2, 9600);

	//Установка параметров запуска 
		int var = 0;
		cout << "Линии или точки? (1/2)" << endl;
		cin >> var;

		if (var == 1)
			typeSonar = line;
		if (var == 2)
			typeSonar = circle;

		var = 0;
		cout << "Область видимости радара 180* или 360*? (1/2)" << endl;
		cin >> var;

		if (var == 1)
			angleSonar = coverage180;
		if (var == 2)
			angleSonar = coverage360;

	//Инициализация масивов под объекты
		objectAngle = new int[angleSonar];
		objectAngleOld = new int[angleSonar];

	//OpenGL - GUI
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL);

	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("SonarArduino");
	Initialize();

	glutDisplayFunc(display);
	glutReshapeFunc(resize);

	glutTimerFunc(40, Update, 0);

	glutMainLoop();
	port.Close();
	
	//Очищаяем память
		delete[] objectAngle;
		delete[] objectAngleOld;

	return 0;
}
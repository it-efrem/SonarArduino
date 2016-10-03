/*
SerialGate - Билиотека для работы с COM-портами
Замечание - библиотека старая, компилируйте приложение под x86 систему!

Добавил в класс переменные, 2 конструктора и 2 метода, которые определены в этом файле

Если Вы не знаете, что делать с этим файлом, то смотрите урок на YouTube:
Канал - https://www.youtube.com/c/code_robots
*/

#if !defined (SERIALGATEH)
#define SERIALGATEH

#include <windows.h>
#include <string>

#define MAX_WIN_PORT 255

struct Port
{
	unsigned char Id;	
	bool Availbl;
};

struct PortInfo
{
	Port p[MAX_WIN_PORT];
	unsigned char koll;
};


extern "C" class __declspec (dllexport)  SerialGate
{	
	HANDLE m_hFile;
	bool state;

	const int numCom;
	const int rate;
	const int speedMS;
	const int maxBuffer;

	int countReadByte;
	char *buf;
public:
	
	enum IN_LINES_NAME {CTS, DSR, RING, RLSD};
	enum OUT_LINES_NAME {DTR, RTS};	

	SerialGate();

	SerialGate(const int numCom, const int rate) :
		numCom(numCom), rate(rate), speedMS(30), maxBuffer(256), countReadByte(0)
	{
		*buf = new char[maxBuffer];
	}

	SerialGate(const int numCom, const int rate, const int speedMS, const int maxBuffer) :
		numCom(numCom), rate(rate), speedMS(speedMS), maxBuffer(maxBuffer), countReadByte(0)
	{
		*buf = new char[maxBuffer];
	}

	bool Open(int port, int baud);	
	int Send(char* buff, int szBuff);
	int Recv(char* buff, int szBuff);	
	void ReadStream();
	void GetSeparationData(int *arrayValue, int countUnitData);
	void SetLine(OUT_LINES_NAME ln, bool state);
	bool GetLine(IN_LINES_NAME ln);
	void GetPortsInfo(PortInfo* pi);
	void Close();
	void Clean();

	~SerialGate();
};

//Вывод на экран всего содержимого потока
void SerialGate::ReadStream()
{
	if (!this->Open(numCom, rate))
	{
		std::cout << "Error opening of port." << std::endl;
		system("pause");
	}

	char *buf = new char[this->maxBuffer];

	while (true)
	{
		Sleep(speedMS);
		this->countReadByte = this->Recv(buf, sizeof(buf));

		//Output data
		for (int i = 0; i < this->countReadByte; i++)
			std::cout << buf[i];
	}
	this->Close();
}

//Для чтения данных по байтам
void SerialGate::GetSeparationData(int *arrayValue, int countUnitData)
{
	this->countReadByte = Recv(buf, sizeof(buf));

	for (int i = 0; i < countUnitData; i++)
	{
		arrayValue[i] = 0;
		arrayValue[i] = int(buf[i]) + 128;
	}
}
#endif
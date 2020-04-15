
#include<iostream>
#include <windows.h>
#include"mmsystem.h"
//#include <SDKDDKVer.h>
//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) //���ش���
#pragma comment(lib, "winmm.lib") 
#define WIN32_LEAN_AND_MEAN
using namespace std;


//int _t(/*int argc, _TCHAR* argv[]*/);
HWAVEIN hWaveIn;  //�����豸  
WAVEFORMATEX waveform; //�ɼ���Ƶ�ĸ�ʽ���ṹ��  
BYTE *pBuffer1;//�ɼ���Ƶʱ�����ݻ���  
WAVEHDR wHdr1; //�ɼ���Ƶʱ�������ݻ���Ľṹ��  
FILE *pf;  
int main()  
{  
	int time = 8;
	string filename = "";
	char pName[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, pName, MAX_PATH);
	int iName = 0;
	for (; pName[iName] != '\0'; iName++);
	for (; pName[iName] != '\\'; iName--);
	for(int i=0;i<iName;i++)
	{
		filename.push_back(pName[i]);
	}
	filename += "\\02.pcm";
	//cout <<"�ļ��洢·���� "<<filename << endl;
    HANDLE          wait;  
    waveform.wFormatTag = WAVE_FORMAT_PCM;//������ʽΪPCM  
    waveform.nSamplesPerSec = 8000;//�����ʣ�16000��/��  
    waveform.wBitsPerSample = 16;//�������أ�16bits/��  
    waveform.nChannels = 1;//������������2����  
    waveform.nAvgBytesPerSec = 16000;//ÿ��������ʣ�����ÿ���ܲɼ������ֽڵ�����  
    waveform.nBlockAlign = 2;//һ����Ĵ�С������bit���ֽ�������������  
    waveform.cbSize = 0;//һ��Ϊ0  
  
    wait = CreateEvent(NULL, 0, 0, NULL);  
    //ʹ��waveInOpen����������Ƶ�ɼ�  
    waveInOpen(&hWaveIn, WAVE_MAPPER, &waveform,(DWORD_PTR)wait, 0L, CALLBACK_EVENT);  
  
    //�����������飨������Խ���������飩����������Ƶ����  
    DWORD bufsize = 1024*100;//ÿ�ο���10k�Ļ���洢¼������  
    int r=fopen_s(&pf, filename.c_str(), "wb");
	if (r != 0)
	{
		//cout << "�ļ�·����������" << endl;
		return -1;
	}
    while (time--)//¼��20�����������������Ƶ��������紫������޸�Ϊʵʱ¼�����ŵĻ�����ʵ�ֶԽ�����  
    {  
        pBuffer1 = new BYTE[bufsize];  
        wHdr1.lpData = (LPSTR)pBuffer1;  
        wHdr1.dwBufferLength = bufsize;  
        wHdr1.dwBytesRecorded = 0;  
        wHdr1.dwUser = 0;  
        wHdr1.dwFlags = 0;  
        wHdr1.dwLoops = 1;  
        waveInPrepareHeader(hWaveIn, &wHdr1, sizeof(WAVEHDR));//׼��һ���������ݿ�ͷ����¼��  
        waveInAddBuffer(hWaveIn, &wHdr1, sizeof (WAVEHDR));//ָ���������ݿ�Ϊ¼�����뻺��  
        waveInStart(hWaveIn);//��ʼ¼��  
        Sleep(1000);//�ȴ�����¼��1s  
        waveInReset(hWaveIn);//ֹͣ¼��  
        fwrite(pBuffer1, 1, wHdr1.dwBytesRecorded, pf);  
        delete pBuffer1; 
		printf("%ds  ", time);
    }  
    fclose(pf);  
    waveInClose(hWaveIn);  
	//_t();
	return 0;  
}  

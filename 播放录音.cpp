#include<iostream>
#include <windows.h>
#include"mmsystem.h"
//#include <SDKDDKVer.h>
//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) //���ش���
#pragma comment(lib, "winmm.lib") 
#define WIN32_LEAN_AND_MEAN
using namespace std;


HWAVEIN hWaveIn;  //�����豸  
WAVEFORMATEX waveform; //�ɼ���Ƶ�ĸ�ʽ���ṹ��  
BYTE* pBuffer1;//�ɼ���Ƶʱ�����ݻ���  
WAVEHDR wHdr1; //�ɼ���Ƶʱ�������ݻ���Ľṹ��  
FILE* pf;
char buf[1024 * 1024 * 4];
string filename = "";
int SoundPlay() {
	FILE* thbgm;//�ļ�  
	int             cnt;
	HWAVEOUT        hwo;
	WAVEHDR         wh;
	WAVEFORMATEX    wfx;
	HANDLE          wait;

	wfx.wFormatTag = WAVE_FORMAT_PCM;//���ò��������ĸ�ʽ  
	wfx.nChannels = 1;//������Ƶ�ļ���ͨ������  
	wfx.nSamplesPerSec = 8000;//����ÿ���������źͼ�¼ʱ������Ƶ��  
	wfx.nAvgBytesPerSec = 16000;//���������ƽ�����ݴ�����,��λbyte/s�����ֵ���ڴ��������С�Ǻ����õ�  
	wfx.nBlockAlign = 2;//���ֽ�Ϊ��λ���ÿ����  
	wfx.wBitsPerSample = 16;
	wfx.cbSize = 0;//������Ϣ�Ĵ�С  
	wait = CreateEvent(NULL, 0, 0, NULL);
	waveOutOpen(&hwo, WAVE_MAPPER, &wfx, (DWORD_PTR)wait, 0L, CALLBACK_EVENT);//��һ�������Ĳ�����Ƶ���װ�������лط�  
	char pName[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, pName, MAX_PATH);
	int iName = 0;
	for (; pName[iName] != '\0'; iName++);
	for (; pName[iName] != '\\'; iName--);
	for (int i = 0; i < iName; i++)
	{
		filename.push_back(pName[i]);
	}
	filename += "\\02.pcm";
	
	cout << "Ŀ���ļ�·���� " << filename << endl;
	int r = fopen_s(&thbgm, filename.c_str(), "rb");
	if (r != 0)
	{
		cout << "*****\n*****\n*****\n�ļ���·��������" << endl;
		return -1;
	}
	cnt = fread(buf, sizeof(char), 1024 * 1024 * 4, thbgm);//��ȡ�ļ�4M�����ݵ��ڴ������в��ţ�ͨ��������ֵ��޸ģ������߳̿ɱ��������Ƶ���ݵ�ʵʱ���䡣��Ȼ���ϣ��������������Ƶ�ļ���Ҳ��Ҫ��������΢��һ��  
	int dolenght = 0;
	int playsize = 1024;
	cout << "������" << endl;
	while (cnt>0) {//��һ������Ҫ�ر�ע�������ѭ������֮���ܻ�̫����ʱ��ȥ����ȡ����֮��Ĺ�������Ȼ��ÿ��ѭ���ļ�϶���С����ա�������  
		wh.lpData = buf + dolenght;
		wh.dwBufferLength = playsize;
		wh.dwFlags = 0L;
		wh.dwLoops = 1L;
		waveOutPrepareHeader(hwo, &wh, sizeof(WAVEHDR));//׼��һ���������ݿ����ڲ���  
		waveOutWrite(hwo, &wh, sizeof(WAVEHDR));//����Ƶý���в��ŵڶ�������whָ��������  
		WaitForSingleObject(wait, INFINITE);//�������hHandle�¼����ź�״̬����ĳһ�߳��е��øú���ʱ���߳���ʱ��������ڹ����INFINITE�����ڣ��߳����ȴ��Ķ����Ϊ���ź�״̬����ú�����������  
		dolenght = dolenght + playsize;
		cnt = cnt - playsize;
		cout << "=";
	}
	cout <<"\n�������"<< endl;
	waveOutClose(hwo);
	fclose(thbgm);
	return 0;
}

int main()
{
	int r = SoundPlay();
	return r;
}
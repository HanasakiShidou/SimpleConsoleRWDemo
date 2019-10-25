#include <iostream>
#include <fstream>
#include "SimpleSerial.h"

#define msleep(mseconds) (usleep(mseconds * 1000))
#define def_szSerialPortPath "/dev/ttyUSB0"
#define def_szFilepath "badappledata.bin"

#define def_iBuffSize 704

char gc_szFilePath[]=def_szFilepath;
char gc_szSerialPortPath[]=def_szSerialPortPath;
char* g_pcFileBuffer;

using namespace std;
int main() {
    //计时
    long lCurrentTime;
    //串口初始化
    SimpleSerial objMainSerial = SimpleSerial(gc_szSerialPortPath, 115200);
    if(!objMainSerial.isSuccess())
        return -1;
    //文件初始化
    ifstream fobjDataFile;
    fobjDataFile.open(gc_szFilePath, ios::in | ios::binary | ios::ate);
    if(!fobjDataFile.is_open())
        return -1;
    //获取文件大小
    int c_iFileSize=fobjDataFile.tellg();
    fobjDataFile.seekg(0 , ios::beg);
    //设置文件读取缓冲区，一次性读取全部读取文件
    g_pcFileBuffer=new char[c_iFileSize];
    fobjDataFile.read(g_pcFileBuffer, c_iFileSize);
    //设置当前位置指针 开始发送数据
    int iCurrentReadPointer = 0;
    int iFrameCounter =0;
    while(iCurrentReadPointer + def_iBuffSize < c_iFileSize)
    {
        lCurrentTime=clock();  //开始传送的时间
        cout << "Sending Freame "<<iFrameCounter;
        cout <<", data size:" << objMainSerial.writePort(&g_pcFileBuffer[iCurrentReadPointer], def_iBuffSize) << endl;
        iFrameCounter++;
        iCurrentReadPointer= iCurrentReadPointer + 704;
        lCurrentTime= clock() - lCurrentTime; //发送结束时的时间
        lCurrentTime= lCurrentTime / (CLOCKS_PER_SEC/1000);
        msleep((100 - lCurrentTime < 0) ? 0 : (100 - lCurrentTime));  //延迟时间 = 0.1s - 发送数据所需时间
    }
    return 0;
}





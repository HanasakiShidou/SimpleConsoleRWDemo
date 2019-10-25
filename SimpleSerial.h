//
// Created by hai on 2019/9/1.
//
#include <iostream>
#include <cstdio>      /*标准输入输出定义*/
#include <cstdlib>     /*标准函数库定义*/
#include <unistd.h>     /*Unix 标准函数定义*/
#include <sys/types.h>  /*数据类型，比如一些XXX_t的那种*/
#include <sys/stat.h>   /*定义了一些返回值的结构，没看明白*/
#include <fcntl.h>      /*文件控制定义*/
#include <termios.h>    /*PPSIX 终端控制定义*/
#include <cerrno>      /*错误号定义*/
#include <strings.h>

#ifndef SIMPLECONSOLERW_SIMPLESERIAL_H
#define SIMPLECONSOLERW_SIMPLESERIAL_H

class SimpleSerial{
private:
    int m_fd= -1;
    int m_nSpeed= 9600;
public:
    void setMNSpeed(int mNSpeed);
    bool isSuccess(void);
    ssize_t readPort(void* buf , size_t count);
    ssize_t writePort(void* buf , size_t count);
    static int setPort(int fd,int nSpeed, int nBits , char nEvent , int nStop );
    int setPort(int nBits = 8 , char nEvent = 'N', int nStop = 1 );
    void openPort(char* path);
    int closePort(void);
    SimpleSerial(char* pchFilePath, int nSpeed);
    ~SimpleSerial();
};

#endif //SIMPLECONSOLERW_SIMPLESERIAL_H

//
// Created by hai on 2019/9/1.
//
#include "SimpleSerial.h"

void SimpleSerial::openPort(char* path) {
    this->m_fd=open(path,O_RDWR | O_NOCTTY | O_NONBLOCK | O_NDELAY );
//    this->m_fd=open(path,O_RDWR | O_NOCTTY | O_NONBLOCK );
    if(this->m_fd < 0){
        std::cerr<<"Open file failed!"<<std::endl;
    }
}

int SimpleSerial::setPort(int nBits, char nEvent, int nStop) {
    int nResult=this->setPort(this->m_fd,this->m_nSpeed,nBits,nEvent,nStop);
    return nResult;
}

int SimpleSerial::setPort(int fd,int nSpeed, int nBits, char nEvent, int nStop)
{
    struct termios newtio,oldtio;
/*保存测试现有串口参数设置，在这里如果串口号等出错，会有相关的出错信息*/
    if  ( tcgetattr( fd,&oldtio)  !=  0) {
        perror("SetupSerial 1");
        printf("tcgetattr( fd,&oldtio) -> %d\n",tcgetattr( fd,&oldtio));
        return -1;
    }
    bzero( &newtio, sizeof( newtio ) );
/*步骤一，设置字符大小*/
    newtio.c_cflag  |=  CLOCAL | CREAD;
    newtio.c_cflag &= ~CSIZE;
/*设置停止位*/
    switch( nBits )
    {
        case 7:
            newtio.c_cflag |= CS7;
            break;
        case 8:
            newtio.c_cflag |= CS8;
            break;
    }
/*设置奇偶校验位*/
    switch( nEvent )
    {
        case 'o':
        case 'O': //奇数
            newtio.c_cflag |= PARENB;
            newtio.c_cflag |= PARODD;
            newtio.c_iflag |= (INPCK | ISTRIP);
            break;
        case 'e':
        case 'E': //偶数
            newtio.c_iflag |= (INPCK | ISTRIP);
            newtio.c_cflag |= PARENB;
            newtio.c_cflag &= ~PARODD;
            break;
        case 'n':
        case 'N':  //无奇偶校验位
            newtio.c_cflag &= ~PARENB;
            break;
        default:
            break;
    }
    /*设置波特率*/
    switch( nSpeed )
    {
        case 2400:
            cfsetispeed(&newtio, B2400);
            cfsetospeed(&newtio, B2400);
            break;
        case 4800:
            cfsetispeed(&newtio, B4800);
            cfsetospeed(&newtio, B4800);
            break;
        case 9600:
            cfsetispeed(&newtio, B9600);
            cfsetospeed(&newtio, B9600);
            break;
        case 115200:
            cfsetispeed(&newtio, B115200);
            cfsetospeed(&newtio, B115200);
            break;
        case 460800:
            cfsetispeed(&newtio, B460800);
            cfsetospeed(&newtio, B460800);
            break;
        default:
            cfsetispeed(&newtio, B9600);
            cfsetospeed(&newtio, B9600);
            break;
    }
/*设置停止位*/
//    newtio.c_cflag |= CRTSCTS;
    newtio.c_cflag &= ~CRTSCTS;
    if( nStop == 1 )
        newtio.c_cflag &=  ~CSTOPB;
    else if ( nStop == 2 )
        newtio.c_cflag |=  CSTOPB;
/*设置等待时间和最小接收字符*/
    newtio.c_cc[VTIME]  = 0;
    newtio.c_cc[VMIN] = 0;
/*处理未接收字符*/
    tcflush(fd,TCIFLUSH);
/*激活新配置*/
    if((tcsetattr(fd,TCSANOW,&newtio))!=0)
    {
        perror("com set error :");
        return -1;
    }
    printf("set done!\n");
/*    {
        struct termios mytio=newtio;
        std::cout<<"newtio:"<<std::endl;
        std::cout<<"mytio.c_cc :"<<mytio.c_cc<<std::endl;
        std::cout<<"mytio.c_cflag :"<<mytio.c_cflag<<std::endl;
        std::cout<<"mytio.c_iflag :"<<mytio.c_iflag<<std::endl;
        std::cout<<"mytio.c_ispeed :"<<mytio.c_ispeed<<std::endl;
        std::cout<<"mytio.c_lflag :"<<mytio.c_lflag<<std::endl;
        std::cout<<"mytio.c_oflag :"<<mytio.c_oflag<<std::endl;
        std::cout<<"mytio.c_ospeed :"<<mytio.c_ospeed<<std::endl;
    }*/
    return 0;
}

ssize_t SimpleSerial::readPort(void *buf, size_t count) {
    ssize_t nResult=read(this->m_fd,buf,count);
    if(nResult<0)
        perror("read port failed! :");
    return nResult;
}

ssize_t SimpleSerial::writePort(void *buf, size_t count) {
    ssize_t nResult=write(this->m_fd,buf,count);
    if(nResult<0)
        perror("write port failed! :");
/*  else
//        std::cerr<<"write port success!"<<std::endl; */
    return nResult;
}

int SimpleSerial::closePort(void) {
    int nResult=close(this->m_fd);
    return nResult;
}

void SimpleSerial::setMNSpeed(int mNSpeed) {
    m_nSpeed = mNSpeed;
}

SimpleSerial::SimpleSerial(char *pchFilePath, int nSpeed) :m_nSpeed(nSpeed){
    this->openPort(pchFilePath);
    if(this->isSuccess())
        this->setPort();
    if(fcntl(this->m_fd, F_SETFL, 0)<0)
        perror("fcntl failed!");
    else
        std::cerr<<"fcntl="<<fcntl(this->m_fd, F_SETFL,0)<<std::endl;
 /*   {
        struct termios mytio;
        tcgetattr(this->m_fd,&mytio);
        std::cout<<"mytio.c_cc :"<<mytio.c_cc<<std::endl;
        std::cout<<"mytio.c_cflag :"<<mytio.c_cflag<<std::endl;
        std::cout<<"mytio.c_iflag :"<<mytio.c_iflag<<std::endl;
        std::cout<<"mytio.c_ispeed :"<<mytio.c_ispeed<<std::endl;
        std::cout<<"mytio.c_lflag :"<<mytio.c_lflag<<std::endl;
        std::cout<<"mytio.c_oflag :"<<mytio.c_oflag<<std::endl;
        std::cout<<"mytio.c_ospeed :"<<mytio.c_ospeed<<std::endl;
    }*/
    /*测试是否为终端设备*/ //no need to test
/*    if(isatty(STDIN_FILENO)==0)
        printf("standard input is not a terminal device\n");
    else
        printf("isatty success!\n");
    printf("fd-open=%d\n",this->m_fd);*/

}

bool SimpleSerial::isSuccess(void) {
    return this->m_fd >= 0 ;
}

SimpleSerial::~SimpleSerial() {
    closePort();
}

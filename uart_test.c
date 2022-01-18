/************************Copyright(c)*****************************
**                       KONTRONNTECK
**                       Embedded Computering Technology
**                       Software Team
**                       WWW.KONTRONN.COM
**----------------------FileInfo-----------------------------------
** File name:               main.c
** Last modified Date:      2014-07-31
** Last Version:            2.0
** Descriptions:            UART programming
**-----------------------------------------------------------------
** Created by:                  vxbus 
** Created date:                2014-07-31
** Version:                     2.0
** Descriptions:                The original version
**------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
*******************************************************************/
// Including the relevant headers for UART programming
#include"stdio.h"      /* Standard input and output */
#include"stdlib.h"     /* Standard library */
#include"unistd.h"     /* Standard function definition for UNIX */
#include"sys/types.h" 
#include"sys/stat.h"   
#include"fcntl.h"      /* Definition for file control*/
#include"termios.h"    /* Definition for terminal control*/
#include"errno.h"      /* Definition for error signal*/
#include"string.h"
 
//Macro definition
#define FALSE  -1
#define TRUE   0
 
/*******************************************************************
* name:             UART0_Open
* function:         open the UART and get the file descriptor
* in param:         fd: file descriptor    port:UART devices(ttyS0, ttyS1, ttyS2)
* out param:        1 if success or 0 if fail
*******************************************************************/
int UART0_Open(int fd,char* port)
{
	fd = open( port, O_RDWR|O_NOCTTY|O_NDELAY);
	if(FALSE == fd)
	{
		perror("Can't Open Serial Port");
		return(FALSE);
	}
	//set the UART into blocked mode
	if(fcntl(fd, F_SETFL, 0) < 0)
	{
		printf("fcntl failed!\n");
		return(FALSE);
	}
	else
	{
		printf("fcntl=%d\n",fcntl(fd, F_SETFL,0));
	}
	// check whether this serial port is terminal device or not
	if(0 == isatty(STDIN_FILENO))
	{
		printf("standard input is not a terminal device\n");
		return(FALSE);
	}
	else
	{
		printf("isatty success!\n");
	}
	printf("fd->open=%d\n",fd);
	return fd;
}
/*******************************************************************
* name:			UART0_Close
* function:		Close the UART
* in param:		fd: file descriptor port:UART(ttyS0,ttyS1,ttyS2)
* out param:	void
*******************************************************************/
void UART0_Close(int fd)
{
    close(fd);
}
/*******************************************************************
* name:			UART0_Set
* function:		set the data bits, stop bit, paraity bit of UART
* in param:		fd: the file descriptor of the UART
*               speed: UART speed
*               flow_ctrl: data flow control
*               databits: data bits, 7 or 8
*               stopbits: stop bit, 1 or 2
*               parity: parity mode, N,E,O,S
* out param:    return 0 if success, or 1 if fail
*******************************************************************/
int UART0_Set(int fd,int speed,int flow_ctrl,int databits,int stopbits,int parity)
{
	int i;
	int status;
	int speed_arr[] = { B115200, B19200, B9600, B4800, B2400, B1200, B300};
	int name_arr[] = {115200,  19200,  9600,  4800,  2400,  1200,  300};
	
	struct termios options;
	
	tcgetattr(fd,&options)
	if(tcgetattr(fd, &options) != 0)
	{
		perror("SetupSerial 1");
		return(FALSE);
	}
	//set the baudrate for the input and output of the UART
	for(i=0; i < sizeof(speed_arr)/sizeof(int);  i++)
	{
		if(speed == name_arr[i])
		{
			cfsetispeed(&options, speed_arr[i]);
			cfsetospeed(&options, speed_arr[i]);
		}
	}
	
	//modify the control mode of the UART
	options.c_cflag |= CLOCAL;
	options.c_cflag |= CREAD;
  
    // set the data flow control
	switch(flow_ctrl)
    {
		case 0:
			options.c_cflag &= ~CRTSCTS;
			break;   
		case 1:
			options.c_cflag |= CRTSCTS;
			break;
		case 2:
			options.c_cflag |= IXON | IXOFF | IXANY;
			break;
	}
	//set the data bits
	// mask other flags
    options.c_cflag &= ~CSIZE;
    switch (databits:)
    {  
       case 5    :
                     options.c_cflag |= CS5;
                     break;
       case 6    :
                     options.c_cflag |= CS6;
                     break;
       case 7    :    
                 options.c_cflag |= CS7;
                 break;
       case 8:    
                 options.c_cflag |= CS8;
                 break;  
       default:   
                 fprintf(stderr,"Unsupported data size\n");
                 return (FALSE); 
    }
	// set parity bits
    switch (parity)
    {  
       case 'n':
       case 'N':
                 options.c_cflag &= ~PARENB; 
                 options.c_iflag &= ~INPCK;    
                 break; 
       case 'o':  
       case 'O':   
                 options.c_cflag |= (PARODD | PARENB); 
                 options.c_iflag |= INPCK;             
                 break; 
       case 'e': 
       case 'E': 
                 options.c_cflag |= PARENB;       
                 options.c_cflag &= ~PARODD;       
                 options.c_iflag |= INPCK;      
                 break;
       case 's':
       case 'S':
                 options.c_cflag &= ~PARENB;
                 options.c_cflag &= ~CSTOPB;
                 break; 
        default:  
                 fprintf(stderr,"Unsupported parity\n");    
                 return (FALSE); 
    } 
    // set stop bit
    switch (stopbits)
    {  
       case 1:
			options.c_cflag &= ~CSTOPB; break; 
       case 2:
			options.c_cflag |= CSTOPB; break;
       default:
			fprintf(stderr,"Unsupported stop bits\n"); 
            return (FALSE);
    }
	
	//modify the output mode, using the RAM mode
	options.c_oflag &= ~OPOST;
	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	//options.c_lflag &= ~(ISIG | ICANON);

    options.c_cc[VTIME] = 1;
	options.c_cc[VMIN] = 1;

	tcflush(fd,TCIFLUSH);

	// active the configuration
	if(tcsetattr(fd,TCSANOW,&options) != 0)
	{
		perror("com set error!\n");
		return (FALSE);
	}
	
	return (TRUE); 
}
/*******************************************************************
* name:			UART0_Init()
* function:     UART initialization
* in param:		fd: the file descriptor of the UART
*               speed: UART speed
*               flow_ctrl: data flow control
*               databits: data bits, 7 or 8
*               stopbits: stop bit, 1 or 2
*               parity: parity mode, N,E,O,S
*                      
* out param:        return 0 if success, or 1 if fail
*******************************************************************/
int UART0_Init(int fd, int speed,int flow_ctrl,int databits,int stopbits,int parity)
{
	int err;
	if (UART0_Set(fd,19200,0,8,1,'N') == FALSE)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}
/*******************************************************************
* name:			UART0_Recv
* function:     receive the UART data
* in param:		fd: file descriptor
*				rcv_buf: buffer for received data
*				data_len: length of the data frame
* out param:	return 0 if success, or 1 if fail
*******************************************************************/
int UART0_Recv(int fd, char *rcv_buf,int data_len)
{
    int len,fs_sel;
    fd_set fs_read;
   
    struct timeval time;
   
    FD_ZERO(&fs_read);
    FD_SET(fd,&fs_read);
   
    time.tv_sec = 10;
    time.tv_usec = 0;
   
    //useing select() to implement the multiply channels' communication
    fs_sel = select(fd+1,&fs_read,NULL,NULL,&time);
    if(fs_sel)
	{
		len = read(fd,rcv_buf,data_len);
		printf("I am right!(version1.2) len = %d fs_sel = %d\n",len,fs_sel);
		return len;
	}
	else
	{
		printf("Sorry,I am wrong!");
		return FALSE;
	}     
}
/********************************************************************
* name:         UART0_Send
* function:     sending data
* in param:     fd: file descriptor    
*               send_buf: buffer storaging the data will be sent
*               data_len: the length of the data frame
* out param:    return 0 if success, or 1 if fail
*******************************************************************/
int UART0_Send(int fd, char *send_buf,int data_len)
{
	int len = 0;
   
	len = write(fd,send_buf,data_len);
    if (len == data_len )
	{
		return len;
	}
	else
	{
		tcflush(fd,TCOFLUSH);
		return FALSE;
	}
}
int main(int argc, char **argv)
{
    int fd;		// file descriptor
    int err;	// returned status
    int len;                        
    int i;
    char rcv_buf[100];       
    char send_buf[20]="tiger john";
    if(argc != 3)
    {
        printf("Usage: %s /dev/ttySn 0(send data)/1 (receive data) \n",argv[0]);
		return FALSE;
	}
    fd = UART0_Open(fd,argv[1]); //open the UART, return the file descriptor
    do{
		err = UART0_Init(fd,19200,0,8,1,'N');
		printf("Set Port Exactly!\n");
	}while(FALSE == err || FALSE == fd);
   
	if(0 == strcmp(argv[2],"0"))
	{
		for(i=0; i < 10; i++)
        {
			len = UART0_Send(fd, send_buf, 10);
			if(len > 0)
				printf(" %d send data successful\n",i);
			else
				printf("send data failed!\n");
				sleep(2);
		}
		UART0_Close(fd);
	}
	else
	{
		while (1) //loop to read the data
		{
			len = UART0_Recv(fd, rcv_buf,9);
			if(len > 0)
			{
				rcv_buf[len] = '\0';
				printf("receive data is %s\n",rcv_buf);
				printf("len = %d\n",len);
			}
			else
			{
				printf("cannot receive data\n");
			}
			sleep(2);
		}
		UART0_Close(fd);
	}
}
/***************************** End Of File *********************************/

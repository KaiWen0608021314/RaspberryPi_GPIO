#include <string>
#include <sstream>
#include <utility>
#include <string>
#include <fstream>
#include <iostream>
#include <chrono>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

using namespace std;

#define TRUE	((bool) 1)
#define FALSE	((bool) 0)

#define GPIO_DEV    "/dev/etx_device"
#define GPIO_SET_OUTPUT		11
#define GPIO_SET_INPUT		12
#define GPIO_OUTPUT_LOW	    13
#define GPIO_OUTPUT_HIGH	14
#define GPIO_GET_VALUE		15
static int fd = -1;


bool GPIOInit()
{
    fd = open(GPIO_DEV, O_RDWR);
    if(fd < 0) {
		cout << "can't open gpio device" << endl;
		return FALSE;
	}

	fcntl(fd, F_SETFD, fcntl(fd, F_GETFD) | FD_CLOEXEC);
    cout << "GPIO device open OK~" << endl;
    return TRUE;
}

bool SetGPIO(int port_num, int value)
{
    int ret;
	int buf = port_num;

	if (fd < 0) {
		cout << "GPIO not open yet" << endl;
		return FALSE;
	}
    if(value == 0) {
		ret = ioctl(fd, GPIO_OUTPUT_LOW, &buf);
	} else {
		ret = ioctl(fd, GPIO_OUTPUT_HIGH, &buf);
	}

    if (ret < 0) {
		cout << "Port=" << port_num<< ", Value=" << value << endl;
		return FALSE;
	}

	cout << "Port=" << port_num<< ", Value=" << value << endl;

	return TRUE;
}

bool GetGPIO(int port_num, int *value)
{
    int ret;
	int buf = port_num;

    if (fd < 0) {
		cout << "GPIO not open yet" << endl;
		return FALSE;
	}

	ret = ioctl(fd, GPIO_GET_VALUE, &buf);
	if (ret < 0) {
		cout << "Port=" << port_num << endl;
		return FALSE;
	}

	if (buf > 0) {
		*value = 1;
	} else {
		*value = 0;
	}

	cout << "Port=" << port_num<< ", Value=" << *value << endl;

	return TRUE;
}

bool GPIOClose()
{
    if(fd > 0)
	{
		cout << "GPIO device close ~" << endl;
		close(fd);
		fd = -1;
	}
	else
		cout << "GPIO device dosen't need to close" << endl;
	
	return TRUE;
}

static void print_usage(const char *prog)
{
    cout << "port_num   : port number of GPIO, from 0 to 26" << endl;
    cout << "value  : write out to port if assign value." << endl;
    cout << "         otherwise, read value from port."<< endl;
}


int main(int argc, char *argv[])
{
    int port_num = 0;
    int value = 0;

    cout << "Start GPIO device" << endl;

    if (argc < 2 || argc > 3) {
        cout << "argument is wrong !! " << endl;
        print_usage(argv[0]);
        return 0;
    }

    if (!GPIOInit())
    {
        cout << "GPIO init fail!!" << endl;
    }

    port_num = atoi(argv[1]);
    if (port_num < 0 || port_num > 26)
    {
        cout << "port number assign error" << endl;
        return 0;    
    }

    if (argc == 2)
    {
        GetGPIO(port_num, &value);
        cout << "Read " << value << " from GPIO: " << port_num  << endl;
    }

    if (argc == 3)
    {
        if (atoi(argv[2]) > 0) {
            value = 1;
        } else {
            value = 0;
        }

        SetGPIO(port_num, value);
        cout << "Write " << value << " to GPIO: " << port_num << endl;        
    }

    return 0;
}
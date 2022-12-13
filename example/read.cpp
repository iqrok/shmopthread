#include <iostream>

#include "ShmopThread.h"

struct SharedData {
	float inFloat;
	int32_t inInt;
	double inDouble;
};

ShmopThread pShm;
SharedData buf;
SharedData *ptr;

void callback(void *p_data){
	pShm.get_data(&buf);

	std::cout << "float	: " << buf.inFloat << std::endl;
	std::cout << "int	: " << buf.inInt << std::endl;
	std::cout << "double: " << buf.inDouble << std::endl;
	std::cout << "---------------------------------------------" << std::endl;
}

int main(void){
	pShm.set_file_name("shm-out.tmp");
	pShm.set_flag(O_CREAT | O_RDONLY);
	pShm.set_mode(0777);
	pShm.set_direction(DIRECTION_READ);
	pShm.set_prot(PROT_READ);
	pShm.set_thread_mode(THREAD_MODE_JOINED);
	pShm.set_period_ms(100);
	pShm.set_data_size(sizeof(SharedData));
	pShm.set_data(&buf);
	pShm.attach(callback);

	pShm.start();

	return 0;
}

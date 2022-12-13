#include <cstdint>

#include "ShmopThread.h"

struct SharedData {
	float inFloat;
	int32_t inInt;
	double inDouble;
};

ShmopThread pShm;
SharedData buf = { 10.4321, 0x40, 12345.6789 };
SharedData *ptr;

void callback(void *p_data){
	buf.inDouble -= 1;
	pShm.set_data(&buf);
}

int main(void){
	pShm.set_file_name("shm-out.tmp");
	pShm.set_flag(O_CREAT | O_RDWR);
	pShm.set_mode(0777);
	pShm.set_direction(DIRECTION_WRITE);
	pShm.set_prot(PROT_READ|PROT_WRITE);
	pShm.set_thread_mode(THREAD_MODE_JOINED);
	pShm.set_period_ms(50);
	pShm.set_data_size(sizeof(SharedData));
	pShm.set_data(&buf);
	pShm.attach(callback);

	pShm.start();

	return 0;
}

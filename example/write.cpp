#include <cstdint>

#include "ShmopThread.h"

struct SharedData {
	float inFloat;
	int32_t inInt;
	double inDouble;
};

ShmopThread *pShm = new ShmopThread();
ShmopThread pShm2;
//~ SharedData buf = { 10.4321, 0x40, 12345.6789 };

uint8_t *buf2;
uint8_t *buf3;

void callback(void *p_data){
	buf2[0]++;

	for(uint8_t idx = 0; idx < 16; idx++){
		printf("0x%02x ", buf2[idx]);

		//~ if(buf2[idx - 1] % 0x10) continue;
		//~ buf2[idx] += 1;
	}
	printf("\t cb1 \n");
	//~ pShm->set_data(buf2);
}

void callback2(void *p_data){
	buf3[0]--;

	for(uint8_t idx = 0; idx < 16; idx++){
		printf("0x%02x ", buf3[idx]);

		//~ if(buf2[idx - 1] % 0x10) continue;
		//~ buf2[idx] += 1;
	}
	printf("\t cb2 \n");
	//~ pShm2.set_data(buf3);
}

void wut(void *p){
	printf("wut\n");
}

int main(void){
	buf2 = new uint8_t[16];
	for(uint8_t idx = 0; idx < 16; idx++){
		buf2[idx] = idx;
	}

	buf3 = new uint8_t[16];
	for(uint8_t idx = 0; idx < 16; idx++){
		buf3[idx] = idx;
	}

	pShm->set_file_name("shm-out.tmp");
	pShm->set_flag(O_CREAT | O_RDWR);
	pShm->set_mode(0777);
	pShm->set_direction(DIRECTION_WRITE);
	pShm->set_prot(PROT_READ|PROT_WRITE);
	pShm->set_thread_mode(THREAD_MODE_DETACHED);
	pShm->set_period_us(25000);
	pShm->set_byte_size(16);
	pShm->attach_data(buf2);
	pShm->attach_callback(callback);

	pShm2.set_file_name("shm-in.tmp");
	pShm2.set_flag(O_CREAT | O_RDWR);
	pShm2.set_mode(0777);
	pShm2.set_direction(DIRECTION_WRITE);
	pShm2.set_prot(PROT_READ|PROT_WRITE);
	pShm2.set_thread_mode(THREAD_MODE_DETACHED);
	pShm2.set_period_us(50000);
	pShm2.set_byte_size(16);
	pShm2.attach_data(buf3);
	pShm2.attach_callback(callback2);

	printf("pSHm: %d, pShm2: %d\n",pShm->get_byte_size(),pShm2.get_byte_size());

	{
		pShm2.start();
		pShm->start();
	}

	while(1){
		std::this_thread::sleep_for(std::chrono::microseconds(10000));
	}

	pShm->stop();
	pShm2.stop();

	return 0;
}

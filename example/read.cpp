#include <iostream>
#include <chrono>
#include <thread>

#include "ShmopThread.h"

struct SharedData {
	float inFloat;
	int32_t inInt;
	double inDouble;
};

ShmopThread pShm;
//~ SharedData buf;
uint8_t *buf2;

void callback(void *p_data){
	pShm.get_data(buf2);
	//~ printf("Callback\n");
	//~ for(uint8_t idx = 0; idx < 16; idx++){
		//~ printf("0x%02x ", buf2[idx]);
	//~ }
	//~ printf("\n");
	//~ std::cout << "float	: " << buf.inFloat << std::endl;
	//~ std::cout << "int	: " << buf.inInt << std::endl;
	//~ std::cout << "double: " << buf.inDouble << std::endl;
	//~ std::cout << "---------------------------------------------" << std::endl;
}

int main(void){
	buf2 = new uint8_t[16];

	pShm.set_file_name("shm-in.tmp");
	pShm.set_flag(O_CREAT | O_RDONLY);
	pShm.set_mode(0777);
	pShm.set_direction(DIRECTION_READ);
	pShm.set_prot(PROT_READ);
	//~ pShm.set_thread_mode(THREAD_MODE_JOINED);
	pShm.set_thread_mode(THREAD_MODE_DETACHED);
	pShm.set_period_ms(100);
	pShm.set_byte_size(16);
	//~ pShm.set_byte_size(sizeof(SharedData));
	pShm.set_data(buf2);
	pShm.attach(callback);

	printf("SIZE: 0x%02ld\n", sizeof(buf2));
	pShm.init();

	printf("SIZE: 0x%02ld\n", sizeof(*buf2));
	while(1){
		pShm.get_data(buf2);
		for(uint8_t idx = 0; idx < pShm.get_byte_size(); idx++){
			printf("0x%02x ", buf2[idx]);
		}
		printf("\n");
		std::this_thread::sleep_for(std::chrono::microseconds(100000));
	}
	pShm.uninit();

	return 0;
}

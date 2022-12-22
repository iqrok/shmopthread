extern "C" {

#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h> /* clock_gettime() */
#include <sys/resource.h>
#include <sys/stat.h>
#include <string.h>

}

#include "ShmopThread.h"

#define NSEC_PER_SEC 1000000000
#define MAX_SAFE_STACK 1024

void stack_prefault(void){
	unsigned char dummy[MAX_SAFE_STACK];
	memset(dummy, 0, MAX_SAFE_STACK);
}

ShmopThread::ShmopThread(){
	set_default_value();
}

void ShmopThread::threaded_routine(struct timespec *wakeup_time){
	while(1){
		(*thread_fn)(ptr);

		switch (direction){
			case DIRECTION_READ:
				memcpy(data, ptr, byte_size);
				break;

			case DIRECTION_WRITE:
				memcpy(ptr, data, byte_size);
				break;

			default: break;
		}

		wait_for_given_periods(wakeup_time);
	}
}

void ShmopThread::set_default_value(void){
	direction = DIRECTION_READ;
	thread_mode = THREAD_MODE_JOINED;
	isOpen = false;
	isInitialized = false;
}

void ShmopThread::set_file_name(std::string _fname){
	fname = _fname.c_str();
}

void ShmopThread::set_file_name(const char * _fname){
	fname = _fname;
}

void ShmopThread::set_flag(int _oflag){
	oflag = _oflag;
}

void ShmopThread::set_prot(int _prot){
	prot = _prot;
}

void ShmopThread::set_mode(mode_t _mode){
	mode = _mode;
}

void ShmopThread::set_byte_size(uint32_t _byte_size){
	byte_size = _byte_size;
}

void ShmopThread::set_period_ns(uint64_t t_ns){
	period_ns = t_ns;
}

void ShmopThread::set_period_us(uint32_t t_us){
	period_ns = t_us * 1E3;
}

void ShmopThread::set_period_ms(uint32_t t_ms){
	period_ns = t_ms * 1E6;
}

void ShmopThread::set_data(void *_data){
	data = _data;
}

void ShmopThread::get_data(void *_data){
	memcpy(_data, ptr, byte_size);
}

void ShmopThread::set_direction(uint8_t _direction){
	direction = _direction;
}

void ShmopThread::attach(void (*fn)(void*)) {
	thread_fn = fn;
}

void ShmopThread::set_thread_mode(uint8_t _thread_mode) {
	thread_mode = _thread_mode;
}

uint32_t ShmopThread::get_byte_size(){
	return byte_size;
};

void ShmopThread::wait_for_given_periods(struct timespec *wakeup_time){
	clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, wakeup_time, NULL);

	wakeup_time->tv_nsec += period_ns;
	while (wakeup_time->tv_nsec >= NSEC_PER_SEC) {
		wakeup_time->tv_nsec -= NSEC_PER_SEC;
		wakeup_time->tv_sec++;
	}
}

int ShmopThread::uninit(void){
	if((fd = shm_unlink(fname))){
		perror("shm open failed");
	}

	isInitialized = false;

	return fd;
}

void* ShmopThread::init(void){
	if((fd = shm_open(fname, oflag, mode)) < 0){
		perror("shm open failed!");
	}

	if(ftruncate(fd, byte_size) < 0){
		perror("Failed at truncate!");
	}

	ptr = mmap(NULL, byte_size, prot, MAP_SHARED, fd, 0);

	// close file descriptor
	close(fd);

	isInitialized = true;

	return ptr;
}

int ShmopThread::stop(void){
	if(isInitialized) uninit();

	if(thread_mode != THREAD_MODE_DETACHED) thrd.join();

	if(isOpen) pthread_cancel(thread_handler);

	isOpen = false;

	return fd;
}

int ShmopThread::start(void){
	if(!isInitialized) init();

	stack_prefault();

	clock_gettime(CLOCK_MONOTONIC, &wakeup_time);
	wakeup_time.tv_sec += 1; // start in future +1 sec
	wakeup_time.tv_nsec = 0;

	thrd = std::thread(
			&ShmopThread::threaded_routine,
			this,
			&wakeup_time
		);

	thread_handler = thrd.native_handle();

	isOpen = true;

	if(thread_mode == THREAD_MODE_DETACHED) thrd.detach();

	return fd;
}

#ifndef _SHMOPTHREAD_H_
#define _SHMOPTHREAD_H_

extern "C" {

#include <fcntl.h>
#include <sys/mman.h>
#include <pthread.h>

}

#include <cstdint>
#include <string>
#include <thread>

#define DIRECTION_READ 0
#define DIRECTION_WRITE 1

#define THREAD_MODE_DETACHED 0
#define THREAD_MODE_JOINED 1

class ShmopThread {
	public:
		ShmopThread();

		void set_file_name(std::string);
		void set_file_name(const char*);
		void set_flag(int);
		void set_prot(int);
		void set_mode(mode_t);
		void set_byte_size(uint32_t);
		void set_addr(int);
		void set_direction(uint8_t);
		void set_thread_mode(uint8_t);
		void set_preserve_link(bool);

		void set_period_ns(uint64_t);
		void set_period_ms(uint32_t);
		void set_period_us(uint32_t);

		uint32_t get_byte_size();

		void attach_data(void*);
		void attach_callback(void (*)(void*));

		void* init(void);
		int uninit(void);
		int start(void);
		int stop(void);

		void read(void*);
		void write(void*);

		void read(void);
		void write(void);

	private:
		bool preserve_link;
		int oflag;
		int prot;
		mode_t mode;
		uint64_t period_ns;
		uint8_t direction;
		uint8_t thread_mode;
		const char *fname;
		uint32_t byte_size;

		bool isOpen;
		bool isInitialized;

		int fd;
		void *ptr;
		void *data;

		std::thread thrd;
		pthread_t thread_handler;
		struct timespec wakeup_time;

		void set_default_value(void);
		void wait_for_given_periods(struct timespec*);
		void threaded_routine(struct timespec*);

		void (*thread_fn)(void *p_data);
};

#endif

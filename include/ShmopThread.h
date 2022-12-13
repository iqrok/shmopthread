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
		void set_data_size(uint32_t);
		void set_addr(int);
		void set_direction(uint8_t);

		void set_period_ns(uint64_t);
		void set_period_ms(uint32_t);
		void set_period_us(uint32_t);

		void attach(void (*)(void*));

		int start(void);
		int stop(void);

		void set_data(void*);
		void get_data(void*);

		void set_thread_mode(uint8_t);

	private:
		int oflag;
		int prot;
		mode_t mode;
		uint32_t data_size;
		uint64_t period_ns;
		uint8_t direction;
		uint8_t thread_mode;
		const char *fname;

		bool isOpen;

		int fd;
		void *ptr;
		void *data;

		pthread_t thread_handler;

		void set_default_value(void);
		void wait_for_given_periods(struct timespec*);
		void threaded_routine(struct timespec*);
};

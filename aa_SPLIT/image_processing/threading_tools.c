# include "image_processing.h"

int	get_num_cores(void)// works!!! must try on windows
{
	int	num_cores;

	#if defined(_WIN32) || defined(_WIN64)
		SYSTEM_INFO sysinfo;
		GetSystemInfo(&sysinfo);
		num_cores = sysinfo.dwNumberOfProcessors;
	#else
		num_cores = sysconf(_SC_NPROCESSORS_ONLN);
		if (num_cores < 1)
		{
			perror("sysconf error\n");
			return (1);
		}
	#endif
	return (num_cores);
}
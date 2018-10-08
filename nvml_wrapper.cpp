#include "nvml_wrapper.hpp"

#include "nvml.h"

#include <dlfcn.h>

typedef nvmlReturn_t (*nvmlInit_v2_p)();
typedef nvmlReturn_t (*nvmlShutdown_p)();
typedef nvmlReturn_t (*nvmlDeviceGetCount_v2_p)(unsigned int*);
typedef nvmlReturn_t (*nvmlDeviceGetHandleByIndex_v2_p)(unsigned int, nvmlDevice_t*);
typedef nvmlReturn_t (*nvmlDeviceGetTemperature_p)(nvmlDevice_t, nvmlTemperatureSensors_t, unsigned int*);

#define NULL (void*)0

struct nvidia_func_ptrs
{
	nvidia_func_ptrs():nvml_so(NULL){}
	~nvidia_func_ptrs() {
		if(nvml_so != NULL){
			dlclose(nvml_so);
		}
	}

	void *nvml_so;
	nvmlInit_v2_p nvmlInit_v2;
	nvmlShutdown_p nvmlShutdown;
	nvmlDeviceGetCount_v2_p nvmlDeviceGetCount_v2;
	nvmlDeviceGetHandleByIndex_v2_p nvmlDeviceGetHandleByIndex_v2;
	nvmlDeviceGetTemperature_p nvmlDeviceGetTemperature;
};

// TODO: Error hadling and all that jazz
// ALso, if we care about handling non nvidia cards, the whole
// library loading jazz may need to be done dyanmically as some
// users may not have the nvidia drivers installed.

nvml_wrapper::nvml_wrapper()
{
	m_ptrs = new nvidia_func_ptrs;
	if(!nvml_so_init()){
		delete m_ptrs;
		return;
	}

	nvmlReturn_t result = m_ptrs->nvmlInit_v2();

	if(NVML_SUCCESS != result){
		// Oops, big error.
	}

	unsigned int temp_count;

	result = m_ptrs->nvmlDeviceGetCount_v2(&temp_count);

	m_gpu_count = (int) temp_count;

	for(unsigned int i = 0; i < temp_count; i++){
		nvmlDevice_t device;
		m_ptrs->nvmlDeviceGetHandleByIndex_v2(i, &device);
		m_devices.push_back(device);
	}
}

bool nvml_wrapper::nvml_so_init()
{
	// For now, hardcode the path that the fedora binary driver puts the file
	// TODO: Make this a config option
	const char libpath[] = "/usr/lib64/nvidia/libnvidia-ml.so";

	m_ptrs->nvml_so = dlopen(libpath, RTLD_LAZY);
	if(m_ptrs->nvml_so == NULL){
		return false;
	}

	m_ptrs->nvmlInit_v2 = (nvmlInit_v2_p) dlsym(m_ptrs->nvml_so, "nvmlInit_v2");
	if(m_ptrs->nvmlInit_v2 == NULL){
		return false;
	}

	m_ptrs->nvmlShutdown = (nvmlShutdown_p) dlsym(m_ptrs->nvml_so, "nvmlShutdown");
	if(m_ptrs->nvmlShutdown == NULL){
		return false;
	}

	m_ptrs->nvmlDeviceGetCount_v2 = (nvmlDeviceGetCount_v2_p) dlsym(m_ptrs->nvml_so, "nvmlDeviceGetCount_v2");
	if(m_ptrs->nvmlDeviceGetCount_v2 == NULL){
		return false;
	}

	m_ptrs->nvmlDeviceGetHandleByIndex_v2 = (nvmlDeviceGetHandleByIndex_v2_p) dlsym(m_ptrs->nvml_so, "nvmlDeviceGetHandleByIndex_v2");
	if(m_ptrs->nvmlDeviceGetHandleByIndex_v2 == NULL){
		return false;
	}

	m_ptrs->nvmlDeviceGetTemperature = (nvmlDeviceGetTemperature_p) dlsym(m_ptrs->nvml_so, "nvmlDeviceGetTemperature");
	if(m_ptrs->nvmlDeviceGetTemperature == NULL){
		return false;
	}

	return true;
}

nvml_wrapper::~nvml_wrapper()
{
	m_ptrs->nvmlShutdown();
	delete m_ptrs;
}

int nvml_wrapper::gpu_count()
{
	return m_gpu_count;
}

int nvml_wrapper::gpu_core_temp(int gpu_id)
{
	unsigned int temp;

	m_ptrs->nvmlDeviceGetTemperature(m_devices[gpu_id],NVML_TEMPERATURE_GPU,&temp);

	return (int) temp;
}

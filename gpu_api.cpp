
#include "gpu_api.hpp"

#if defined(FM_WIN)
	#include "nvapi_wrapper.hpp"
#elif (FM_LINUX)
	#include "nvml_wrapper.hpp"
#endif

gpu_api::gpu_api()
{

}

gpu_api::~gpu_api()
{

}


gpu_api* gpu_api::create_instance()
{
	// TODO: If I ever support AMD gpus, add some autodetection funkyness here.
#if defined(FM_WIN)
	return new nvidia_gpu;
#elif (FM_LINUX)
	return new nvml_wrapper;
#endif
	return nullptr;
}

#ifndef NVAPI_HPP
#define NVAPI_HPP

#include "nvapi.h"

#include "gpu_api.hpp"

class nvidia_gpu final : public gpu_api
{
public:
	nvidia_gpu();
	~nvidia_gpu() override;

	int gpu_count() override;

	// The core temp of the GPU in degrees
	int gpu_core_temp(int gpu_id) override;

private:
	NvU32 m_gpu_count;
	NvPhysicalGpuHandle m_handles[NVAPI_MAX_PHYSICAL_GPUS];
};

#endif // NVAPI_HPP

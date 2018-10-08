#include "nvapi_wrapper.hpp"

#include "nvapi.h"

nvidia_gpu::nvidia_gpu()
{
	NvAPI_Initialize();

	NvAPI_EnumPhysicalGPUs(m_handles, &m_gpu_count);
}

nvidia_gpu::~nvidia_gpu()
{
	NvAPI_Unload();
}

int nvidia_gpu::gpu_count()
{
	return (int) m_gpu_count;
}

int nvidia_gpu::gpu_core_temp(int gpu_id)
{
	NV_GPU_THERMAL_SETTINGS settings;
	settings.version = NV_GPU_THERMAL_SETTINGS_VER;

	NvAPI_GPU_GetThermalSettings(m_handles[gpu_id],
								 NVAPI_THERMAL_TARGET_NONE,
								 &settings);
	return settings.sensor[0].currentTemp;
}

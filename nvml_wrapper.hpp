#ifndef NVML_WRAPPER_HPP
#define NVML_WRAPPER_HPP

#include "gpu_api.hpp"
#include <vector>
#include "nvml.h"

struct nvidia_func_ptrs;

class nvml_wrapper final : public gpu_api
{
public:
	nvml_wrapper();
	~nvml_wrapper() override;

	int gpu_count() override;

	int gpu_core_temp(int gpu_id) override;
private:

	bool nvml_so_init();

	int m_gpu_count;
	std::vector<nvmlDevice_t> m_devices;

	nvidia_func_ptrs *m_ptrs;
};

#endif // NVML_WRAPPER_HPP

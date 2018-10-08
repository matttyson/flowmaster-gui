#ifndef GPU_API_HPP
#define GPU_API_HPP

class gpu_api {
public:
	gpu_api();
	virtual ~gpu_api();
	// The number of GPUs in the system.
	virtual int gpu_count() = 0;
	// The temperature of the GPU core, in celcius.
	virtual int gpu_core_temp(int gpu_id) = 0;

	// perform autodetection and creation of the object
	static gpu_api* create_instance();
};

#endif // GPU_API_HPP

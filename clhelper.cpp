#include "clhelper.h"

using namespace std;

void printPlatform(int i, cl::Platform& p)
{

	cout << "OpenCL version: " << p.getInfo<CL_PLATFORM_VERSION>() << endl;

}

void printDevice(int i, cl::Device& d)
{
	cout << "Device #" << i << endl;
	cout << "Name: " << d.getInfo<CL_DEVICE_NAME>() << endl;
	cout << "Type: " << d.getInfo<CL_DEVICE_TYPE>();
	cout << " (GPU = " << CL_DEVICE_TYPE_GPU << ", CPU = " << CL_DEVICE_TYPE_CPU << ")" << endl;
	cout << "Vendor: " << d.getInfo<CL_DEVICE_VENDOR>() << endl;
	cout << "Max Compute Units: " << d.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>() << endl;
	cout << "Global Memory: " << d.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>()/(1024*1024) << " MByte" << endl;
	cout << "Max Clock Frequency: " << d.getInfo<CL_DEVICE_MAX_CLOCK_FREQUENCY>() << endl;
	cout << "Max Allocateable Memory: " << d.getInfo<CL_DEVICE_MAX_MEM_ALLOC_SIZE>()/(1024*1024) << " MByte" << endl;
	cout << "Local Memory: " << d.getInfo<CL_DEVICE_LOCAL_MEM_SIZE>()/1024 << " KByte" << endl;
	cout << "Available: " << d.getInfo< CL_DEVICE_AVAILABLE>() << endl;
}

bool initOpenCL(cl::Platform& plat, cl::Device& dev, cl::Context& con, cl::Program& prog, cl::CommandQueue& q, ifstream& sourcefile, int platid, int devid)
{
	vector<cl::Platform> platforms; 
	cl::Platform::get(&platforms);
	
	for (int i = 0; i < platforms.size(); i++)
		printPlatform(i, platforms[i]);
	
	
	if (platforms.size() == 1)
		platid = 0;
	else {
		while (!(platid >= 0 && platid < platforms.size())) {
			cout << "Platform choice: ";
			cin >> platid;
			cout << endl;
		}
	}
	cout << endl;
	plat = platforms[platid];
	
	if (plat() == 0)
		throw runtime_error("No OpenCL 2.0 platform found");
	
	vector<cl::Device> devices;
	plat.getDevices(CL_DEVICE_TYPE_ALL, &devices);
	if (devices.size() == 0)
		throw runtime_error("No devices found");
	for (int i = 0; i < devices.size(); i++)
		printDevice(i, devices[i]);
	
	
	if (devices.size() == 1)
		devid = 0;
	else {
		while (!(devid >= 0 && devid < devices.size())) {
			cout << "Device choice: ";
			cin >> devid;
			cout << endl;
		}
	}
	cout << endl;
	dev = devices[devid];

	con = cl::Context({dev});

	cl::Program::Sources sources;

	string sourcecode(istreambuf_iterator<char>(sourcefile), (istreambuf_iterator<char>()));
	sources.push_back({ sourcecode.c_str(), sourcecode.length() });
	
	prog = cl::Program (con, sources);

	try {
		prog.build({dev}, "-g -cl-opt-disable");
	} catch (...) {
		cout << "Error while building: " << prog.getBuildInfo<CL_PROGRAM_BUILD_LOG>(dev) << endl;
		return false;
    }
	cout << "Compiled the kernel" << endl;

	q = cl::CommandQueue (con, dev);
	
	return true;	   
}

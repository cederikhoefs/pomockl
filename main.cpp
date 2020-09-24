
#include "main.h"
#include "clhelper.h"

using namespace std;

cl::Platform platform;
cl::Device device;
cl::Context context;
cl::Program program;
cl::CommandQueue queue;


void test(int N = 10){

	cl::Kernel testkernel(program, "testkernel");

	int Ah[N];
	int Bh[N];
	int Ch[N];

 	for (int i = 0; i < N; i++) {
 		Ah[i] = rand();
        Bh[i] = rand();
        Ch[i] = 0;
    }

	cl::Buffer Ad(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, N * sizeof(int), Ah);
	cl::Buffer Bd(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, N * sizeof(int), Bh);
	cl::Buffer Cd(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, N * sizeof(int), Ch);

	testkernel.setArg(0, Ad);
	testkernel.setArg(1, Bd);
	testkernel.setArg(2, Cd);

	queue.enqueueNDRangeKernel(testkernel, cl::NullRange, cl::NDRange(N), cl::NullRange);
	queue.finish();

	queue.enqueueReadBuffer(Cd, true, 0, N * sizeof(int), Ch);

	for (int i = 0; i < N; i++) {
 		cout << Ah[i] << " + " << Bh[i] << " = " << Ch[i] << endl;
    }

}


int main(int argc, char* argv[])
{

	ifstream sourcefile("kernel.cl");

	if (initOpenCL(platform, device, context, program, queue, sourcefile))
		cout << "Inited OpenCL successfully!" << endl;
	else {
		cout << "Could not init OpenCL" << endl;
		return -1;
	}
	
	test();
	
	return 0;
}


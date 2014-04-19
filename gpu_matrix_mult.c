#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <OpenCL/opencl.h>

////////////////////////////////////////////////////////////////////////////////

// Use a static data size for simplicity
//
#define DATA_SIZE (512)

////////////////////////////////////////////////////////////////////////////////

// Simple compute kernel which computes the square of an input array 
//
const char *KernelSource = "\n" \
"__kernel void square(                                                       \n" \
"   __global float*mat_gpu1,                                             \n" \
"   __global float* mat_gpu2,                                             \n" \
"   __global float* output_gpu,                                             \n" \
"   int count)                                           \n" \
"{                                                                      \n" \
"   int i = get_global_id(0);                                           \n" \
"   int j = get_global_id(1);                                           \n" \
"   if(i < count){                                                      \n" \
"for ( int k = 0; k < count; k++)                                       \n"\
"{                                                                     \n"\
"float value=0;                                                        \n"\
"for(int m=0;m<count;m++){                                             \n"\
"value = value + mat_gpu1[k + j * count] * mat_gpu2[k*count + i];      \n"\
"}                                                                     \n"\
"output_gpu[k+ count * i] = value;                                    \n"\
"}                                                                     \n"\
"}                                                                      \n"\
"}                                                                      \n" \
"\n";

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
    cl_int err;                            // error code returned from api calls
    printf("jhhjbjhbjbjhbjhb");
    float mat1[DATA_SIZE][DATA_SIZE];
    float mat2[DATA_SIZE][DATA_SIZE];
    float mult[DATA_SIZE][DATA_SIZE];
    float data[DATA_SIZE];              // original data set given to device
    float results[DATA_SIZE];           // results returned from device
    unsigned int correct;               // number of correct results returned

    size_t global=DATA_SIZE;
    size_t local;
    cl_device_id device_id = NULL;             // compute device id
    cl_context context = NULL;                 // compute context
    cl_command_queue commands = NULL;          // compute command queue
    cl_program program=NULL;                 // compute program
    cl_kernel kernel=NULL;                   // compute kernel
  
    cl_mem input = NULL;                       // device memory used for the input array
    cl_mem output= NULL;                      // device memory used for the output array
    cl_mem mat_gpu1=NULL;
    cl_mem mat_gpu2=NULL;
    cl_mem output_gpu=NULL;
    cl_int res;
    cl_platform_id platform_id = NULL;
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    

    
    // Fill our data set with random float values
    //
    int i = 0;
    unsigned int count = DATA_SIZE;
    for(i = 0; i < count; i++){
     data[i] = rand() / (float)RAND_MAX;
       // printf("the output is '%f'",data[i]);

    }
    
    
    for (int k=0; k<DATA_SIZE; k++) {
        for (int j=0; j<DATA_SIZE; j++) {
            mat1[k][j]= rand() / (float)RAND_MAX;
            mat2[k][j]= rand() / (float)RAND_MAX;
                      }
    }
 
    // Connect to a compute device
    //
    int gpu = 1;
    res = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    err = clGetDeviceIDs(platform_id, gpu ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU, 1, &device_id,&ret_num_devices);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to create a device group!\n");
        return EXIT_FAILURE;
    }
  
    // Create a compute context 
    //
 
    context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &err);
    if (!context)
    {
        printf("Error: Failed to create a compute context!\n");
        return EXIT_FAILURE;
    }
    // Create a command commands
    //
    commands = clCreateCommandQueue(context, device_id, 0, &err);
    if (!commands)
    {
        printf("Error: Failed to create a command commands!\n");
        return EXIT_FAILURE;
    }

    // Create the compute program from the source buffer
    //

    program = clCreateProgramWithSource(context, 1, (const char **) & KernelSource,NULL, &err);
    if (!program)
    {
        printf("Error: Failed to create compute program!\n");
        return EXIT_FAILURE;
    }

    // Build the program executable
    //
    err = clBuildProgram(program, 0,NULL, NULL, NULL, NULL);
    if (err != CL_SUCCESS)
    {
       size_t len;
        char buffer[DATA_SIZE*DATA_SIZE];

        printf("Error: Failed to build program executable!\n");
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        printf("%s\n", buffer);
        exit(1);
        
    }

    // Create the compute kernel in the program we wish to run
    //
    kernel = clCreateKernel(program, "square", &err);
    if (!kernel || err != CL_SUCCESS)
    {
        printf("Error: Failed to create compute kernel!\n");
        exit(1);
    }

    // Create the input and output arrays in device memory for our calculation
    //


     mat_gpu1 = clCreateBuffer(context,  CL_MEM_READ_WRITE,  sizeof(float) *count*count, NULL, &err);
     mat_gpu2 = clCreateBuffer(context,  CL_MEM_READ_WRITE,  sizeof(float) *count*count, NULL, &err);
    output_gpu = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * count*count, NULL, &err);
    if (!mat_gpu1 || !mat_gpu2)
    {
        printf("Error: Failed to allocate device memory!\n");
        exit(1);
    }    
    
    // Write our data set into the input array in device memory 
    //
    

    err = clEnqueueWriteBuffer(commands, mat_gpu1, CL_TRUE, 0, sizeof(float) * count*count, mat1, 0, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to write to source array!\n");
        exit(1);
    }
    err = clEnqueueWriteBuffer(commands, mat_gpu2, CL_TRUE, 0, sizeof(float) * count*count, mat2, 0, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to write to source array!\n");
        exit(1);
    }

    // Set the arguments to our compute kernel
    //
    err = 0;
 
    err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &mat_gpu1);
    err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &mat_gpu2);
    err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &output_gpu);
    err |= clSetKernelArg(kernel, 3, sizeof(unsigned int), &count);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to set kernel arguments! %d\n", err);
        exit(1);
    }

    // Get the maximum work group size for executing the kernel on the device
    //
    
    size_t globalThreads[2] = {DATA_SIZE,DATA_SIZE};
    size_t localThreads[2] = {DATA_SIZE,DATA_SIZE};
    err = clGetKernelWorkGroupInfo(kernel, device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to retrieve kernel work group info! %d\n", err);
        exit(1);
    }
   

    // Execute the kernel over the entire range of our 1d input data set
    // using the maximum number of work group items for this device
    //
   // err=  CL_DEVICE_MAX_WORK_GROUP_SIZE;
    //printf("the max number of dimension'%d'",err);
    err = clEnqueueNDRangeKernel(commands, kernel, 1, NULL, &global, &local, NULL, 0, NULL);
    if (err)
    {
        printf("Error: Failed to execute kernel!\n '%d'",err);
        return EXIT_FAILURE;
    }

    // Wait for the command commands to get serviced before reading back results
    //
    clFinish(commands);

    // Read back the results from the device to verify the output
    //
    
    err = clEnqueueReadBuffer( commands, output_gpu, CL_TRUE, 0, sizeof(float) * count*count,mult, 0, NULL, NULL );
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to read output array! %d\n", err);
        exit(1);
    }
    
    // Validate our results
    //
    correct = 0;
    for(i = 0; i < count; i++)
    {
        for (int j=0; j<count; j++) {
            printf("the output is '%f'\n",mult[i][j]);
        }
        

    }

    
    // Shutdown and cleanup
    //
    clReleaseMemObject(input);
    clReleaseMemObject(output);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(commands);
    clReleaseContext(context);

    return 0;
}

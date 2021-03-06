/*
-- (c) Copyright 2019 Xilinx, Inc. All rights reserved.
--
-- This file contains confidential and proprietary information
-- of Xilinx, Inc. and is protected under U.S. and
-- international copyright and other intellectual property
-- laws.
--
-- DISCLAIMER
-- This disclaimer is not a license and does not grant any
-- rights to the materials distributed herewith. Except as
-- otherwise provided in a valid license issued to you by
-- Xilinx, and to the maximum extent permitted by applicable
-- law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
-- WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
-- AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
-- BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
-- INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
-- (2) Xilinx shall not be liable (whether in contract or tort,
-- including negligence, or under any other theory of
-- liability) for any loss or damage of any kind or nature
-- related to, arising under or in connection with these
-- materials, including for any direct, or any indirect,
-- special, incidental, or consequential loss or damage
-- (including loss of data, profits, goodwill, or any type of
-- loss or damage suffered as a result of any action brought
-- by a third party) even if such damage or loss was
-- reasonably foreseeable or Xilinx had been advised of the
-- possibility of the same.
--
-- CRITICAL APPLICATIONS
-- Xilinx products are not designed or intended to be fail-
-- safe, or for use in any application requiring fail-safe
-- performance, such as life-support or safety devices or
-- systems, Class III medical devices, nuclear facilities,
-- applications related to the deployment of airbags, or any
-- other applications that could lead to death, personal
-- injury, or severe property or environmental damage
-- (individually and collectively, "Critical
-- Applications"). Customer assumes the sole risk and
-- liability of any use of Xilinx products in Critical
-- Applications, subject only to applicable laws and
-- regulations governing limitations on product liability.
--
-- THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
-- PART OF THIS FILE AT ALL TIMES.
*/

#ifndef _N2CUBE_H_
#define _N2CUBE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define DNNDK_VERSION    "3.1"
#define N2Cube_VERSION   "3.0"

/* DPU Task runtime mode definitions */

/* Task in normal mode (defaul mode) */
#define T_MODE_NORMAL        (0)

/* Task in profiling mode in order to collect performance stastics for each DPU Node */
#define T_MODE_PROFILE       (1<<0)

/* Task in debug mode in order to dump each Node's Code/Bias/Weights/Input/Output raw data for debugging */
#define T_MODE_DEBUG         (1<<1)

/* Exported data structures of DPU Kernel/Task/Tensor */
struct  dpu_kernel;
struct  dpu_task;
struct  task_tensor;

typedef struct dpu_kernel  DPUKernel;
typedef struct dpu_task    DPUTask;
typedef struct task_tensor DPUTensor;

/* The exception handling mode */
#define N2CUBE_EXCEPTION_MODE_PRINT_AND_EXIT 0
#define N2CUBE_EXCEPTION_MODE_RET_ERR_CODE 1
int dpuSetExceptionMode(int mode);
int dpuGetExceptionMode();
const char *dpuGetExceptionMessage(int error_code);

/* Open & initialize the usage of DPU device */
int dpuOpen();

/* Close & finalize the usage of DPU device */
int dpuClose();

#ifdef __cplusplus
}
#endif

/* Load a DPU Kernel and allocate DPU memory space for
   its Code/Weight/Bias segments */
DPUKernel *dpuLoadKernel(const char *netName);

/* Set mean values for DPU Kernel */
int dpuSetKernelMeanValue(DPUKernel *kernel, int mean1, int mean2, int mean3);

/* Destroy a DPU Kernel and release its associated resources */
int dpuDestroyKernel(DPUKernel *kernel);

/* Instantiate a DPU Task from one DPU Kernel, allocate its private
   working memory buffer and prepare for its execution context */
DPUTask *dpuCreateTask(DPUKernel *kernel, int mode);

/* Launch the running of DPU Task */
int dpuRunTask(DPUTask *task);

/* Remove a DPU Task, release its working memory buffer and destroy
   associated execution context */
int dpuDestroyTask(DPUTask *task);

/* Enable dump facility of DPU Task while running for debugging purpose */
int dpuEnableTaskDebug(DPUTask *task);

/* Enable profiling facility of DPU Task while running to get its performance metrics */
int dpuEnableTaskProfile(DPUTask *task);

/* Get the execution time of DPU Task */
long long dpuGetTaskProfile(DPUTask *task);

/* Get the execution time of DPU Node */
long long dpuGetNodeProfile(DPUTask *task, const char*nodeName);


/*
 * API for both single IO and multiple IO.
 * For multiply IO, should specify the input/output tensor idx.
 */

/* Get total number of input Tensor of DPU Task */
int dpuGetInputTensorCnt(DPUTask * task, const char * nodeName);

/* Get input Tensor of DPU Task */
DPUTensor* dpuGetInputTensor(DPUTask *task, const char*nodeName, int idx = 0);

/* Get the start address of DPU Task's input Tensor */
int8_t* dpuGetInputTensorAddress(DPUTask *task, const char *nodeName, int idx = 0);

/* Get the size (in byte) of one DPU Task's input Tensor */
int dpuGetInputTensorSize(DPUTask *task, const char *nodeName, int idx = 0);

/* Get the scale value (DPU INT8 quantization) of one DPU Task's input Tensor */
float dpuGetInputTensorScale(DPUTask *task, const char *nodeName, int idx = 0);

/* Get the height dimension of one DPU Task's input Tensor */
int dpuGetInputTensorHeight(DPUTask *task, const char *nodeName, int idx = 0);

/* Get the width dimension of one DPU Task's input Tensor */
int dpuGetInputTensorWidth(DPUTask *task, const char *nodeName, int idx = 0);

/* Get the channel dimension of one DPU Task's input Tensor */
int dpuGetInputTensorChannel(DPUTask *task, const char *nodeName, int idx = 0);

/* Get total number of output Tensor of DPU Task */
int dpuGetOutputTensorCnt(DPUTask * task, const char * nodeName);

/* Get output Tensor of one DPU Task */
DPUTensor* dpuGetOutputTensor(DPUTask *task, const char *nodeName, int idx = 0);

/* Get the start address of one DPU Task's output Tensor */
int8_t* dpuGetOutputTensorAddress(DPUTask *task, const char *nodeName, int idx = 0);

/* Get the size (in byte) of one DPU Task's output Tensor */
int dpuGetOutputTensorSize(DPUTask *task, const char *nodeName, int idx = 0);

/* Get the scale value (DPU INT8 quantization) of one DPU Task's output Tensor */
float dpuGetOutputTensorScale(DPUTask *task, const char *nodeName, int idx = 0);

/* Get the height dimension of one DPU Task's output Tensor */
int dpuGetOutputTensorHeight(DPUTask *task, const char *nodeName, int idx = 0);

/*  Get the channel dimension of one DPU Task's output Tensor */
int dpuGetOutputTensorWidth(DPUTask *task, const char *nodeName, int idx = 0);

/* Get DPU Node's output tensor's channel */
int dpuGetOutputTensorChannel(DPUTask *task, const char *nodeName, int idx = 0);

/* Set DPU Task's input Tensor with data stored under Caffe
   Blob's order (channel/height/width) in INT8 format */
int dpuSetInputTensorInCHWInt8(DPUTask *task, const char *nodeName, int8_t *data, int size, int idx = 0);

/* Set DPU Task's input Tensor with data stored under Caffe
   Blob's order (channel/height/width) in FP32 format */
int dpuSetInputTensorInCHWFP32(DPUTask *task, const char *nodeName, float *data, int size, int idx = 0);

/* Set DPU Task's input Tensor with data stored under DPU
   Tensor's order (height/width/channel) in INT8 format */
int dpuSetInputTensorInHWCInt8(DPUTask *task, const char *nodeName, int8_t *data, int size, int idx = 0);

/* Set DPU Task's input Tensor with data stored under DPU
   Tensor's order (height/width/channel) in FP32 format */
int dpuSetInputTensorInHWCFP32(DPUTask *task, const char *nodeName, float *data, int size, int idx = 0);

/* Get DPU Task's output Tensor and store them under Caffe
   Blob's order (channel/height/width) in INT8 format */
int dpuGetOutputTensorInCHWInt8(DPUTask *task, const char *nodeName, int8_t *data, int size, int idx = 0);

/* Get DPU Task's output Tensor and store them under Caffe
   Blob's order (channel/height/width) in FP32 format */
int dpuGetOutputTensorInCHWFP32(DPUTask *task, const char *nodeName, float *data, int size, int idx = 0);

/* Get DPU Task's output Tensor and store them under DPU
   Tensor's order (height/width/channel) in INT8 format */
int dpuGetOutputTensorInHWCInt8(DPUTask *task, const char *nodeName, int8_t *data, int size, int idx = 0);

/* Get DPU Task's output Tensor and store them under DPU
   Tensor's order (height/width/channel) in FP32 format */
int dpuGetOutputTensorInHWCFP32(DPUTask *task, const char *nodeName, float *buffer, int size, int idx = 0);


/* Get the size of one DPU Tensor */
int dpuGetTensorSize(DPUTensor* tensor);

/* Get the start address of one DPU Tensor */
int8_t* dpuGetTensorAddress(DPUTensor* tensor);

/* Get the scale value of one DPU Tensor */
float dpuGetTensorScale(DPUTensor* tensor);

/* Get the height dimension of one DPU Tensor */
int dpuGetTensorHeight(DPUTensor* tensor);

/* Get the width dimension of one DPU Tensor */
int dpuGetTensorWidth(DPUTensor* tensor);

/* Get the channel dimension of one DPU Tensor */
int dpuGetTensorChannel(DPUTensor* tensor);

/* Compute softmax */
int dpuRunSoftmax(int8_t *input, float *output, int numClasses, int batchSize, float scale);

#endif

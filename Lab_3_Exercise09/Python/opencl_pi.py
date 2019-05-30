#
# This program will numerically compute the integral of
#
#                4/(1+x*x)
#
# from 0 to 1.  The value of this integral is pi -- which
# is great since it gives us an easy way to check the answer.
#
# This the original sequential program.
#
# History: Written in C by Tim Mattson, 11/99
#          Ported to Python by Tom Deakin, July 2013


# Import the Python OpenCL API
import pyopencl as cl
# Import the Python Maths Library (for vectors)
import numpy

# Import a library to print out the device information
import deviceinfo
# Import Standard Library to time the execution
from time import time

LENGTH = 1024


kernelsource = """
__kernel void pi(
    __global float* integrals)
	{
		int num_steps = 1000000;

		double step = 1.0/num_steps;

    	int i = get_global_id(0);
    	
	    double x = (i - 0.5) * step;
    	integrals[i] = 4.0 / (1.0 + x * x);
   	}"""


context = cl.create_some_context()

# Print out device info
deviceinfo.output_device_info(context.devices[0])

# Create a command queue
queue = cl.CommandQueue(context)

# Create the compute program from the source buffer
# and build it
program = cl.Program(context, kernelsource).build()

#Create empty vector of h_i that will be used to store the output
h_i = numpy.empty(LENGTH).astype(numpy.float32)

# Create the output (i) array in device memory
d_i = cl.Buffer(context, cl.mem_flags.READ_WRITE, h_i.nbytes)


rtime = time()

# Execute the kernel over the entire range of our 1d input
# allowing OpenCL runtime to select the work group items for the device
pi = program.pi
pi.set_scalar_arg_dtypes([None])
pi(queue, h_i.shape, None, d_i, LENGTH)
queue.finish()
rtime = time() - rtime
print "The kernel ran in", rtime, "seconds"

cl.enqueue_copy(queue, h_i, d_i)


num_steps = 1000000
print "Doing", num_steps, "steps instead."

step = 1.0/num_steps

integral_sum = 0.0
#Need to verify that "iterations" is indeed correct
for i in zip(h_i):
    # assign element i of a+b to tmp
    integral_sum += i
    # compute the deviation of expected and output result
    

pi = step * integral_sum


print("Pi is " + str(pi))
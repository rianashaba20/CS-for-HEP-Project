# Project Description 
Create a virtual machine on Google Cloud with the following characteristics:

- 2 CPUs, 4 GB of memory
- Rocky Linux 9 operating system
- 30 GB disk

Do not use a spot instance, so that the VM is guaranteed to stay online
Under “SSH keys”, add the following key:

```
ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAIO3dlLZU37NgQofgLnxoQAYS/EnC8XS7BCd5oYGNpIwW chierici@infn.it
```

On this VM, install Docker and enable the Docker service. Create the directory <$HOME/containers/IMAPP25>, which will be used as permanent storage for the following steps.

Create a Docker image starting from Ubuntu 24.04 that includes all the tools and libraries (g++, CMake, SFML, TBB, …) needed to develop, build, and test the parallel application described below. Inside the container, the directory $HOME/containers/IMAPP25 must be mounted at /workspace.

The parallel application consists of the computation of the Mandelbrot set, starting from the sequential version available at https://github.com/giacomini/cshep2025, under the project directory.

The focus of the parallelization is the two-dimensional iteration over the image pixels corresponding to the complex plane around the origin. This iteration must be transformed into a TBB parallel_for loop using a blocked_range2d. The parallel_for must use a simple_partitioner and experiment with multiple values of the grain size.

The output of the application must include: - an image of the Mandelbrot set in PNG format - a text file with two columns: the grain size and the execution time obtained with that grain size

You are free to add additional functionality and improvements, for example: counting the number of executed tasks or showing the actual grid used by the TBB scheduler to divide the data space.

Once the application is considered complete, build it in Release mode and ensure that all files are stored under the /workspace directory. Then, create a second Docker image, again starting from Ubuntu 24.04, that contains only what is necessary to run the application (for example, the compiler and CMake are not required).

Run this second image, mounting $HOME/containers/IMAPP25 at /workspace inside the container. Execute the application and produce the required output.

Verify on the Rocky Linux 9 virtual machine that the requested output is saved in $HOME/containers/IMAPP25.

The project may be carried out in groups of up to four people. Once the work is complete, the group (not individual students) must communicate the details of the virtual machine to allow the evaluation.

Leave the virtual machine (with Rocky Linux 9) running and send an email to andrea.chierici7@unibo.it and francesco.giacomini3@unibo.it containing the following information:

the public IP address of the virtual machine
the names of all contributors
You will be notified once the evaluation is complete, after which you may shut down the virtual machine.

This assignment must be delivered at least one week before the oral exam.

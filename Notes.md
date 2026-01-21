### Install and Enable Docker
When you have created your VM instance, SSH in to it and run these commands:
```js
sudo -i

dnf -y install nano wget curl git

curl https://download.docker.com/linux/centos/docker-ce.repo -o /etc/yum.repos.d/docker-ce.repo

sed -i -e "s/enabled=1/enabled=0/g" /etc/yum.repos.d/docker-ce.repo

dnf --enablerepo=docker-ce-stable -y install docker-ce

systemctl enable --now docker

To check it works: systemctl status docker 			(Press q to exit)
```
Now we create the directory: 
```js
exit								(To exit the root user)

mkdir -p $HOME/containers/IMAPP25				(-p to also automatically create the containers directory
```
Create the first Docker file ```Dockerfile_build```:
Go to ```cd containers/```, and then ```vim Dockerfile_build```.
The content of the Docker file you should write is (To quit type: ```:wq```):
```js
FROM ubuntu:24.04
ENV DEBIAN_FRONTEND=noninteractive
RUN apt update && DEBIAN_FRONTEND=noninteractive apt install -y \
		build-essential \
		g++ \
		cmake \
		libsfml-dev \
		libtbb-dev \
		git \
		vim \
		&& rm -rf /var/lib/apt/lists/*
WORKDIR /workspace
CMD ["bash"]
```
And then:
```
sudo docker build -f Dockerfile_build -t imapp25_build .
```
To run the container in the ```/workspace```:
```js
sudo docker run -it \
  -v $HOME/containers/IMAPP25:/workspace \
  -w /workspace \
  imapp25_build \
  /bin/bash
```
You should see ```root@5dbb3732d428:/workspace#```.
You can then exit by just typing ```exit```.

### First let's check our container:
```js
docker ps			
docker ps -a 
```
We should see our container's name in there and ID.
Then, let's go to the ```container directory``` where we have the ```IMAPP25 folder``` and check the content (it should be empty):
```js
cd containers/IMAPP25
ls
```
We need to upload our ```main_parallel.cpp``` and ```CMakelists.txt``` to the ```IMAPP25``` folder. For this one, just click on ```UPLOAD FILE``` button. The files will be in the home directory, and you can move them to where we want by following these commands:
```js
sudo mv ~/CMakeLists.txt ~/containers/IMAPP25/
sudo mv ~/main_parallel.cpp ~/containers/IMAPP25/
```

## Build Mode
Inside the container and ```/workspace``` type the commands (BUILD MODE):
```js
cmake -S . -B build-d -DCMAKE_BUILD_TYPE=Debug
cmake --build build-d
./build-d/mandelbrot_par
```
If you exit the container by just writing the command ```exit```, you can go back to the ```cd containers/IMAPP25```and then ```ls``` to check that the output is now there.


## Release Mode
Go inside the ```workspace``` in the container as before and simply run the following commands:
```js
cmake -S . -B build-o -DCMAKE_BUILD_TYPE=Release
cmake --build build-o
```
After that we have to create a new Dockerfile for the release mode. Go to the ```containers```folder and run the commands:
```js
cd containers/
sudo vim Dockerfile_Release
```
The content of the ```Docker_release``` file you should write is:
```js
FROM ubuntu:24.04
ENV DEBIAN_FRONTEND=noninteractive
RUN apt update && DEBIAN_FRONTEND=noninteractive apt install -y \
        libtbb12 \
        libsfml-graphics2.6
WORKDIR /workspace
CMD ["bash"]
```
Type ```:wq``` to write and quit. Then:
```js
sudo docker build -f Dockerfile_release -t imapp25_release .
```
Now we will run the new container ```ìmapp25_release``` and test the compiled program in it.
To run the new container:
```js
sudo docker run -it \
  -v $HOME/containers/IMAPP25:/workspace \
  -w /workspace \
  imapp25_release \
  /bin/bash
```
Then run the ```mandelbrot_par``` executable in release mode (build-o):
```js
./build-o/mandelbrot_par
```
To check the output, stay inside the ```workspace``` and simply type ```ls -la```. All should be there.

Now exit the container and go back to ```cd containers/IMAPP25```. We will download the files.
First, make a zip file for the output with the following command:
```js
tar -czf output_results.tar.gz *.png results.csv
tar -czf builds.tar.gz build-d build-o
```
Then to download, simply click on ```DOWNLOAD FILE``` by entering the **full path** (which in my case)
```js
/home/anashaba20/containers/IMAPP25/output_results.tar.gz
```
Download the code file and docker files by simply replacing the name in the full path.

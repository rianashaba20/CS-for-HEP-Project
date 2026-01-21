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



ACCORDING TO THE PROJECT DESCRIPTION WE NOW HAVE TO CREATE THE DIRECTORY. 

exit								(To exit the root user)

mkdir -p $HOME/containers/IMAPP25				(-p to also automatically create the containers directory



NOW CREATE THE DOCKER INSTANCE WITH ALL THE REQUIRED PACKAGES AND DEPENDENCIES

docker pull ubuntu


docker run ubuntu /bin/bash -c "apt update; apt -y install g++ cmake libsfml-dev libtbb-dev"			(To install the packages and dependencies)


docker ps -a													(Take the container ID of the most recent instance)


docker commit <the ID you just copied without the brackets> imapp25_build


docker images													(You should now see two images: ubuntu and imapp25_build)

docker run -v $HOME/containers/IMAPP25/:/workspace -i -t imapp25_build /bin/bash			(This couples the IMAPP25 folder to a new workspace folder)
```

Then, to get the output in **Build** mode, I tested it locally. I installed Docker, followed the same initial steps as in the beginning in the SSH connection to create the directory and the image. But I also had to install the Dockerfile from the SSH connection window at **Download File** button (here you have to insert your path: /home/<your username>/containers).  
The commands I used after I connected to the VS Code were simply the following (BUILD MODE):
```js
cmake -S . -B build-d -DCMAKE_BUILD_TYPE=Debug
cmake --build build-d
./build-d/mandelbrot_par
```
## Working the same thing but on the VM SSH connection 
First let's check our container:
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
Now we are ready to run the container:
```js
docker run -v $HOME/containers/IMAPP25/:/workspace -w /workspace -i -t imapp25-dev /bin/bash
```
You should see: ```root@28b91510e2f3:/workspace#```
Run the following commands for **BUILD MODE**:
```js
cmake -S . -B build-d -DCMAKE_BUILD_TYPE=Debug
cmake --build build-d
./build-d/mandelbrot_par
```
If you exit the container by just writing the command ```exit```, you can go back to the ```cd containers/IMAPP25```and then ```ls``` to check that the output is now there.

### Release Mode
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
The content of the ```Docker_Release``` file you should write is:
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
sudo docker build -f Dockerfile_Release -t imapp25-release .
```
Now we will run the new container ``ìmapp25-release``` and test the compiled program in it.
To run the new container:
```js
docker run -v $HOME/containers/IMAPP25:/workspace -i -t imapp25-release /bin/bash
```
Then run the ```mandelbrot_par``` executable in release mode (build-o):
```js
./build-o/mandelbrot_par
```
To check the output, stay inside the ```workspace``` and simply type ```ls -la```. All should be there.

Now exit the container and go back to ```cd containers/IMAPP25```. We will download the files.
First, make a zip file for the output with the following command:
```js
tar -czf output_results.tar.gz * .png results.csv
```
Then to download, simply click on ```DOWNLOAD FILE``` by entering the **full path** (which in my case)
```js
/home/anashaba20/containers/IMAPP25/output_results.tar.gz
```
Download the code file and docker files by simply replacing the name in the full path.

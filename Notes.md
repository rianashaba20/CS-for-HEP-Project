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


docker commit <the ID you just copied without the brackets> ubuntu_with_req_packages


docker images													(You should now see two images: ubuntu and ubuntu_with_req_packages)

docker run -v $HOME/containers/IMAPP25/:/workspace -i -t ubuntu_with_req_packages /bin/bash			(This couples the IMAPP25 folder to a new workspace folder)
```
Next we need to create Dockerfile.

For this, you need to exit the container, and go to the containers directory. Follow the steps below.

```js
cd containers/
vim Dockerfile
```
Then, you have to write on the text file, simply copy and paste the following:
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
Inside the containers directory, build the image with the following command (name is imapp25-dev):
```js
sudo docker build -t imapp25-dev .
```
Then, to get the output in **Build** mode, I tested it locally. I installed Docker, followed the same initial steps as in the beginning in the SSH connection to create the directory and the image. But I also had to install the Dockerfile from the SSH connection window at **Download File** button (here you have to insert your path: /home/<your username>/containers).  
The commands I used after I connected to the VS Code were simply the following (BUILD MODE):
```js
cmake -S . -B build-d -DCMAKE_BUILD_TYPE=Debug
cmake --build build-d
./build-d/mandelbrot_par
```

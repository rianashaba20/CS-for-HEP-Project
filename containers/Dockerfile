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

FROM ubuntu:latest

# Install dependencies
RUN apt-get update && \
    apt-get install -y \
    build-essential \
    cmake \
    git \
    python3 \
    python3-pip \
    gcc \
    wget \
    make \
    libncurses-dev \
    flex \
    bison \
    gperf \
    mingw-w64 \
    ninja-build \
    gdb \
    jq \
    clang-format \
    net-tools \
    iproute2

# Create an alias for python3
RUN ln -s /usr/bin/python3 /usr/bin/python

# Download and setup ESP8266 toolchain
WORKDIR /home/esp
RUN wget https://dl.espressif.com/dl/xtensa-lx106-elf-gcc8_4_0-esp-2020r3-linux-amd64.tar.gz && \
    tar -xzf xtensa-lx106-elf-gcc8_4_0-esp-2020r3-linux-amd64.tar.gz && \
    rm xtensa-lx106-elf-gcc8_4_0-esp-2020r3-linux-amd64.tar.gz
ENV PATH="/home/esp/xtensa-lx106-elf/bin:${PATH}"
RUN echo "alias get_lx106='export PATH=\$PATH:/home/esp/xtensa-lx106-elf/bin'" >> /etc/bash.bashrc
# Get ESP8266_RTOS_SDK
ENV ESP8266_RTOS_SDK="/home/esp/ESP8266_RTOS_SDK"
RUN git clone -b fix-libcoap-404 --recursive https://github.com/viordash/ESP8266_RTOS_SDK.git ${ESP8266_RTOS_SDK}
RUN echo "export ESP8266_RTOS_SDK=${ESP8266_RTOS_SDK}" >> /etc/bash.bashrc
# Install the Required Python Packages
RUN pip3 install --break-system-packages -r ${ESP8266_RTOS_SDK}/requirements.txt

# Expose the udp and tcp ports used by the application
EXPOSE 9999/udp
EXPOSE 8888/tcp

# Set the working directory
WORKDIR /workspace
# Copy the local files to the container
COPY . /workspace

# Set the default command to run when the container starts
CMD ["bash"]

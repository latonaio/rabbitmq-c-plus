FROM ubuntu:xenial as builder

RUN apt-get update \
  && apt-get install -y git \
                        g++ \
                        make \
                        openssl \
                        libssl-dev \
                        libblkid-dev e2fslibs-dev libboost-all-dev libaudit-dev \
                        wget vim

ARG RABBITMQ_HOST_IP
ARG RABBITMQ_USER
ARG RABBITMQ_PASSWORD
ARG RABBITMQ_PORT
ARG RABBITMQ_VIRTUAL_HOST
ARG RABBITMQ_QUEUE_FROM

ENV RABBITMQ_HOST_IP=$RABBITMQ_HOST_IP
ENV RABBITMQ_USER=$RABBITMQ_USER
ENV RABBITMQ_PASSWORD=$RABBITMQ_PASSWORD
ENV RABBITMQ_PORT=$RABBITMQ_PORT
ENV RABBITMQ_VIRTUAL_HOST=$RABBITMQ_VIRTUAL_HOST
ENV RABBITMQ_QUEUE_FROM=$RABBITMQ_QUEUE_FROM

RUN wget https://cmake.org/files/v3.26/cmake-3.26.5.tar.gz
RUN tar -xzvf cmake-3.26.5.tar.gz
RUN cd cmake-3.26.5 && \
    ./bootstrap && \
    make -j$(nproc) && \
    make install

RUN \
	rm -rf rabbitmq-c && \
	git clone https://github.com/alanxz/rabbitmq-c.git && \
	cd rabbitmq-c && \
	git checkout -b v0.8.0 && \
	mkdir -p build && \
	cd build && cmake -DENABLE_SSL_SUPPORT=OFF .. && \
	cmake --build . && \
	cmake -DCMAKE_INSTALL_PREFIX=/usr/local .. && \
	make && make install && \
	cd / && rm -rf rabbitmq-c

RUN \
  rm -rf SimpleAmqpClient && \
  git clone https://github.com/alanxz/SimpleAmqpClient.git && \
  cd SimpleAmqpClient && \
  mkdir -p simpleamqpclient-build2 && \
  cd simpleamqpclient-build2 && cmake .. && \
  cmake --build . && \
  cmake -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX=/usr/local .. && \
  make && make install && \
  cd / && rm -rf SimpleAmqpClient

RUN cd /

COPY . /app
WORKDIR /app

RUN mkdir -p build && \
        cd build && \
        cmake .. && \
        cmake --build . && cp json_parser_c_plus ../ && cd ..

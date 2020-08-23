FROM ubuntu:18.04

RUN  apt-get update \
  && apt-get install -y wget gcc cmake libboost-all-dev g++ \
  && rm -rf /var/lib/apt/lists/*

WORKDIR /clu

CMD /bin/bash


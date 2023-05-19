#                                               _
#  ___ __ _ _ __ ___  _ __  _   _ ___  ___ ___ (_)_ __
# / __/ _` | '_ ` _ \| '_ \| | | / __|/ __/ _ \| | '_ \
#| (_| (_| | | | | | | |_) | |_| \__ \ (_| (_) | | | | |
# \___\__,_|_| |_| |_| .__/ \__,_|___/\___\___/|_|_| |_|
#
#
# Build ccd, cc-cli, and cc-tx from latest Github master.

FROM ubuntu:18.04
LABEL maintainer CampusCoinDevs

ENV TZ=America/New_York
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Install depends
RUN apt-get update && \
apt install -y tzdata && \
apt-get install -y --no-install-recommends \
autotools-dev \
autoconf \
build-essential \
ca-certificates \
dh-autoreconf \
git \
wget \
libboost-chrono-dev \
libboost-filesystem-dev \
libboost-program-options-dev \
libboost-system-dev \
libboost-test-dev \
libboost-thread-dev \
libevent-dev \
libssl1.0-dev \
libtool \ 
pkg-config 

# Fix for ca-certificates on older linux distros under Docker
RUN rm /etc/ssl/certs/ca-certificates.crt
RUN wget https://curl.se/ca/cacert.pem -O /etc/ssl/certs/ca-certificates.crt

# Install db4.8 from sources
ENV BERKELEYDB_VERSION=db-4.8.30.NC
ENV BERKELEYDB_PREFIX=/usr/local

RUN wget https://download.oracle.com/berkeley-db/db-4.8.30.NC.tar.gz  && tar zxvf db-4.8.30.NC.tar.gz
RUN sed s/__atomic_compare_exchange/__atomic_compare_exchange_db/g -i ${BERKELEYDB_VERSION}/dbinc/atomic.h
RUN mkdir -p ${BERKELEYDB_PREFIX}

WORKDIR /${BERKELEYDB_VERSION}/build_unix

RUN ../dist/configure --enable-cxx --disable-shared --with-pic --prefix=${BERKELEYDB_PREFIX}
RUN make -j4
RUN make install

WORKDIR /

RUN rm -rf /db-4.8.30.NC/ 
RUN rm /db-4.8.30.NC.tar.gz
RUN rm -rf ${BERKELEYDB_PREFIX}/docs

#Clone the source code.
RUN mkdir -p /root/devel && cd $_
WORKDIR /root/devel
RUN git clone https://github.com/campuscoindev/CC.git

# Build sources
RUN cd CC && ./autogen.sh && \
./configure --with-gui=no --disable-tests && \
make && \
make install && \
make clean

# Wrap it up
RUN echo "Finished building CC."

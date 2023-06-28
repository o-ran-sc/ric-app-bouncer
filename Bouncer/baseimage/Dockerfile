FROM nexus3.o-ran-sc.org:10002/o-ran-sc/bldr-ubuntu20-c-go:1.0.0 as buildenv
#FROM nexus3.o-ran-sc.org:10001/ubuntu:18.04 as buildenv
# spaces to save things in the build image to copy to final image
RUN mkdir -p /playpen/assets /playpen/src /playpen/bin
ARG SRC=.

WORKDIR /playpen

# versions we snarf from package cloud
ARG RMR_VER=4.8.1
#ARG SDL_VER=1.4.0
ARG XFCPP_VER=2.3.8

# package cloud urls for wget
ARG PC_REL_URL=https://packagecloud.io/o-ran-sc/release/packages/debian/stretch
# ARG PC_STG_URL=https://packagecloud.io/o-ran-sc/staging/packages/debian/stretch

# pull in rmr
RUN wget -nv --content-disposition ${PC_REL_URL}/rmr_${RMR_VER}_amd64.deb/download.deb && \
        wget -nv --content-disposition ${PC_REL_URL}/rmr-dev_${RMR_VER}_amd64.deb/download.deb && \
        dpkg -i rmr_${RMR_VER}_amd64.deb rmr-dev_${RMR_VER}_amd64.deb

# pull in xapp framework c++
RUN wget -nv --content-disposition ${PC_REL_URL}/ricxfcpp-dev_${XFCPP_VER}_amd64.deb/download.deb && \
        wget -nv --content-disposition ${PC_REL_URL}/ricxfcpp_${XFCPP_VER}_amd64.deb/download.deb && \
        dpkg -i ricxfcpp-dev_${XFCPP_VER}_amd64.deb ricxfcpp_${XFCPP_VER}_amd64.deb

# # snarf up SDL dependencies, then pull SDL package and install
RUN apt-get update && apt-get install -y \
        libboost-filesystem1.67.0 \
        libboost-system1.67.0 \
        libhiredis-dev \
 libhiredis0.14 \
        && apt-get clean
#RUN wget -nv --content-disposition ${PC_REL_URL}/sdl_${SDL_VER}-1_amd64.deb/download.deb && \
#        wget -nv --content-disposition ${PC_REL_URL}/sdl-dev_${SDL_VER}-1_amd64.deb/download.deb &&\
#        dpkg -i sdl-dev_${SDL_VER}-1_amd64.deb sdl_${SDL_VER}-1_amd64.deb

#RUN apt-get update && apt-get install -y cmake gcc make git g++ wget
#building cpprestsdk
RUN apt-get install -y libcpprest-dev

RUN apt-get install -y  g++ git libboost-atomic-dev libboost-thread-dev libboost-system-dev libboost-date-time-dev libboost-regex-dev libboost-filesystem-dev libboost-random-dev libboost-chrono-dev libboost-serialization-dev libwebsocketpp-dev openssl libssl-dev ninja-build zlib1g-dev

RUN git clone https://github.com/Microsoft/cpprestsdk.git casablanca && \
    cd casablanca && \
    mkdir build && \
    cd build && \
    cmake -G Ninja .. -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=OFF -DBUILD_SAMPLES=OFF -DCMAKE_INSTALL_PREFIX=/usr/local .. && \
    ninja && \
    ninja install && \
    cmake -G Ninja .. -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=0 -DBUILD_TESTS=OFF -DBUILD_SAMPLES=OFF -DCMAKE_INSTALL_PREFIX=/usr/local .. && \
    ninja && \
    ninja install && \
    rm -rf casablanca
RUN apt-get update && apt-get install -y cmake gcc make git g++ wget
#installing all dependicies for pistache
RUN apt-get update && apt-get install -y cmake gcc make \
git g++ wget meson libcurl4-openssl-dev libssl-dev pkg-config ninja-build
RUN git clone https://github.com/Tencent/rapidjson && \
      cd rapidjson && \
     mkdir build && \
     cd build && \
    cmake -DCMAKE_INSTALL_PREFIX=/usr/local .. && \
   make install && \
    cd ../../
        #rm -rf rapidjson

#building and installing pistache
RUN git clone https://github.com/pistacheio/pistache.git
RUN cd pistache && \
        meson setup build \
    --buildtype=release \
    -DPISTACHE_USE_SSL=true \
    -DPISTACHE_BUILD_EXAMPLES=true \
    -DPISTACHE_BUILD_TESTS=true \
    -DPISTACHE_BUILD_DOCS=false \
    --prefix="/usr/local"
RUN cd pistache/build && \
        ninja && \
        ninja install
RUN cp /usr/local/lib/x86_64-linux-gnu/libpistache* /usr/local/lib/
RUN cp /usr/local/lib/x86_64-linux-gnu/pkgconfig/libpistache.pc /usr/local/lib/pkgconfig

#install nlohmann json
RUN git clone https://github.com/nlohmann/json.git && cd json && cmake . && make install

#install json-schema-validator
RUN git clone https://github.com/pboettch/json-schema-validator.git && cd json-schema-validator &&mkdir build &&cd build && cmake .. && make install

COPY e2ap/ e2ap/
COPY e2sm_kpm/ e2sm_kpm/
COPY e2sm_rc/ e2sm_rc/
# "COMPILING E2AP Wrapper"
RUN cd e2ap && \
    gcc -c -fPIC -Iheaders/ lib/*.c wrapper.c && \
    gcc *.o -shared -o libe2apwrapper.so && \
    cp libe2apwrapper.so /usr/local/lib/ && \
    mkdir /usr/local/include/e2ap && \
    cp wrapper.h headers/*.h /usr/local/include/e2ap && \
    ldconfig
# "COMPILING E2SM-KPM Wrapper"
RUN cd e2sm_kpm && \
    gcc -c -fPIC -Iheaders/ lib/*.c wrapper.c && \
    gcc *.o -shared -o libe2smkpmwrapper.so&& \
    cp libe2smkpmwrapper.so /usr/local/lib/ && \
    mkdir /usr/local/include/e2sm_kpm && \
    cp wrapper.h headers/*.h /usr/local/include/e2sm_kpm && \
    ldconfig
# "COMPILING E2SM-RC Wrapper"
RUN cd e2sm_rc && \
    gcc -c -fPIC -Iheaders/ lib/*.c wrapper.c && \
    gcc *.o -shared -o libe2smrcwrapper.so&& \
    cp libe2smrcwrapper.so /usr/local/lib/ && \
    mkdir /usr/local/include/e2sm_rc && \
    cp wrapper.h headers/*.h /usr/local/include/e2sm_rc && \
    ldconfig
#...
#FROM ubuntu:20.04

#COPY --from=buildenv /usr/local/lib /usr/local/lib/
#COPY --from=buildenv /usr/local/include /usr/local/include
#COPY --from=buildenv /usr/local/bin/rmr_probe /usr/local/bin/
#COPY --from=buildenv /playpen/bin /usr/local/bin/
#COPY --from=buildenv /playpen/assets /data


ENV PATH=/usr/local/bin:$PATH
ENV LD_LIBRARY_PATH=/usr/local/lib64:/usr/local/lib

COPY routes.txt .
COPY bouncer.cpp .

# if needed, set RMR vars
ENV RMR_SEED_RT=/playpen/routes.txt
#ENV RMR_RTG_SVC=rm-host:port
ENV RMR_SRC_ID=service-ricxapp-bouncer-xapp-rmr.ricxapp:4560
# ENV RMR_VCTL_FILE=/tmp/rmr.v
# RUN echo "2" >/tmp/rmr.v
#ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y gcc g++
#libboost-all-dev
RUN ls

RUN g++ -std=c++17 bouncer.cpp -o bouncer-xapp -lricxfcpp -lrmr_si -lpthread -lm -lboost_system -lcrypto -lssl -lcpprest -lpistache -lnlohmann_json_schema_validator -le2apwrapper -le2smrcwrapper -le2smrcwrapper

RUN cp bouncer-xapp /usr/local/bin/
CMD [ "/usr/local/bin/bouncer-xapp" ]


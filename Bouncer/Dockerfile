FROM kumarsandeep3/hcl-bouncer-base-image-ubntu-20:1.0.0 as buildenv
RUN mkdir -p /lite
WORKDIR /lite
#RUN apt-get update
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


# -----  copy files and source code ----------------------------------

COPY routes.txt .
COPY xapp-descriptor/ xapp-descriptor/
COPY src/ src/


RUN g++ -std=c++17 /lite/src/*.cpp  -I /lite/src/ -I /lite/e2ap/headers -I /lite/e2sm_kpm/headers -I /lite/e2sm_rc/headers -o bouncer-xapp -lricxfcpp -lrmr_si -lpthread -lm -lboost_system -lcrypto -lssl -lcpprest -lpistache -lnlohmann_json_schema_validator -le2apwrapper -le2smrcwrapper -le2smrcwrapper

RUN cp bouncer-xapp /usr/local/bin/

# -----  create final, smaller, image ----------------------------------
FROM ubuntu:20.04

RUN mkdir -p /lite
WORKDIR /lite

COPY --from=buildenv /usr/local/lib /usr/local/lib/
#for ssl
COPY --from=buildenv /lib/x86_64-linux-gnu/libcrypto.so.1.1 /lib/x86_64-linux-gnu/libcrypto.so.1.1
COPY --from=buildenv /lib/x86_64-linux-gnu/libssl.so.1.1 /lib/x86_64-linux-gnu/libssl.so.1.1

COPY --from=buildenv /usr/local/include /usr/local/include
COPY --from=buildenv /usr/local/bin/ /usr/local/bin/
COPY --from=buildenv /lite/routes.txt /lite/routes.txt

ENV RMR_SEED_RT=/lite/routes.txt
ENV RMR_SRC_ID=service-ricxapp-bouncer-xapp-rmr.ricxapp:4560
ENV PATH=/usr/local/bin:$PATH
ENV LD_LIBRARY_PATH=/usr/local/lib64:/usr/local/lib


CMD [ "/usr/local/bin/bouncer-xapp" ]

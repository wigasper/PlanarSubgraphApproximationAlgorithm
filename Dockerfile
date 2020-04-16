FROM ubuntu:bionic AS builder
RUN apt-get update && \
	apt-get install --no-install-recommends -y build-essential git cmake libboost-all-dev autoconf libtool pkg-config
#RUN apt-get upgrade
COPY ./ ./planarityfilter/
WORKDIR /planarityfilter/build
RUN cmake .. && make

FROM ubuntu:bionic
RUN apt-get update && \
	apt-get install -y libboost-all-dev
WORKDIR /
COPY --from=builder /planarityfilter/build/planarityfilter .

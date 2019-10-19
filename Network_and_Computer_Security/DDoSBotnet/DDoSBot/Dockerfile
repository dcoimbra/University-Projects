FROM ubuntu:18.04

RUN apt-get update && apt-get -y install telnetd netkit-inetd

# keep running
ENTRYPOINT ["tail", "-f", "/dev/null"]
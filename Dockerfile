FROM debian:latest

RUN apt-get update && \
    apt-get install -y valgrind gcc g++ make net-tools

COPY . /app
VOLUME /app

CMD ["bash"]

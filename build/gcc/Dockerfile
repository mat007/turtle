# $ docker build -f Dockerfile -t turtle-gcc .
FROM gcc
RUN apt-get update && apt-get install -y xsltproc docbook-xsl docbook-xml && apt-get autoremove && apt-get clean
ENV DOCBOOK_XSL_DIR=/usr/share/xml/docbook/stylesheet/docbook-xsl \
    DOCBOOK_DTD_DIR=/usr/share/xml/docbook/schema/dtd/4.2 \
    BOOST_ROOT=/home/dev/cpp/boost/
RUN echo 'cd /home/dev/cpp/turtle/build' >> ~/.bashrc
# $ docker run --platform=linux --rm -v C:/dev:/home/dev -m 16g -it turtle-gcc
# ./build.sh  --toolset=gcc "cxxflags=-std=c++17 -Wno-noexcept-type -Wno-unused-variable -Wno-unused-function -Wno-deprecated-declarations"

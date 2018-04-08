# $ docker build --platform=linux -f clang/Dockerfile -t turtle-clang .
FROM buildpack-deps:stretch
RUN apt-get update && apt-get install -y xsltproc docbook-xsl docbook-xml && apt-get autoremove && apt-get clean
ENV DOCBOOK_XSL_DIR=/usr/share/xml/docbook/stylesheet/docbook-xsl \
    DOCBOOK_DTD_DIR=/usr/share/xml/docbook/schema/dtd/4.2 \
    BOOST_ROOT=/home/dev/cpp/boost/
#    wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key|apt-key add - && \
RUN echo "deb http://apt.llvm.org/stretch/ llvm-toolchain-stretch-5.0 main" >> /etc/apt/sources.list && \
    apt-get update && \
    apt-get install -y --allow-unauthenticated clang-5.0 lld-5.0 libc++1 && \
    apt-get autoremove && \
    apt-get clean && \
    update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-5.0 100 && \
    update-alternatives --install /usr/bin/clang clang /usr/bin/clang-5.0 100
RUN echo 'cd /home/dev/cpp/turtle/build' >> ~/.bashrc
# $ docker run --platform=linux --rm -v C:/dev:/home/dev -m 32g -it turtle-clang
# ./build.sh --toolset=clang "cxxflags=-std=c++17 -stdlib=libc++ -Wno-unused-variable"

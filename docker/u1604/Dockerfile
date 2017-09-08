FROM openmicroscopy/ome-files-cpp-u1604
MAINTAINER ome-devel@lists.openmicroscopy.org.uk

RUN apt-get -y install \
    qt5-default \
    libqt5opengl5-dev \
    libqt5svg5-dev \
    libglm-dev

COPY . /git/ome-qtwidgets

RUN cmake \
    -Dgit-dir=/git \
    -Dbuild-prerequisites=OFF \
    -Dome-superbuild_BUILD_gtest=ON \
    -Dbuild-packages=ome-qtwidgets \
    -DCMAKE_BUILD_TYPE=Release \
    /git/ome-cmake-superbuild
RUN make
RUN make install
RUN ldconfig

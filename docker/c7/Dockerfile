FROM openmicroscopy/ome-files-cpp-c7
MAINTAINER ome-devel@lists.openmicroscopy.org.uk

RUN yum -y install \
    qt5-qtbase-devel \
    qt5-qtsvg-devel \
    glm-devel

COPY . /git/ome-qtwidgets

RUN cmake3 \
    -Dgit-dir=/git \
    -Dbuild-prerequisites=OFF \
    -Dome-superbuild_BUILD_gtest=ON \
    -Dbuild-packages=ome-qtwidgets \
    -DCMAKE_BUILD_TYPE=Release \
    /git/ome-cmake-superbuild
RUN make
RUN make install
RUN ldconfig

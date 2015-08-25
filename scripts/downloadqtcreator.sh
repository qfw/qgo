#!/bin/bash

QTC_VERSION=3.5
QTC_RELEASE=3.5.0
OS_NAME=`uname -s`

if [ "$OS_NAME" = "Darwin" ]; then
    alias wget="curl -O"
fi

wget http://master.qt-project.org/official_releases/qtcreator/${QTC_VERSION}/${QTC_RELEASE}/qt-creator-opensource-src-${QTC_RELEASE}.tar.gz && \
tar -C ../src/3rdparty -xvf qt-creator-opensource-src-${QTC_RELEASE}.tar.gz && \
rm qt-creator-opensource-src-${QTC_RELEASE}.tar.gz && \
cd ../src/3rdparty && \
mv qt-creator-opensource-src-${QTC_RELEASE} qtcreator

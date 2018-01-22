#! /bin/bash
# for php 7.1

sudo apt-get install bison
sudo apt-get install re2c
sudo apt-get install libxml2-dev
sudo apt-get install libcurl4-openssl-dev
sudo apt-get install libjpeg-dev
sudo apt-get install libpng12-dev
sudo apt-get install libfreetype6-dev
sudo apt-get install libmcrypt-dev
sudo apt-get install libpspell-dev
sudo apt-get install librecode-dev
sudo apt-get install libbz2-dev
sudo apt-get install libssl-dev

./configure --prefix=$HOME/local/php7 \
    --with-config-file-path=$HOME/local/php7/etc \
    --with-mcrypt=/usr/include \
    --with-mysqli=mysqlnd \
    --with-pdo-mysql=mysqlnd \
    --with-iconv \
    --with-zlib \
    --enable-xml \
    --enable-bcmath \
    --enable-shmop \
    --enable-sysvsem \
    --enable-inline-optimization \
    --enable-mbregex \
    --enable-fpm \
    --enable-mbstring \
    --enable-ftp \
    --enable-gd-native-ttf \
    --with-openssl \
    --enable-pcntl \
    --enable-sockets \
    --with-xmlrpc \
    --enable-zip \
    --enable-soap \
    --without-pear \
    --with-gettext \
    --enable-session \
    --with-curl \
    --with-jpeg-dir \
    --with-freetype-dir \
    --enable-opcache

# make && make install

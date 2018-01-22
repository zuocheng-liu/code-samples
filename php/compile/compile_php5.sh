#! /bin/bash
# for php 5

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

./buildconf --force # for php 5.6 without configure

./configure \
     --prefix=$HOME/local/php7 \
     --with-config-file-path=$HOME/local/php7/etc \
     --enable-ftp \
     --enable-inline-optimization \
     --enable-mbstring \
     --enable-soap \
     --enable-wddx=shared \
     --enable-xml \
     --with-gd \
     --with-gettext \
     --with-regex=system \
     --with-zlib-dir=/usr/lib \
     --enable-fpm \
     --with-jpeg-dir \
     --with-fpm-user=admin \
     --with-fpm-group=admin \
     --enable-shmop \
     --with-mcrypt \
     --with-mysqli=mysqlnd \
     --with-curl=/usr/local
#    --disable-debug \
#    --with-mysql=../mysql-5.1.54-linux-x86_64-glibc23/\
#    --with-freetype-dir=/usr/lib64/libfreetype.so\

# make && make install




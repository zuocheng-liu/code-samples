#! /bin/bash
# for php 5
./configure \
     --prefix=/usr/local/php \
#    --with-mysql=../mysql-5.1.54-linux-x86_64-glibc23/\
#    --with-freetype-dir=/usr/lib64/libfreetype.so\
#     --with-config-file-path=/usr/local/php/etc \
     --disable-debug \
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


./configure --prefix=/usr/local/php7 \
    --with-config-file-path=/usr/local/php7/etc \
    --with-mcrypt=/usr/include \
    --with-mysql=mysqlnd \
    --with-mysqli=mysqlnd \
    --with-pdo-mysql=mysqlnd \
    --with-gd \
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


# for php 7.1
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

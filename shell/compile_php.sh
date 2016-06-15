#! /bin/bash
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

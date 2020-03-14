#!/bin/bash

function my_cmd(){
  ip=$1
  echo "ssh ${ip} begine"
  echo "ssh ${ip} end"
}

tmp_fifofile="/tmp/$$.fifo" 
mkfifo $tmp_fifofile      # 新建一个fifo类型的文件
exec 1000<>$tmp_fifofile     # 将fd1000指向fifo类型
rm $tmp_fifofile    #删也可以

thread_num=100  # 最大可同时执行线程数量

#根据线程总数量设置令牌个数
for ((i=0;i<${thread_num};i++));do
  echo
done >&1000 



while read line
do
  # 一个read -u1000命令执行一次，就从fd1000中减去一个回车符，然后向下执行，
  # fd1000中没有回车符的时候，就停在这了，从而实现了线程数量控制
  read -u1000 

  #可以把具体的需要执行的命令封装成一个函数
  {   
    my_cmd $line
  } &

  echo >&1000 # 当进程结束以后，再向fd1000中加上一个回车符，即补上了read -u1000减去的那个
done < $file

wait
exec 1000>&- # 关闭fd1000
echo "over"

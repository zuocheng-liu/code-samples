// Copyright (c) 2016, Zuocheng.net
//
// Author: Zuocheng Liu
//
// Lisence: GPL
//
// File: code_samples/code_style/google_code_style.cc
// C++文件以.cc 结尾，头文件以.h 结尾。
//
// Google C++ code Style 代码示例 (简体中文版)

// **include文件** 
// 减少.h 文件中#include 的数量
// 包含头文件次序：
// - 本cc对应的头文件
// - C系统文件
// - C++系统文件
// - 其他库头文件
// - 本项目中头文件 
#include "code_samples/code_style/google_code_style.h"

#include <stdio.h>

#include <string>


// - 不要在.h 文件中定义宏；
// - 使用前正确#define，使用后正确#undef；
// - 不要只是对已经存在的宏使用#undef，选择一个不会冲突的名称；
// - 不使用会导致不稳定的C++构造（unbalanced C++ constructs，译者注）的宏，
// 至少文档说明其行为。
#define MAX((a), (b)) ((a) > (b) ? (a): (b)) 

uint32_t GoogleCodeStyle::MyExcitingMethod(CodeStyle &code_style, 
                                           char *output) {
          
}

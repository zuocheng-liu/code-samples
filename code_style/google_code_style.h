// Copyright (c) 2016, Zuocheng.net
//
// Author: Zuocheng Liu
//
// Lisence: GPL
//
// File: code_samples/code_style/google_code_style.h 
// (文件名要全部小写，可以包含下划线（_）或短线（-）)
// C++文件以.cc 结尾，头文件以.h 结尾。
//
// Google C++ code Style 代码示例 (简体中文版)
//
// *注释  使用//或/* */，但要统一。
// *注释  每一个文件版权许可及作者信息后，对文件内容进行注释说明
// -------------------------------------------------------------最多不能超过80行
#ifndef CODE_SAMPLES_CODE_STYLE_GOOGLE_CODE_STYLE_
#define CODE_SAMPLES_CODE_STYLE_GOOGLE_CODE_STYLE_
// 命名空间的名称是全小写的，其命名基于项目名称和目录结构
// 不要声明命名空间std 下的任何内容
namespace code_samples {
namespace code_style {

// **变量命名**
// 尽可能给出描述性名称，不要节约空间
// 变量名一律小写，单词间以下划线相连
// 少用全用变量，可以以g_与局部变量区分
int g_my_exciting_global_variable;

// 只有在描述数据时用struct ，其他情况都用class
typedef struct CodeStyle {
    uint32_t type;
} CodeStyle;

// **类注释** 
// 每个类的定义要附着描述类的功能和用法的注释
//
// GoogleCodeStyle , 通过代码示例，展现google c++ code style
//
// (按需注明synchronization assumptions，是否线程安全)
class GoogleCodeStyle {
public :
    
    // ** 声明次序 **
    // - typedefs 和enums；
    // - 常量；
    // - 构造函数；
    // - 析构函数；
    // - 成员函数，含静态成员函数；
    // - 数据成员，含静态数据成员。
    
    // **类型命名**
    // 每个单词以大写字母开头，不包含下划线
    // 所有类型命名——类、结构体、类型定义（typedef）、枚举——使用相同约定
    typedef enum StyleType {
        // 枚举值应全部大写，单词间以下划线相连
        GOOGLE_CODE_STYLE = 0,
        K_AND_R,
        POCO
    } StyleType;

    // 常量命名，在名称前加k
    const int kDaysInAWeek = 7;   

    GoogleCodeStyle() {}
    ~GoogleCodeStyle() {}
    // **函数注释**
    // 函数声明处注释描述函数功能，定义处描述函数实现.
    // - inputs（输入）及outputs（输出）；
    // - 对类成员函数而言：函数调用期间对象是否需要保持引用参数，是否会释放这些参数；
    // - 如果函数分配了空间，需要由调用者释放；
    // - 参数是否可以为NULL；
    // - 是否存在函数使用的性能隐忧（performance implications）；
    // - 如果函数是可重入的（re-entrant），其同步前提（synchronization assumptions）
    // 举例 ：
    // 初始化函数
    ~Init() {}

    // 普通函数名以大写字母开头，每个单词首字母大写，没有下划线
    // 
    uint32_t MyExcitingMethod(CodeStype &code_style, char *output);
    // 存取函数要与存取的变量名匹配
    inline int num_entries() const { return num_entries_; }
    inline void set_num_entries(int num_entries) { num_entries_ = num_entries; }

    // 类的成员变量以下划线结尾
    int num_completed_connections_;

protected :
    
private :
    // **类数据成员**
    // 每个类数据成员（也叫实例变量或成员变量）应注释说明用途。
    // 如果变量可以接受NULL 或-1, 等警戒值（sentinel values），须说明之
    int num_entries_;

}; // class GoogleCodeStyle

}} // namespace code_samples::code_style 

#endif //CODE_SAMPLES_CODE_STYLE_GOOGLE_CODE_STYLE

// help.cpp
#include "../include/help.h"
#include "../include/common.h"

void print_usage(const char* prog_name) {
    cerr << "Usage: " << prog_name << " <域名> [选项]...\n\n"
     << "选项:\n"
     << "  -a [文件]  启用子域名枚举（可指定字典文件）\n"
     << "  -t [次数]  设置DNS查询重试次数（默认3次）\n"
     << "  -v        启用详细输出模式\n";
}
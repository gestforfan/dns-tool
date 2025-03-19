# DNS Query Tool

[![License](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)
[![Build Status](https://img.shields.io/github/actions/workflow/status/gestforfan/dns-tool/build.yml?branch=main)](https://github.com/gestforfan/dns-tool/actions)

基于C++17实现的增强版DNS诊断工具，支持自动依赖安装、CNAME追踪、多类型记录查询和子域名枚举。

## ✨ 功能特性

- 🛠️ 自动检测并安装dig工具（支持apt/yum/dnf/pacman/apk）
- 🔍 多层级CNAME解析追踪（最多5层）
- 🌈 彩色终端输出（支持ANSI颜色）
- 📊 支持多种记录类型查询：A/AAAA/CNAME/MX/NS/TXT
- 🔁 可配置的重试机制
- 📂 子域名枚举功能（支持自定义字典文件）

## 🚀 快速开始

### 编译安装
```bash
git clone https://github.com/gestforfan/dns-tool.git
cd dns-tool
make
```

### 使用示例
```bash
# 基础查询
./dns example.com

# 详细模式（显示调试信息）
./dns example.com -v

# 使用子域名枚举（默认字典）
./dns example.com -a

# 自定义子域名字典 + 设置重试次数
./dns example.com -a custom_subdomains.txt -t 5
```

## 📝 参数说明
```text
Usage: dns 域名 [选项]
选项:
  -a [文件]  启用子域名枚举（可指定字典文件）
  -t [次数]  设置DNS查询重试次数（默认3次）
  -v        启用详细输出模式
  --type=T1,T2 指定查询的DNS记录类型(默认：所有类型)
```

## 📦 依赖要求
- dig工具（自动安装时会检测）
- C++17兼容编译器
- make构建工具

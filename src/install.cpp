// install.cpp
#include "../include/install.h"
#include "../include/utils.h"

unordered_map<string, vector<string>> package_managers = {
    {"apt", {"apt-get", "install", "-y", "dnsutils"}},
    {"yum", {"yum", "install", "-y", "bind-utils"}},
    {"dnf", {"dnf", "install", "-y", "bind-utils"}},
    {"pacman", {"pacman", "-S", "--noconfirm", "bind-tools"}},
    {"apk", {"apk", "add", "bind-tools"}}
};

bool check_dig_installed() {
    return system("command -v dig >/dev/null 2>&1") == 0;
}

void install_dig() {
    cout << COLOR_YELLOW << "dig未安装，尝试自动安装..." << COLOR_RESET << endl;

    string pkg_manager;
    for (auto& [mgr, _] : package_managers) {
        if (system(("command -v " + mgr + " >/dev/null 2>&1").c_str()) == 0) {
            pkg_manager = mgr;
            break;
        }
    }

    if (pkg_manager.empty()) {
        cerr << COLOR_RED << "无法检测到支持的包管理器！" << COLOR_RESET << endl;
        exit(EXIT_FAILURE);
    }

    vector<string> cmd_args = package_managers[pkg_manager];
    string install_cmd;
    for (auto& arg : cmd_args) {
        install_cmd += arg + " ";
    }

    cout << "执行安装命令: " << COLOR_BLUE << install_cmd << COLOR_RESET << endl;
    int status = system(("sudo " + install_cmd).c_str());
    
    if (status != 0) {
        cerr << COLOR_RED << "安装失败，请手动安装dig工具" << COLOR_RESET << endl;
        exit(EXIT_FAILURE);
    }

    if (!check_dig_installed()) {
        cerr << COLOR_RED << "安装后仍找不到dig命令" << COLOR_RESET << endl;
        exit(EXIT_FAILURE);
    }

    cout << COLOR_GREEN << "dig安装成功！" << COLOR_RESET << endl;
}

#include "../include/common.h"
#include "../include/install.h"
#include "../include/dns_query.h"
#include "../include/help.h"
#include "../include/utils.h"

int main(int argc, char* argv[]) {
    // 检查dig是否安装
    if (!check_dig_installed()) {
        install_dig();
    }

    // 参数验证
    if (argc < 2) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    string domain = argv[1];
    bool detailed = false;
    bool verbose = false;
    string subdomain_file;
    int retries = 3;

    // 参数解析
    for (int i = 2; i < argc; ++i) {
        if (strcmp(argv[i], "-a") == 0) {
            detailed = true;
            if (i+1 < argc && argv[i+1][0] != '-') {
                subdomain_file = argv[++i];
            }
        } else if (strcmp(argv[i], "-t") == 0) {
            if (i+1 < argc && all_of(argv[i+1], argv[i+1]+strlen(argv[i+1]), ::isdigit)) {
                retries = atoi(argv[++i]);
            } else {
                retries = 3;
            }
        } else if (strcmp(argv[i], "-v") == 0) {
            verbose = true;
        } else {
            if (argv[i][0] == '-') {
                cerr << COLOR_RED << "错误：未知选项 " << argv[i] << COLOR_RESET << endl;
            } else {
                cerr << COLOR_RED << "错误：未知参数 " << argv[i] << COLOR_RESET << endl;
            }
            print_usage(argv[0]);
            return EXIT_FAILURE;
        }
    }

    // 主域名查询
    cout << "===== 主域名查询: " << domain << " =====" << endl;
    query_domain(domain, retries, verbose);

    // 子域名扫描
    if (detailed) {
        vector<string> subdomains;
        
        // 从文件加载或使用默认列表
        if (!subdomain_file.empty()) {
            ifstream file(subdomain_file);
            if (!file) {
                cerr << COLOR_RED << "无法打开文件: " << subdomain_file << COLOR_RESET << endl;
                return EXIT_FAILURE;
            }

            string line;
            while (getline(file, line)) {
                // 清理输入并处理注释
                line.erase(line.find_last_not_of(" \t")+1);
                size_t pos = line.find('#');
                if (pos != string::npos) line = line.substr(0, pos);
                if (!line.empty()) subdomains.push_back(line);
            }
        } else {
            // 默认子域名列表
            subdomains = {
                "mail", "www", "ftp", "blog", "webmail",
                "smtp", "pop", "imap", "admin", "shop",
                "api", "bbs", "test", "dev", "staging",
                "mx", "ns1", "ns2", "cdn", "static",
                "app", "portal", "support", "wiki", "git"
            };
        }

        // 遍历所有子域名
        for (auto& sub : subdomains) {
            string full_sub = sub + "." + domain;
            cout << "\n===== 检查子域名: " << full_sub << " =====" << endl;

            // 检查有效记录存在性
            string cmd = "dig " + full_sub + " A +short +retry=" + to_string(retries) + 
                       " | grep -E '([0-9]{1,3}\\.){3}[0-9]{1,3}'"
                       " && dig " + full_sub + " AAAA +short +retry=" + to_string(retries) + 
                       " | grep -E '([a-f0-9]{1,4}:){7}[a-f0-9]{1,4}'";
            string output = exec(cmd.c_str());

            // 根据verbose模式决定是否显示无记录域名
            if (!output.empty() || verbose) {
                query_domain(full_sub, retries, verbose, "  ");
            } else {
                cout << "  " << COLOR_YELLOW << "无有效A/AAAA记录" << COLOR_RESET << endl;
            }
        }
    }

    return EXIT_SUCCESS;
}

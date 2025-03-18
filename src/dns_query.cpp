// dns_query.cpp
#include "../include/dns_query.h"
#include "../include/utils.h"

DNSResult enhanced_dig(const string& domain, const string& type, int retries) {
    DNSResult result;
    string current_domain = domain;
    int cname_depth = 0;
    const int max_cname_depth = 5;
    regex ipv4_pattern(R"(\b(?:\d{1,3}\.){3}\d{1,3}\b)");
    regex ipv6_pattern(R"((?:[a-f0-9]{1,4}:){7}[a-f0-9]{1,4})");
    bool original_type_has_records = false;

    while (cname_depth < max_cname_depth) {
        string cmd = "dig " + current_domain + " " + type + 
                    " +retry=" + to_string(retries) +
                    " +time=3 +noall +answer";
        string output = exec(cmd.c_str());

        stringstream ss(output);
        string line;
        bool has_cname = false;

        while (getline(ss, line)) {
            if (!line.empty()) {
                // 记录原始类型的结果
                if (line.find(type) != string::npos && type != "CNAME") {
                    original_type_has_records = true;
                }

                // 检测CNAME记录
                if (line.find("CNAME") != string::npos) {
                    if (type != "CNAME") result.has_conflict = true;
                    
                    size_t pos = line.find_last_of(" \t");
                    if (pos != string::npos) {
                        current_domain = line.substr(pos + 1);
                        current_domain.erase(remove(current_domain.begin(), 
                                                  current_domain.end(), '.'), 
                                          current_domain.end());
                        has_cname = true;
                        result.cnames.push_back(line);
                    }
                }
                // 提取IP地址
                else if (type == "A" || type == "AAAA") {
                    smatch match;
                    if (regex_search(line, match, 
                        (type == "A") ? ipv4_pattern : ipv6_pattern)) {
                        result.ips.push_back(match.str());
                    }
                }

                result.records.push_back(line);
            }
        }

        // 如果没有原始类型记录且存在CNAME冲突，清除记录
        if (result.has_conflict && !original_type_has_records) {
            result.records.clear();
            result.has_conflict = false;
        }

        if (!has_cname) break;
        if (++cname_depth >= max_cname_depth) {
            result.records.push_back(COLOR_RED "CNAME解析深度超过限制（可能循环）" COLOR_RESET);
        }
    }
    result.has_valid_records = !result.records.empty();
    return result;
}

void query_domain(const string& domain, int retries, bool verbose, const vector<string>& types, const string& indent = "") {
    for (const auto& type : types) {
        DNSResult result = enhanced_dig(domain, type, retries);
        
        if (!result.has_valid_records && !verbose) continue;

        cout << indent << "===== " << type << " =====" << endl;
        
        // 输出记录
        for (auto& line : result.records) {
            cout << indent << line << endl;
        }

        // 冲突检测
        if (result.has_conflict) {
            cout << COLOR_YELLOW << indent 
                 << "[警告] CNAME与其他记录类型冲突（违反RFC规范）" 
                 << COLOR_RESET << endl;
        }

        // 反向解析（仅对A/AAAA记录）
        if (type == "A" || type == "AAAA") {
            for (auto& ip : result.ips) {
                cout << indent << "===== PTR查询（反向解析 " << ip << ") =====" << endl;
                string cmd = "dig -x " + ip + " +retry=" + to_string(retries) + 
                            " +time=3 +noall +answer";
                string output = exec(cmd.c_str());
                
                stringstream ss(output);
                string line;
                while (getline(ss, line)) {
                    if (!line.empty()) {
                        cout << indent << line << endl;
                    }
                }
            }
        }
    }
}
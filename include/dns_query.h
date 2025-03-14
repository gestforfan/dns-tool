// dns_query.h
#ifndef DNS_QUERY_H
#define DNS_QUERY_H

#include "common.h"

struct DNSResult {
    vector<string> records;
    vector<string> cnames;
    vector<string> ips;
    bool has_conflict = false;
    bool has_valid_records = false;
};

DNSResult enhanced_dig(const string& domain, const string& type, int retries);
void query_domain(const string& domain, int retries, bool verbose, const string& indent = "");

#endif
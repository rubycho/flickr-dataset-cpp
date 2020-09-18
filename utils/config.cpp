#include "config.hpp"

#include <iostream>
#include <cstdlib>
#include <cstring>

#include "inih/ini.h"

using namespace std;

static int handler(
        void *user,
        const char *section,
        const char *name,
        const char *value
) {
    int val;
    auto *c = (Config *) user;

    if (!strcmp(name, "api_key"))
        c->api_key = string(value);
    else if (!strcmp(name, "api_secret"))
        c->api_secret = string(value);
    else if (!strcmp(name, "keyword"))
        c->keyword = string(value);
    else if (!strcmp(name, "size")) {
        val = atoi(value);
        if (val < 128) val = 128;
        if (val > 640) val = 640;
        c->size = val;
    }
    else if (!strcmp(name, "sp")) {
        val = atoi(value);
        if (val < 0) val = 1;
        c->sp = val;
    }
    else if (!strcmp(name, "ep")) {
        val = atoi(value);
        if (val < 0) val = 2;
        c->ep = val;
    }
    else
        return 0;
    return 1;
}

int Config::parse(const string& ini_path) {
    return ini_parse(ini_path.c_str(), handler, this);
}

void Config::print() const {
    cout << "========================================" << endl;

    cout << "api_key: " << this->api_key << endl;
    cout << "api_secret: " << this->api_secret << endl;
    cout << "keyword: " << this->keyword << endl;
    cout << "size: " << this->size << endl;
    cout << "start page: " << this->sp << endl;
    cout << "end page: " << this->ep << endl;

    cout << "========================================" << endl;
}

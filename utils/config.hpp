#ifndef FLICKR_DATASET_CONFIG_H
#define FLICKR_DATASET_CONFIG_H

#include <string>

class Config {
public:
    std::string api_key;
    std::string api_secret;
    std::string keyword;

    int size = 128;
    int sp = 1;
    int ep = 2;

    int parse(const std::string& ini_path);
    void print() const;
};

#endif

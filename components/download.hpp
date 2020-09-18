#ifndef FLICKR_DATASET_DOWNLOAD_H
#define FLICKR_DATASET_DOWNLOAD_H

#include <string>
#include <vector>

#include "curl/curl.h"

#include "../utils/config.hpp"

class Downloader {
public:
    explicit Downloader(const Config& _c);
    ~Downloader();

    void download();
private:
    Config c;
    std::string ws;

    std::vector<std::string> page_urls;
    std::vector<std::string> img_urls;
    std::vector<std::string> img_paths;

    CURL *curl;
    CURLcode res;

    int send_request(const std::string& url, const std::string& fname);
    int fill_page_urls();
    int fill_img_urls();
    int download_imgs();
    int process_imgs();
};

#endif

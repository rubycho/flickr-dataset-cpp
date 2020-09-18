#include "download.hpp"

#include <iostream>
#include <sys/stat.h>

#include "curl/curl.h"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "nlohmann/json.hpp"

#include "../utils/console.hpp"
#include "../utils/formatter.hpp"

using namespace std;
using namespace cv;

using json = nlohmann::json;

Downloader::Downloader(const Config& _c) {
    c = _c;

    /* init curl */
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    /* create workspace */
    ws = path_to_ws(c.keyword);
    if (mkdir(ws.c_str(), 0755) < 0) {
        fd_error("failed to mkdir", ws);
        exit(1);
    }
}

Downloader::~Downloader() {
    /* cleanup curl */
    curl_easy_cleanup(curl);
}

void Downloader::download() {
    if (fill_page_urls() < 0) {
        fd_error("failure during func", "fill_page_urls()"); return;
    }
    if (fill_img_urls() < 0) {
        fd_error("failure during func", "fill_img_urls()"); return;
    }
    if (download_imgs() < 0) {
        fd_error("failure during func", "download_imgs()"); return;
    }
    if (process_imgs() < 0) {
        fd_error("failure during func", "process_imgs()"); return;
    }
}

/*
 * int send_request(string url, string fname)
 * send request to url,
 * and save response to file called fname.
 */
int Downloader::send_request(const string& url, const string& fname) {
    FILE *fp = fopen(fname.c_str(), "w+");
    if (fp == nullptr) return -1;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

    res = curl_easy_perform(curl); fclose(fp);
    if (res != CURLE_OK) return -1;

    return 0;
}

int Downloader::fill_page_urls() {
    ostringstream ss;

    page_urls.clear();
    for (int i = c.sp; i <= c.ep; i++) {
        ss.str(""); ss.clear();
        ss << "https://www.flickr.com/services/rest/"
            << "?method=flickr.photos.search"
            << "&api_key=" << c.api_key
            << "&text=" << c.keyword
            << "&sort=relevance"
            << "&extras=url_z"
            << "&per_page=100"
            << "&page=" << i
            << "&format=json"
            << "&nojsoncallback=1";
        page_urls.push_back(ss.str());
    }
    return 0;
}

int Downloader::fill_img_urls() {
    FILE *fp;
    string tmp_name = "tmp.json";

    string task_name = "Fetch image urls";
    int task_len = page_urls.size();

    img_urls.clear();
    for (int i = 0; i < task_len; i++) {
        fd_progress(task_name, i+1, task_len);

        if (send_request(page_urls.at(i), tmp_name) < 0) {
            fd_error("failed to download json", page_urls.at(i));
            continue;
        }

        fp = fopen(tmp_name.c_str(), "r");
        if (fp == nullptr) {
            fd_error("failed to open file", tmp_name);
            return -1;
        }

        auto j = json::parse(fp);
        auto p = j["photos"]["page"];
        if ((c.sp + i) != p) {
            fd_warn("stopping at page" , to_string(c.sp + i));
            break;
        }

        auto photos = j["photos"]["photo"];
        for (auto photo : photos)
            if (photo.contains("url_z"))
                img_urls.push_back(photo["url_z"]);
    }
    remove(tmp_name.c_str());
    return 0;
}

int Downloader::download_imgs() {
    string task_name = "Download images";
    int task_len = img_urls.size();
    int zw; for (zw = 1; pow(10, zw) <= task_len; zw++);

    img_paths.clear();
    for (int i=0; i < task_len; i++) {
        fd_progress(task_name, i+1, task_len);

        string img_name = path_to_img(ws, i, zw);
        if (send_request(img_urls.at(i), img_name) < 0) {
            fd_error("failed to download img", img_urls.at(i));
            continue;
        }
        img_paths.push_back(img_name);
    }
    return 0;
}

int Downloader::process_imgs() {
    Mat img;
    for (const auto& path : img_paths) {
        img = cv::imread(path, IMREAD_COLOR);
        resize(img, img, Size(c.size, c.size), 0.5, 0.5, INTER_AREA);
        imwrite(path, img);
    }
    return 0;
}

#ifndef FLICKR_DATASET_FORMATTER_H
#define FLICKR_DATASET_FORMATTER_H

#include <string>

std::string path_to_ws(const std::string& dname);
std::string path_to_img(const std::string& ws_path, int idx, int zw = 0);
std::string path_to_imgs(const std::string& dname);

#endif

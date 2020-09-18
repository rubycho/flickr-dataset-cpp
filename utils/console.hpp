#ifndef FLICKR_DATASET_CONSOLE_H
#define FLICKR_DATASET_CONSOLE_H

#include <string>

void fd_error(const std::string& msg, const std::string& reason = "");
void fd_warn(const std::string& msg, const std::string& reason = "");

void fd_progress(const std::string& task_name, int curr, int total);

#endif

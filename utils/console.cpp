#include "console.hpp"

#include <iostream>

using namespace std;

void fd_error(const string& msg, const string& reason) {
    cerr << "[ERR] " << msg << ": " << reason << endl;
}

void fd_warn(const string& msg, const string& reason) {
    cerr << "[WARN] " << msg << ": " << reason << endl;
}

void fd_progress(const string& task_name, int curr, int total) {
    cout << "Current: " << curr << " / Total: " << total << " [" << task_name << "]" << endl;
}

#include "formatter.hpp"

#include <sstream>
#include <iomanip>

using namespace std;

string path_to_ws(const string& dname) {
    ostringstream ss;
    ss << "./ws_" << dname;

    return ss.str();
}

string path_to_img(const string& ws_path, int idx, int zw) {
    ostringstream ss;
    ss << ws_path << "/";
    ss << setw(zw) << setfill('0') << idx;
    ss << ".jpg";

    return ss.str();
}

string path_to_imgs(const string& dname) {
    ostringstream ss;
    ss << path_to_ws(dname) << "/*.jpg";

    return ss.str();
}

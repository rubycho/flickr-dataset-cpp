#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>

#include <QtWidgets/QApplication>

#include "utils/config.hpp"
#include "utils/console.hpp"

#include "components/download.hpp"
#include "components/check.hpp"

#define CMD_NA          0
#define CMD_DOWNLOAD    1
#define CMD_CHECK       2

using namespace std;

void help(char *exe_name) {
    cout << "Usage: " << endl;
    cout << "\t" << exe_name << " download -c [path to config.ini]" << endl;
    cout << "\t" << exe_name << " check -c [path to config.ini]" << endl;
    cout << "* c option is optional. The default value is ./config.ini." << endl;

    exit(0);
}

int main(int argc, char **argv) {
    int mode = CMD_NA;
    string ini_path = "./config.ini";

    if (argc < 2)
        help(argv[0]);

    /* command check */
    if (!strcmp(argv[1], "download"))
        mode = CMD_DOWNLOAD;
    else if (!strcmp(argv[1], "check"))
        mode = CMD_CHECK;
    else
        help(argv[0]);

    /* option check */
    int ret = getopt(argc, argv, "c:");
    if (ret == 'c')
        ini_path = optarg;

    /* ini file check */
    auto c = Config();
    if (c.parse(ini_path) < 0) {
        fd_error("failed to parse ini file", ini_path);
        exit(1);
    }
    c.print();

    /* handle each command */
    if (mode == CMD_DOWNLOAD)
        Downloader(c).download();
    if (mode == CMD_CHECK) {
        QApplication app(argc, argv);
        CheckWidget widget(c);

        widget.show();
        return app.exec();
    }
    return 0;
}

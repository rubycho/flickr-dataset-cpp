#ifndef FLICKR_DATASET_CHECK_H
#define FLICKR_DATASET_CHECK_H

#include <map>
#include <string>
#include <vector>

#include <QtWidgets>

#include "../utils/config.hpp"

class CheckWidget: public QWidget {
Q_OBJECT
public:
    explicit CheckWidget(Config& c, QWidget *parent = nullptr);
    ~CheckWidget();
private:
    std::string mark_text = "Will be deleted: YES";
    std::string unmark_text = "Will be deleted: NO";

    int idx = 0;
    int path_len = 0;

    std::vector<std::string> paths;
    std::map<std::string, bool> history;

    QLabel *status;
    QLabel *path_status;
    QLabel *mark_status;

    QLabel *img;

    QPushButton *mark_btn;
    QPushButton *next_btn;
    QPushButton *prev_btn;
    QPushButton *stop_btn;

    QHBoxLayout *btn_layout;
    QVBoxLayout *layout;

    int fill_paths(const Config& c);
private slots:
    void on_mark();
    void on_next();
    void on_prev();
    void on_stop();
    void on_click();
};

#endif

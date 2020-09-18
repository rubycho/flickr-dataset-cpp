#include "check.hpp"

#include <iostream>
#include <sstream>
#include <cstdio>
#include <glob.h>

#include "../utils/console.hpp"
#include "../utils/formatter.hpp"

using namespace std;

int CheckWidget::fill_paths(const Config& c) {
    string ws = path_to_imgs(c.keyword);
    glob_t glob_result;

    if (glob(ws.c_str(), GLOB_TILDE, nullptr, &glob_result) < 0) return -1;
    if (glob_result.gl_pathc <= 0) return -1;

    for (int i=0; i<glob_result.gl_pathc; i++)
        paths.emplace_back(glob_result.gl_pathv[i]);
    path_len = paths.size();
    return 0;
}

CheckWidget::CheckWidget(Config& c, QWidget *parent): QWidget(parent) {
    if (fill_paths(c) < 0) {
        fd_error("failed to fetch image paths");
        exit(1);
    }

    for (auto & path : paths)
        history[path] = false;

    status = new QLabel("");
    path_status = new QLabel("");
    mark_status = new QLabel("");

    img = new QLabel();
    img->setAlignment(Qt::AlignCenter);

    mark_btn = new QPushButton("Mark/Unmark");
    next_btn = new QPushButton("Next");
    prev_btn = new QPushButton("Prev");
    stop_btn = new QPushButton("Stop");

    connect(mark_btn, SIGNAL (released()), this, SLOT(on_mark()));
    connect(next_btn, SIGNAL (released()), this, SLOT(on_next()));
    connect(prev_btn, SIGNAL (released()), this, SLOT(on_prev()));
    connect(stop_btn, SIGNAL (released()), this, SLOT(on_stop()));

    btn_layout = new QHBoxLayout();
    btn_layout->addWidget(mark_btn);
    btn_layout->addWidget(next_btn);
    btn_layout->addWidget(prev_btn);
    btn_layout->addWidget(stop_btn);

    layout = new QVBoxLayout();
    layout->addWidget(status);
    layout->addWidget(path_status);
    layout->addWidget(mark_status);
    layout->addWidget(img);
    layout->addLayout(btn_layout);

    this->setLayout(layout);
    on_click();
}

CheckWidget::~CheckWidget() {
    delete layout;
    delete btn_layout;

    delete status;
    delete path_status;
    delete mark_status;

    delete img;

    delete mark_btn;
    delete next_btn;
    delete prev_btn;
    delete stop_btn;
};

void CheckWidget::on_mark() {
    string curr_path = paths.at(idx);
    history[curr_path] = !history[curr_path];
    on_click();
}

void CheckWidget::on_next() {
    idx++; on_click();
}

void CheckWidget::on_prev() {
    idx--; on_click();
}

void CheckWidget::on_stop() {
    setEnabled(false);
    for (auto& path : paths) {
        if (history[path]) {
            remove(path.c_str());
            cout << "Removed " << path << endl;
        }
    }
    exit(0);
}

void CheckWidget::on_click() {
    string curr_path = paths.at(idx);

    ostringstream ss;
    ss << "Current " << idx << " / Total " << path_len;
    status->setText(ss.str().c_str());

    ss.str(""); ss.clear();
    ss << "Path: " << curr_path;
    path_status->setText(ss.str().c_str());

    mark_status->setText(history[curr_path] ?
        mark_text.c_str() : unmark_text.c_str());

    QPixmap pixmap(curr_path.c_str());
    pixmap = pixmap.scaled(256, 256);
    img->setPixmap(pixmap);

    next_btn->setEnabled(idx + 1 < path_len);
    prev_btn->setEnabled(idx > 0);
}

//
// Created by peter on 2020-04-05.
//

#include "mainwindow.h"
#include "view/ui_mainwindow.h"
#include <iostream>


MainWindow::MainWindow(const int window_height, const int window_width, QWidget *parent) : QMainWindow(parent),
                                                                                           ui(new Ui::MainWindow),
                                                                                           image(new QImage(
                                                                                                   window_width,
                                                                                                   window_height,
                                                                                                   QImage::Format_RGB32)),
                                                                                           window_height(window_height),
                                                                                           window_width(window_width) {
    ui->setupUi(this);
    this->setFixedSize(window_width, window_height);
}

MainWindow::~MainWindow() {
    delete image;
    delete ui;
}

void MainWindow::set_pixel(int x, int y, const QColor &color) {
    image->setPixelColor(x, y, color);
}

int MainWindow::get_width() const {
    return image->width();
}

int MainWindow::get_height() const {
    return image->height();
}

void MainWindow::display() {
    ui->label->setPixmap(QPixmap::fromImage((*image).mirrored()));
}



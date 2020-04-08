//
// Created by peter on 2020-04-05.
//

#ifndef TINYRENDERER_MAINWINDOW_H
#define TINYRENDERER_MAINWINDOW_H

#include <iostream>

#include <QMainWindow>
#include <QLabel>
#include <QImage>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(int window_height, int window_width, QWidget *parent = nullptr);

    ~MainWindow() override;

    void set_pixel(int x, int y, const QColor &color);

    int get_width() const;

    int get_height() const;

    void display();

private:
    Ui::MainWindow *ui;
    QImage *image;
    const int window_height;
    const int window_width;
};

#endif //TINYRENDERER_MAINWINDOW_H

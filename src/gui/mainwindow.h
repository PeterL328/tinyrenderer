//
// Created by peter on 2020-04-05.
//

#ifndef TINYRENDERER_MAINWINDOW_H
#define TINYRENDERER_MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
};

#endif //TINYRENDERER_MAINWINDOW_H

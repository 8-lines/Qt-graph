#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void drawGraph(bool notEmpty = 0);
    void recountPixels();
    void getData();
    void inputLock();
    void inputUnlock();
    
private slots:
    void on_btnStart_clicked();

    void on_btnPause_clicked();

    void on_btnBreak_clicked();

    //void on_btnSave_clicked();

    //void on_load_clicked();

private:
    Ui::MainWindow *ui;
    double par_A,par_B,par_C;
    double leftX,rightX;
    double botY,topY;
    int pictWidth,pictHeight;
    double step;
    double onePixelX,onePixelY;
    double Ox,Oy;
    bool pause_b, break_b, progress_b;
};

inline void delay(int millisecondsWait);

#endif // MAINWINDOW_H

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pictHeight = 370;
    pictWidth = 540;
    step = 0.1;
    leftX = -100; rightX = 100;
    botY = -100; topY = 100;
    step = 0.01;
    drawGraph(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Вычисление пикселей на графе
void MainWindow::recountPixels()
{
    onePixelX = 540.0/(rightX-leftX);
    onePixelY = 370.0/(topY-botY);
    Ox = fabs(leftX); Oy = topY;
}

// Блокировка полей ввода
void MainWindow::inputLock()
{
    ui->inputA->setReadOnly(true);
    ui->inputB->setReadOnly(true);
    ui->inputC->setReadOnly(true);
    ui->inputLeftX->setReadOnly(true);
    ui->inputRightX->setReadOnly(true);
    ui->inputBotY->setReadOnly(true);
    ui->inputTopY->setReadOnly(true);
    ui->inputStep->setReadOnly(true);
    ui->funcBox->setEnabled(false);
}

// Разблокировка полей ввода
void MainWindow::inputUnlock()
{
    ui->inputA->setReadOnly(false);
    ui->inputB->setReadOnly(false);
    ui->inputC->setReadOnly(false);
    ui->inputLeftX->setReadOnly(false);
    ui->inputRightX->setReadOnly(false);
    ui->inputBotY->setReadOnly(false);
    ui->inputTopY->setReadOnly(false);
    ui->inputStep->setReadOnly(false);
    ui->funcBox->setEnabled(true);
}

// Получение информации из полей ввода
void MainWindow::getData()
{
    par_A = ui->inputA->text().toDouble();
    par_B = ui->inputB->text().toDouble();
    par_C = ui->inputC->text().toDouble();
    leftX = ui->inputLeftX->text().toDouble();
    rightX = ui->inputRightX->text().toDouble();
    botY = ui->inputBotY->text().toDouble();
    topY = ui->inputTopY->text().toDouble();
    step = ui->inputStep->text().toDouble();
    inputLock();

    // Тестовые значения-заглушки
    /*par_A = 1;
    par_B = 1;
    par_C = 1;
    leftX = -100;
    rightX = 100;
    botY = -100;
    topY = 100;
    step = 0.1;*/
}

// Отрисовка графика функции
void MainWindow::drawGraph(bool notEmpty)
{
    QPixmap graph(540,370);
    QPainter paint;
    QLine line;

    // Очищаем существующий график и рисуем ось координат
    paint.begin(&graph);
    paint.eraseRect(0,0,540,370);
    paint.drawLine(Ox*onePixelX,0,Ox*onePixelX,pictHeight);
    paint.drawLine(0,Oy*onePixelY,pictWidth,Oy*onePixelY);

    paint.setPen(QPen(Qt::black,3));
    for(double i = leftX;i<=rightX;i+=10.0)
        paint.drawPoint((i+Ox)*onePixelX,Oy*onePixelY);
    for(double i = botY;i<=topY;i+=10.0)
        paint.drawPoint(Ox*onePixelX,(Oy-i)*onePixelY);

    ui->outputGraph->setPixmap(graph);

    // Отрисовку пустого графика заканчиваем здесь
    if(notEmpty == false) {
        paint.end();
        return;
    }

    paint.setPen(QPen(Qt::green,1,Qt::SolidLine));
    paint.setRenderHint(QPainter::Antialiasing, true);

    int funcNmb = ui->funcBox->currentIndex();
    int f_length = (int)((rightX-leftX)/step) + 1;
    double* x_arr = new double[f_length];
    double* y_arr = new double[f_length];
    double x = leftX;
    double y = 0;
    int progress = 0;
    QString pr_text;
    pause_b = false, break_b = false, progress_b = false;

    // Вычисляем функцию
    for(int i = 0; i<f_length; i++) {
        // Проверяем нажатие кнопок Pause и Break
        if (pause_b == true)
        {
            delay(200);
            if (i > 0)
                i--;
            continue;
        }

        if (break_b == true)
            break;

        // Выбираем нужную функцию и вычисляем значение в следующей точке
        switch (funcNmb)
        {
            case 0:
                y = par_A*(x*x) + par_B*x + par_C;
                break;
            case 1:
                y = par_A * sin(x) + par_B * cos( par_C*x );
                break;
            case 2:
                y = par_A*log( par_B*x );
                break;
            case 3:
                y = par_A / ( sin(x*x) * par_B );
                break;
            default:
                y = 0;
                break;
        }

        // Отрисовываем линию на графике
        if(i!=0 && !std::isnan(y))
        {
            line = QLine((x_arr[i-1]+Ox)*onePixelX, (Oy-y_arr[i-1])*onePixelY, (x+Ox)*onePixelX, (Oy-y)*onePixelY);
            paint.drawLine(line);
            ui->outputGraph->setPixmap(graph);
        }

        // Заполняем массивы X и Y
        x_arr[i] = x;
        y_arr[i] = y;
        x += step;

        // Фиксируем прогресс
        progress = ((double)i/f_length)*100;
        pr_text = QStringLiteral("Progress (%1\%)").arg(progress);
        ui->btnStart->setText(pr_text);
        delay(3);

    }

    ui->btnStart->setText("New");
    paint.end();
    return;
}

// Если нажата кнопка Start/New
void MainWindow::on_btnStart_clicked()
{
    QString text = ui->btnStart->text();
    if (text == "Start")
    {
        getData();
        recountPixels();
        drawGraph(true);
    }
    if (text == "New")
    {
        drawGraph(false);
        ui->btnStart->setText("Start");
        inputUnlock();
    }
}

// Если нажата кнопка Pause/Continue
void MainWindow::on_btnPause_clicked()
{
    QString text = ui->btnPause->text();

    if (text == "Pause")
    {
        pause_b = true;
        ui->btnPause->setText("Continue");
    }
    if (text == "Continue")
    {
        pause_b = false;
        ui->btnPause->setText("Pause");
    }
}

// Если нажата кнопка Break
void MainWindow::on_btnBreak_clicked()
{
    break_b = true;
}

// Вспомогательная функция ожидания
inline void delay(int millisecondsWait)
{
    QEventLoop loop;
    QTimer t;
    t.connect(&t, &QTimer::timeout, &loop, &QEventLoop::quit);
    t.start(millisecondsWait);
    loop.exec();
}


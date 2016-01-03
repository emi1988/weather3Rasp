#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "defs.h"

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QLabel>



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void startRequest(QUrl url);


    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void generateLabels();
    void parseXML(QByteArray receivedXML);
    void parseNews(QByteArray receivedXML);

    QList<QList<QLabel*>* > dayList;

    QList<QLabel *> labelList;

    QList<QLabel *> day1Labels;
    QList<QLabel *> day2Labels;
    QList<QLabel *> day3Labels;

    QNetworkAccessManager networkAccesManager;
    QNetworkReply *reply;

    int currentModuleRefresh;
    bool waitForRefresh;

    //enum weekDays { Montag, Dienstag , Mittwoch, Donnerstag, Freitag, Samstag, Sonntag };

private slots:

    void httpFinished();
    void timerTimeFinished();
    void timerHttpFinished();
    //void httpReadyRead();
    //void on_pushButtonStart_clicked();


};



#endif // MAINWINDOW_H

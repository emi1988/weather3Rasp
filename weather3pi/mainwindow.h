#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "defs.h"

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QLabel>

#include <gpiocontrol.h>



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void startRequest(QUrl url, QString sender);


    ~MainWindow();

private:
    Ui::MainWindow *ui;
    gpioControl *myGpioControl;
    bool videoRequested;

    void generateLabels();
    void parseWeather(QByteArray receivedXML);
    void parseNews(QByteArray receivedXML);
    void parseVideoPodcast(QByteArray receivedXML);
    void showVideoPodcast();
    void showImages();
    void readIniFile();
    void readImageFiles();

    //QList<QList<QLabel*>* > dayList;

    QList<QLabel *> m_labelListWeather;
    QList<QLabel *> m_labelListNews;

    //QList<QLabel *> day1Labels;
    //QList<QLabel *> day2Labels;
    //QList<QLabel *> day3Labels;

    QNetworkAccessManager *m_networkAccesManager;
    QNetworkReply *m_reply;
    QNetworkReply *m_replyVideoPodcast;

    QString m_videoPodscastUrl;

    QHash<QString, QString> m_settings;
    QStringList m_picturePaths;
    int m_currentPicCounter;

    int m_currentHttpModule;
    bool m_waitForRefresh;
    QStringList m_httpModules;

    int m_newsUrlCounter;
    int m_newsPositionCounter;


private slots:

    void httpFinished();
    void videoPodcastFinished();
    void timerTimeFinished();
    void timerHttpFinished();
    void timerPicturesFinished();
    //void httpReadyRead();
    //void on_pushButtonStart_clicked();
    void gpioChanged(int pin);

};



#endif // MAINWINDOW_H

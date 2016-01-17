#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtGui>
#include <QtNetwork>
#include <QDebug>
#include <QtXml/QDomDocument>
#include <QDate>
#include <QLabel>
#include <QFile>
#include <QTime>
//#include <QWebView>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    readIniFile();

    m_networkAccesManager = new QNetworkAccessManager(this);

   //TEST
//    QUrl url = QUrl("http://api.wetter.com/forecast/weather/city/DE0007840/project/testqt/cs/99da8a0c7841156f23b9234011fd9ff5");
//    startRequest(url,"NewsWeather");

    //QUrl url2 = QUrl("http://www.tagesschau.de/export/video-podcast/tagesschau-in-100-sekunden/");
    QUrl urlVideopodcast = QUrl(m_settings.value("videoPodcastUrl"));
    startRequest(urlVideopodcast, "videoPodcast");

    ui->setupUi(this);
    ui->centralWidget->setStyleSheet("background-color:black;");
    //ui->textEditDates->setStyleSheet("QLabel {font-size: 14pt; color: white }");

    ui->labelTime->setStyleSheet("QLabel { color: white; font-weight: bold; font-size: 40pt }");
    ui->labelTime->setTextFormat(Qt::RichText);
    ui->labelTime->setFont(QFont("Comic Sans MS"));

    ui->labelPicture->setStyleSheet("QLabel { color: white; font-weight: bold; font-size: 20pt }");

    generateLabels();

    //set the httpModules which are set in the settings
    if(m_settings.value("uiWeather").compare("1")== 0)
    {
        m_httpModules.append("weather");
    }
    if(m_settings.value("uiVideoPodcast").compare("1")== 0)
    {

    }


    if(m_settings.value("uiNews").compare("1")== 0)
    {
        //check if at least one (respectively the first news-url) is set
        if(m_settings.contains("news0"))
        {
            m_httpModules.append("news");
        }
    }

    m_currentHttpModule = 0;
    m_waitForRefresh = false;

    m_newsPositionCounter = 0;
    m_newsUrlCounter = 0;

    //set the timers for the activated modules

    if(m_httpModules.size() > 0)
    {
        QTimer *timerHttp = new QTimer(this);
        connect(timerHttp, SIGNAL(timeout()), this, SLOT(timerHttpFinished()));
        timerHttp->start(m_settings.value("httpRefreshTime").toInt());
    }

    if(m_settings.value("uiTime").compare("1")== 0)
    {
        QTimer *timerTimer = new QTimer(this);
        connect(timerTimer, SIGNAL(timeout()), this, SLOT(timerTimeFinished()));
        timerTimer->start(1000);
    }
    else
    {
        ui->gridLayoutTime->removeWidget(ui->labelTime);
    }

    if(m_settings.value("uiImages").compare("1")== 0)
    {
        readImageFiles();

        //check if there are pictures in the specified path
        if(m_picturePaths.size() >0)
        {
            QTimer *imagesTimer = new QTimer(this);
            connect(imagesTimer, SIGNAL(timeout()), this, SLOT(timerPicturesFinished()));
            imagesTimer->start(m_settings.value("slideShowTime").toInt());
        }

    }
}

void MainWindow::startRequest(QUrl url, QString sender)
{

    if(sender.compare("NewsWeather") == 0)
    {
        m_reply = m_networkAccesManager->get(QNetworkRequest(url));

        connect(m_reply, SIGNAL(finished()), this , SLOT(httpFinished()));
    }
    if(sender.compare("videoPodcast")== 0)
    {
        // m_replyVideoPodcast = m_networkAccesManagerVideoPodcast.get(QNetworkRequest(url));
        m_replyVideoPodcast = m_networkAccesManager->get(QNetworkRequest(url));

        connect(m_replyVideoPodcast, SIGNAL(finished()), this , SLOT(videoPodcastFinished()));
    }

}
MainWindow::~MainWindow()
{
    delete ui;
}

/*show all
void MainWindow::parseXML(QByteArray receivedXML)
{

    QDomDocument domDoc;

    domDoc.setContent(receivedXML);

    QDomElement citytElement = domDoc.namedItem("city").toElement();

    if(citytElement.isNull())
    {
        ui->textEditDates->append("error");
    }
    QDomElement forecastElement = citytElement.namedItem("forecast").toElement();

    if(forecastElement.isNull())
    {
        ui->textEditDates->append("error");
    }

    QString weekday[] = {"", "Montag", "Dienstag" , "Mittwoch", "Donnerstag", "Freitag", "Samstag", "Sonntag"};
    //loop through all date-child nodes
    QDomElement currentDateElement = forecastElement.firstChildElement("date");

   int counter = 1;
   int labelCounter = 0;
   while(!currentDateElement.isNull())
    {
        QString currentDateString = currentDateElement.attribute("value", "null");
        qDebug() << "date" << currentDateString;

        QDate currentDateTime = QDate::fromString(currentDateString, "yyyy-MM-dd");
        qDebug() << currentDateTime.dayOfWeek() << currentDateTime.day();
        qDebug() << weekday[currentDateTime.dayOfWeek()];

        //set the weekday
       // labelList.at(labelCounter)->setText(weekday[currentDateTime.dayOfWeek()]);
        //labelCounter++;

        //go through all the time-elements in one day

        QDomElement currentTimeElement = currentDateElement.firstChildElement("time");

        QString collectedDayStringRich = "";
        //go through all the elements at one time
        while(!currentTimeElement.isNull())
        {
            QString currentTimeString = currentTimeElement.attribute("value", "null");

            //set the time
            //labelList.at(labelCounter)->setText(currentTimeString);

            //get the weather text
            QDomElement currentWeatherTxt = currentTimeElement.firstChildElement("w_txt");
            QString currentWeatherTxtString = currentWeatherTxt.text();

            //get the max temp
            QDomElement currentMaxTemp = currentTimeElement.firstChildElement("tx");
            QString currentMaxTempString= currentMaxTemp.text();

            //get the min temp
            QDomElement currentMinTemp = currentTimeElement.firstChildElement("tn");
            QString currentMinTempString= currentMinTemp.text();


            //set the time + weather text
           // labelList.at(labelCounter)->setText(currentTimeString + "\n" + currentWeatherTxtString + "\n Max:" + currentMaxTempString + "\n Min:" + currentMinTempString);

            collectedDayStringRich = collectedDayStringRich + "<hr>" + currentTimeString + "<br>" + currentWeatherTxtString + "<br> Max:<b>" + currentMaxTempString + "°C</b><br> Min:<b>" + currentMinTempString + "°C</b><br><br>";
//"<font size = 2>" +

           // labelList.at(labelCounter)->setWordWrap(true);
            //labelCounter++;

            //get the next time element
            currentTimeElement = currentTimeElement.nextSiblingElement("time");

        }

       QString curentDayStringRich = weekday[currentDateTime.dayOfWeek()] + "<br>" + collectedDayStringRich;


       labelList.at(labelCounter)->setText(curentDayStringRich);
       labelCounter++;
        //get the next element
        currentDateElement =  currentDateElement.nextSiblingElement("date");
        counter ++;
    }

}
*/

void MainWindow::parseWeather(QByteArray receivedXML)
{

    QDomDocument domDoc;

    domDoc.setContent(receivedXML);

    QDomElement citytElement = domDoc.namedItem("city").toElement();

    if(citytElement.isNull())
    {
        //  ui->textEditDates->append("error");
    }
    QDomElement forecastElement = citytElement.namedItem("forecast").toElement();

    if(forecastElement.isNull())
    {
        // ui->textEditDates->append("error");
    }

    QString weekday[] = {"", "Montag", "Dienstag" , "Mittwoch", "Donnerstag", "Freitag", "Samstag", "Sonntag"};

    //loop through all date-child nodes
    QDomElement currentDateElement = forecastElement.firstChildElement("date");


    int labelCounter = 0;
    while(!currentDateElement.isNull())
    {
        QString currentDateString = currentDateElement.attribute("value", "null");
        qDebug() << "date" << currentDateString;

        QDate currentDateTime = QDate::fromString(currentDateString, "yyyy-MM-dd");
        qDebug() << currentDateTime.dayOfWeek() << currentDateTime.day();
        qDebug() << weekday[currentDateTime.dayOfWeek()];

        //get the rain probability
        QString rainProbability = currentDateElement.firstChildElement("pc").text();

        //go through all the time-elements in one day

        QDomElement currentTimeElement = currentDateElement.firstChildElement("time");

        QString collectedDayStringRich = "";

        double maxDayTemp= 0;
        double minDayTemp=0;

        QString weatherState11, weatherState17;

        QString weatherText11, weatherText17;

        int counter = 0;

        while(!currentTimeElement.isNull())
        {
            QString currentTimeString = currentTimeElement.attribute("value", "null");

            //set the time
            //labelList.at(labelCounter)->setText(currentTimeString);

            //get the weather text
            QString currentWeatherTxt = currentTimeElement.firstChildElement("w_txt").text();

            //get the wather state
            QString currentWeatherState = currentTimeElement.firstChildElement("w").text();


            //just save the text and state for 11 a.m. an 17 p.m.
            if(counter == 1)
            {
                weatherText11 = currentWeatherTxt;
                weatherState11 = currentWeatherState;
            }
            else if(counter ==2)
            {
                weatherText17 = currentWeatherTxt;
                weatherState17 = currentWeatherState;
            }

            //get the max temp
            QString currentMaxTemp = currentTimeElement.firstChildElement("tx").text();

            if((counter == 0) |(maxDayTemp < currentMaxTemp.toDouble()))
            {
                maxDayTemp = currentMaxTemp.toDouble();
            }

            //get the min temp
            QString currentMinTemp = currentTimeElement.firstChildElement("tn").text();

            if((counter == 0) | (minDayTemp > currentMinTemp.toDouble()))
            {
                minDayTemp = currentMinTemp.toDouble();
            }

            //get the next time element
            currentTimeElement = currentTimeElement.nextSiblingElement("time");

            counter ++;
        }

        qDebug()<< QString::number(maxDayTemp);
        // QString curentDayStringRich = weekday[currentDateTime.dayOfWeek()] + "<br>" + "Vormittag: " + weatherText11 + "<br>" + "Nachmittag: " + weatherText17 +  "<br> Max: <b>" + QString::number(maxDayTemp) + "°C</b><br> Min: <b>" +  QString::number(minDayTemp)+ "°C</b>";
        QString curentDayStringRich ="<b>" + weekday[currentDateTime.dayOfWeek()] + "</b>" + "<br>" ;
        //curentDayStringRich.append("Niederschlag:<b> "+ rainProbability + "%</b>" "<br> Max: <b>" + QString::number(maxDayTemp) + QString::fromUtf8("°") +  "C</b><br> Min: <b>" +  QString::number(minDayTemp)+ QString::fromUtf8("°") +"C</b>");
        curentDayStringRich.append("<b> "+ rainProbability + "%</b>");
        curentDayStringRich.append("<br> <b>" + QString::number(minDayTemp) + QString::fromUtf8("°") +"C</b>");
        curentDayStringRich.append("-> <b>" + QString::number(maxDayTemp) + QString::fromUtf8("°") + "C</b>");


        m_labelListWeather.at(labelCounter)->setText(curentDayStringRich);
        labelCounter++;

        //set the text over the icons
        m_labelListWeather.at(labelCounter)->setText("Vormittag:<br>" + weatherText11);
        labelCounter++;

        m_labelListWeather.at(labelCounter)->setText("Nachmittag:<br>" + weatherText17);
        labelCounter++;

        //QFile *testFile =new QFile("://icons/d_0_L.png");
        //bool test = testFile->exists();

        //set the icons
        m_labelListWeather.at(labelCounter)->setPixmap(QPixmap(":/d/icons/d_" + weatherState11 + "_L.png").scaled(QSize(60,60),Qt::KeepAspectRatio));
        labelCounter++;
        m_labelListWeather.at(labelCounter)->setPixmap(QPixmap(":/n/icons/n_" + weatherState17 + "_L.png").scaled(QSize(60,60),Qt::KeepAspectRatio));

        labelCounter++;
        //get the next element
        currentDateElement =  currentDateElement.nextSiblingElement("date");
        counter ++;
    }

}


void MainWindow::generateLabels()
{

    //labels for the weather
    int Cols = 3;
    int Rows = 5; //1 row for first text, 2 for second text and 2 row for the icons

    for (int labelCounter = 0; labelCounter < Cols*Rows; ++labelCounter)
    {
        m_labelListWeather << new QLabel(this);
    }

    int position =0;
    // add the weather labels into the grid widget
    for (int dayCounter = 0; dayCounter < 3; ++dayCounter)
    {

        ui->gridLayoutWeather->addWidget(m_labelListWeather.at(position),0,dayCounter, 1, 1);

        m_labelListWeather.at(position)->setTextFormat(Qt::RichText);
        m_labelListWeather.at(position)->setStyleSheet("QLabel {font-size: 12pt; color: white }");
        position++;

        QGridLayout *iconLayout = new QGridLayout();
        ui->gridLayoutWeather->addLayout(iconLayout,1,dayCounter,1,1);

        //set text for "Vormittag" and "Nachmittag"
        iconLayout->addWidget(m_labelListWeather.at(position),0,0, 1, 1);
        m_labelListWeather.at(position)->setTextFormat(Qt::RichText);
        m_labelListWeather.at(position)->setStyleSheet("QLabel { color: white }");
        position++;
        iconLayout->addWidget(m_labelListWeather.at(position),0,1, 1, 1);
        m_labelListWeather.at(position)->setTextFormat(Qt::RichText);
        m_labelListWeather.at(position)->setStyleSheet("QLabel { color: white }");
        position++;

        //set the icons
        iconLayout->addWidget(m_labelListWeather.at(position),1,0, 1, 1);
        position++;

        iconLayout->addWidget(m_labelListWeather.at(position),1,1, 1, 1);

        position++;

    }


    //labels for the news
    int amountNews = m_settings.value("newsAmount").toInt();

    int counter = 0;
    while (true)
    {
        QString currentNewsAdressKey = "news" + QString::number(counter);

        if(m_settings.contains(currentNewsAdressKey)== true)
        {
            counter++;
        }
        else
        {
            break;
        }

    }

    int amountUrls = counter;


    //we need 2 lines per news (title +text)
    for(int labelCounter=0; labelCounter < amountNews*amountUrls*2; labelCounter++)
    {
        //generate new label
        m_labelListNews << new QLabel(this);
        //add label to the grid-layout
        ui->gridLayoutNews->addWidget(m_labelListNews.at(labelCounter),labelCounter,0,1,1);

        //apply the styles to the labels

        if(labelCounter % 2 ==0)
        {
            //label is a title
            m_labelListNews.at(labelCounter)->setStyleSheet("QLabel { color: white; font-weight: bold; font-size: 14pt }");
            m_labelListNews.at(labelCounter)->setTextFormat(Qt::RichText);
            m_labelListNews.at(labelCounter)->setFont(QFont("Comic Sans MS",14));
        }
        else
        {
            //label is a news text
            m_labelListNews.at(labelCounter)->setStyleSheet("QLabel { color: white; font-size: 10pt }");
            m_labelListNews.at(labelCounter)->setTextFormat(Qt::RichText);
            m_labelListNews.at(labelCounter)->setWordWrap(true);
            m_labelListNews.at(labelCounter)->setFont(QFont("Comic Sans MS",10));
        }
    }
}


void MainWindow::httpFinished(){


//    QNetworkReply *reply = qobject_cast(sender());
//    QByteArray receivedData =  reply->readAll();
//    QString receivedDataString(receivedData);


    m_waitForRefresh = false;
    QByteArray receivedData =  m_reply->readAll();
    QString receivedDataString(receivedData);


    //ui->textEditDates->setText(receivedDataString);



    if(m_httpModules.at(m_currentHttpModule).compare("weather")== 0)
    {
        parseWeather(receivedData);
    }
    else if(m_httpModules.at(m_currentHttpModule).compare("news")== 0)
    {
        parseNews(receivedData);
    }


    if(m_currentHttpModule >= m_httpModules.size()-1)
    {
        //go to the first element in the list

        m_currentHttpModule = 0;
    }
    else
    {
        m_currentHttpModule++;

    }

}

void MainWindow::videoPodcastFinished()
{
    qDebug() << "Video Podcast Url received:";

   // QNetworkReply *reply = qobject_cast(sender());
    QByteArray receivedData =  m_replyVideoPodcast->readAll();
    QString receivedDataString(receivedData);
    qDebug() << receivedDataString;

    parseVideoPodcast(receivedData);
}

/*
void MainWindow::on_pushButtonStart_clicked()
{
    //QUrl url = "http://api.wetter.com/forecast/weather/city/DE0007840/project/testqt/cs/99da8a0c7841156f23b9234011fd9ff5";
    QUrl url = QUrl("http://api.wetter.com/forecast/weather/city/DE0007840/project/testqt/cs/99da8a0c7841156f23b9234011fd9ff5");

    startRequest(url);
}
*/
void MainWindow::timerHttpFinished()
{

    //first check if we wait for an old request before starting a new one
    if (m_waitForRefresh == false)
    {
        m_waitForRefresh = true;
        QUrl url;


        if(m_httpModules.at(m_currentHttpModule).compare("weather")== 0)
        {
            url = QUrl(m_settings.value("weatherUrl"));
            startRequest(url,"NewsWeather");
        }
        else if(m_httpModules.at(m_currentHttpModule).compare("news")== 0)
        {

            QString currentNewsAdressKey = "news" + QString::number(m_newsUrlCounter);

            if(m_settings.contains(currentNewsAdressKey))
            {
                url = QUrl(m_settings.value(currentNewsAdressKey));

                m_newsUrlCounter++ ;

            }
            else
            {
                //no more news URLs are present, so start at the first url again
                m_newsUrlCounter =0;

                currentNewsAdressKey = "news" + QString::number(m_newsUrlCounter);
                url = QUrl(m_settings.value(currentNewsAdressKey));

                //reset the position counter for the ui-position
                m_newsPositionCounter = 0;

                m_newsUrlCounter++;
            }

            startRequest(url,"NewsWeather");
        }
    }

}

void MainWindow::timerPicturesFinished()
{
    showImages();
}

void MainWindow::parseVideoPodcast(QByteArray receivedXML)
{
    QDomDocument domDoc;

    domDoc.setContent(receivedXML);

    QDomElement rssElement = domDoc.namedItem("rss").toElement();
    QDomElement chanelElement = rssElement.firstChildElement("channel");
    QDomElement itemElement = chanelElement.firstChildElement("item");
    QDomElement enclosureElement = itemElement.firstChildElement("enclosure");


    if(rssElement.isNull())
    {
        qDebug() << "error parsing podcast";
    }


    m_videoPodscastUrl = enclosureElement.attribute("url", "null");



    //show the current videoPodcast
    showVideoPodcast();

}

void MainWindow::showVideoPodcast()
{

    //generate the command to start the omxPlayer
    QString omxPlayerCommand = "omxplayer -o hdmi" +  m_videoPodscastUrl + "--win \"";
    omxPlayerCommand.append(m_settings.value("omxTopLeftCornerX")+ " ");
    omxPlayerCommand.append(m_settings.value("omxTopLeftCornerY")+ " ");

    int rightBottomCornerX = m_settings.value("omxTopLeftCornerX").toInt()+ m_settings.value("omxTopLeftCornerWidth").toInt();
    int rightBottomCornerY = m_settings.value("omxTopLeftCornerY").toInt()+ m_settings.value("omxTopLeftCornerHeight").toInt();


    omxPlayerCommand.append(QString::number(rightBottomCornerX)+ " ");
    omxPlayerCommand.append(QString::number(rightBottomCornerY)+ "\"");


    QProcess p;

    p.start(omxPlayerCommand);


   // p.terminate();
    //p.kill();

    p.waitForFinished(-1);


    QString pStdout = p.readAllStandardOutput();
    QString pStdErr = p.readAllStandardError();

    qDebug() << pStdout << pStdErr;

    p.kill();


}

void MainWindow::parseNews(QByteArray receivedXML)
{
    QDomDocument domDoc;

    domDoc.setContent(receivedXML);

    QDomElement rssElement = domDoc.namedItem("rss").toElement();
    QDomElement channelElement = rssElement.firstChildElement("channel");

    if(rssElement.isNull())
    {
        qDebug() << "error parsing news";
    }

    //loop through all date-child nodes
    QDomElement currentNewsElement = channelElement.firstChildElement("item");


    int counter = 0; //counter for one news url

    while(!currentNewsElement.isNull())
    {
        //get the nes title
        QString newsTitle = currentNewsElement.firstChildElement("title").text();


        m_labelListNews.at(m_newsPositionCounter)->setText(newsTitle);
        m_newsPositionCounter ++;

        //get the news-message of the news
        QString newsContent = currentNewsElement.firstChildElement("description").text();

        m_labelListNews.at(m_newsPositionCounter)->setText(newsContent);
        m_newsPositionCounter ++;

        //get the next element
        currentNewsElement =  currentNewsElement.nextSiblingElement("item");

        //if(counter >= (m_settings.value("newsAmount").toInt())*2 )

        //check if the maximum amount of news-topics is reached
        if(counter >= (m_settings.value("newsAmount").toInt()-1))
        {
            break;
        }
        else
        {
            counter++;
        }
    }
}


void MainWindow::showImages()
{

    QPixmap frameOverlay = QPixmap(":/i/icons/frame2.png");

    QPixmap currentPic;


    if(m_currentPicCounter > m_picturePaths.count()-1)
    {
        //start with the first picture again
        m_currentPicCounter  = 0;
    }

    currentPic.load(m_picturePaths.at(m_currentPicCounter));
    m_currentPicCounter++;

    currentPic = currentPic.scaled(QSize(260,260),Qt::KeepAspectRatio);

    frameOverlay = frameOverlay.scaled(QSize(currentPic.width(), currentPic.height()),Qt::IgnoreAspectRatio);

    QPixmap result(currentPic.width(), currentPic.height());
    result.fill(Qt::transparent); // force alpha channel

    QPainter painter(&result);
    painter.drawPixmap(0,0, currentPic);
    painter.drawPixmap(0,0,frameOverlay);

    ui->labelPicture->setPixmap(result);

}

void MainWindow::readIniFile()
{

    QString pathToIniFile = QDir::currentPath();

    pathToIniFile.append("/settings.ini");


    QSettings settings(pathToIniFile, QSettings::IniFormat);

    //go through all groups
    const QStringList groups = settings.childGroups();
    foreach(const QString &childGroup, groups)
    {
        qDebug() << "group:" <<childGroup;
        settings.beginGroup(childGroup);
        //go through all keys in the group
        const QStringList childKeys = settings.childKeys();
        foreach (const QString &childKey, childKeys)
        {
            qDebug() <<childKey <<":" << settings.value(childKey).toString();
            //save the settings in the settings hash
            m_settings.insert(childKey, settings.value(childKey).toString());
        }
        settings.endGroup();

    }
}

void MainWindow::readImageFiles()
{
    //read the images from the folder specified in the .ini file
    QStringList nameFilter;
    nameFilter << "*.jpg" << "*.jpeg" << "*.bmp" << "*.png";
    QDir directory(m_settings.value("imagesFolder"));

    directory.setNameFilters(nameFilter);

    QFileInfoList tempFileInfoList =  directory.entryInfoList();

    foreach (QFileInfo tempFileInfo, tempFileInfoList)
    {

        m_picturePaths.append(tempFileInfo.absoluteFilePath());

    }

    //set the image counter
    m_currentPicCounter =0;
}

void MainWindow::timerTimeFinished()
{
    QTime tempCurrentTime =  QTime::currentTime();

    ui->labelTime->setText(QString::number(tempCurrentTime.hour()) + ":" );
    if(tempCurrentTime.minute() < 10)
    {
        ui->labelTime->setText(ui->labelTime->text() + "0"+ QString::number(tempCurrentTime.minute()));

    }
    else
    {
        ui->labelTime->setText(ui->labelTime->text() +  QString::number(tempCurrentTime.minute()));

    }

}


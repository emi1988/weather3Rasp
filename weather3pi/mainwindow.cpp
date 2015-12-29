#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtGui>
#include <QtNetwork>
#include <QDebug>
#include <QtXml/QDomDocument>
#include <QDate>
#include <QLabel>
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralWidget->setStyleSheet("background-color:black;");
    //ui->textEditDates->setStyleSheet("QLabel {font-size: 14pt; color: white }");

    generateLabels();

    currentModuleRefresh = weather;
    waitForRefresh = false;

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerFinished()));
    timer->start(4000);
}

void MainWindow::startRequest(QUrl url)
{
    reply = networkAccesManager.get(QNetworkRequest(url));

    connect(reply, SIGNAL(finished()), this , SLOT(httpFinished()));

}

/*alles anzeigen
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

void MainWindow::parseXML(QByteArray receivedXML)
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


        //set the weekday
        // labelList.at(labelCounter)->setText(weekday[currentDateTime.dayOfWeek()]);
        //labelCounter++;

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

            //set the time + weather text
            // labelList.at(labelCounter)->setText(currentTimeString + "\n" + currentWeatherTxtString + "\n Max:" + currentMaxTempString + "\n Min:" + currentMinTempString);

            //  collectedDayStringRich = collectedDayStringRich + "<hr>" + currentTimeString + "<br>" + currentWeatherTxtString + "<br> Max:<b>" + currentMaxTempString + "°C</b><br> Min:<b>" + currentMinTempString + "°C</b><br><br>";

            // labelList.at(labelCounter)->setWordWrap(true);
            //labelCounter++;

            //get the next time element
            currentTimeElement = currentTimeElement.nextSiblingElement("time");

            counter ++;
        }

        // QString maxTempString = QString(maxDayTemp);

        qDebug()<< QString::number(maxDayTemp);
        // QString curentDayStringRich = weekday[currentDateTime.dayOfWeek()] + "<br>" + "Vormittag: " + weatherText11 + "<br>" + "Nachmittag: " + weatherText17 +  "<br> Max: <b>" + QString::number(maxDayTemp) + "°C</b><br> Min: <b>" +  QString::number(minDayTemp)+ "°C</b>";
        QString curentDayStringRich ="<b>" + weekday[currentDateTime.dayOfWeek()] + "</b>" + "<br>" + "Niederschlag:<b> "+ rainProbability + "%</b>" "<br> Max: <b>" + QString::number(maxDayTemp) + QString::fromUtf8("°") +  "C</b><br> Min: <b>" +  QString::number(minDayTemp)+ QString::fromUtf8("°") +"C</b>";


        labelList.at(labelCounter)->setText(curentDayStringRich);
        labelCounter++;

        //set the text over the icons
        labelList.at(labelCounter)->setText("Vormittag:<br>" + weatherText11);
        labelCounter++;

        labelList.at(labelCounter)->setText("Nachmittag:<br>" + weatherText17);
        labelCounter++;

        QFile *testFile =new QFile("://icons/d_0_L.png");
        bool test = testFile->exists();

        //set the icons
        labelList.at(labelCounter)->setPixmap(QPixmap(":/d/icons/d_" + weatherState11 + "_L.png").scaled(QSize(60,60),Qt::KeepAspectRatio));
        labelCounter++;
        labelList.at(labelCounter)->setPixmap(QPixmap(":/n/icons/n_" + weatherState17 + "_L.png").scaled(QSize(60,60),Qt::KeepAspectRatio));

        labelCounter++;
        //get the next element
        currentDateElement =  currentDateElement.nextSiblingElement("date");
        counter ++;
    }

}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::generateLabels()
{

    int Cols = 3;
    int Rows = 5; //1 row for first text, 2 for second text and 2 row for the icons

    for (int lableCounter = 0; lableCounter < Cols*Rows; ++lableCounter)
    {
        labelList << new QLabel(this);

    }


    int position =0;

    /* for (int dayCounter = 0; dayCounter < 3; ++dayCounter)
    {

        ui->gridLayout->addWidget(labelList.at(position),0,dayCounter*2, 1, 1);
        // labelList.at(position)->setFrameStyle(QFrame::Box | QFrame::Plain);
        //labelList.at(position)->setFrameStyle(QFrame::Panel | QFrame::Plain);
        //labelList.at(position)->setLineWidth(2);
        labelList.at(position)->setTextFormat(Qt::RichText);
        labelList.at(position)->setStyleSheet("QLabel { color: white }");
        position++;

        //set the icons
        ui->gridLayout->addWidget(labelList.at(position),1,dayCounter*2, 1, 1);
        position++;
        ui->gridLayout->addWidget(labelList.at(position),1,dayCounter+1, 1, 1);
       //test: labelList.at(position)->setPixmap(QPixmap(":/icons/d_0_L.png"));


        position++;

    }
    */

    for (int dayCounter = 0; dayCounter < 3; ++dayCounter)
    {

        ui->gridLayoutWeather->addWidget(labelList.at(position),0,dayCounter, 1, 1);
        // labelList.at(position)->setFrameStyle(QFrame::Box | QFrame::Plain);
        //labelList.at(position)->setFrameStyle(QFrame::Panel | QFrame::Plain);
        //labelList.at(position)->setLineWidth(2);
        labelList.at(position)->setTextFormat(Qt::RichText);
        labelList.at(position)->setStyleSheet("QLabel {font-size: 12pt; color: white }");
        position++;

        QGridLayout *iconLayout = new QGridLayout();
        ui->gridLayoutWeather->addLayout(iconLayout,1,dayCounter,1,1);

        //set text for "Vormittag" and "Nachmittag"
        iconLayout->addWidget(labelList.at(position),0,0, 1, 1);
        labelList.at(position)->setTextFormat(Qt::RichText);
        labelList.at(position)->setStyleSheet("QLabel { color: white }");
        position++;
        iconLayout->addWidget(labelList.at(position),0,1, 1, 1);
        labelList.at(position)->setTextFormat(Qt::RichText);
        labelList.at(position)->setStyleSheet("QLabel { color: white }");
        position++;

        //set the icons
        iconLayout->addWidget(labelList.at(position),1,0, 1, 1);
        //ui->gridLayout->addWidget(labelList.at(position),1,dayCounter, 1, 1);
        position++;

        iconLayout->addWidget(labelList.at(position),1,1, 1, 1);

        //ui->gridLayout->addWidget(labelList.at(position),1,dayCounter, 1, 1);
        //test: labelList.at(position)->setPixmap(QPixmap(":/icons/d_0_L.png"));


        position++;

    }


    //    for (int rowCounter = 0; rowCounter < amountRows; ++rowCounter)
    //    {
    //        for (int dayCounter = 0; dayCounter < amountDays; ++dayCounter)
    //        {
    //            ui->gridLayout->addWidget(labelList.at(position),rowCounter,dayCounter, 1, 1);
    //            position++;
    //        }

    //    }

    //ui->gridLayout->addWidget(dayList.at(0)->at(0),0,0, 1, 1);

    //    for (int lableCounter = 0; lableCounter < 5; ++lableCounter)
    //    {
    //        labelList << new QLabel(this);
    //    }

    //    for (int dayCounter = 0; dayCounter < 2; ++dayCounter)
    //    {
    //       //FEHLER
    //        dayList.append(&labelList);
    //    }

    //    //QList<QLabel*> *currentList = dayList.at(0);

    //    dayList.at(0)->at(0)->setText("hallo10");
    //    dayList.at(0)->at(1)->setText("hallo11");
    //    dayList.at(0)->at(2)->setText("hallo12");
    //    dayList.at(0)->at(3)->setText("hallo13");
    //    dayList.at(0)->at(4)->setText("hallo14");

    //    dayList.at(1)->at(0)->setText("hallo20");
    //    dayList.at(1)->at(1)->setText("hallo21");
    //    dayList.at(1)->at(2)->setText("hallo22");
    //    dayList.at(1)->at(3)->setText("hallo23");
    //    dayList.at(1)->at(4)->setText("hallo24");




    //    for (int dayCounter = 0; dayCounter < 5; ++dayCounter)
    //    {
    //       // lableList << QList<QLabel>();
    //        //lableList.append(QList<QLabel*>());
    //        lableList.append(new QList<QLabel*>());

    //        for (int lableCounter = 0; lableCounter < 5; ++lableCounter)
    //        {

    //            lableList.at(dayCounter) = new QLabel();

    //            // lableList.at(dayCounter).at(lableCounter) << new QLabel(this);

    //           // QLabel curentLabel << new QLabel(this);
    //            //lableList.at(dayCounter).append(curentLabel);
    //           // QLabel myLabel = new QLabel();
    //           // lableList[dayCounter].append(myLabel);

    //           // lableList.at(dayCounter) << new QLabel(this);
    //            //lableList.at(dayCounter).at(lableCounter) << new QLabel(this);
    //            //lableList.[dayCounter].[lableCounter] << new QLabel(this);

    ////            day1Labels << new QLabel(this);
    ////            day2Labels << new QLabel(this);
    ////            day3Labels << new QLabel(this);
    //        }
    //    }

    //    day1Labels.at(1)->setText("hallo test");
    //    day2Labels.at(0)->setText("hallo 56464");


    //    ui->gridLayout->addWidget(day1Labels.at(1));
    //    ui->gridLayout->addWidget(day2Labels.at(0));
}

void MainWindow::downloadFile()
{

}

void MainWindow::httpFinished(){

    waitForRefresh = false;
    QByteArray receivedData =  reply->readAll();
    QString receivedDataString(receivedData);

    //ui->textEditDates->setText(receivedDataString);


    switch(currentModuleRefresh)
    {
    case weather:
    parseXML(receivedData);


        break;

    case news:

        parseNews(receivedData);
        break;
    }

    if(currentModuleRefresh == count-1)
    {
        //go to the first element
        currentModuleRefresh = weather;
    }
    else
    {

        currentModuleRefresh ++;
    }
}

/*
void MainWindow::on_pushButtonStart_clicked()
{
    //QUrl url = "http://api.wetter.com/forecast/weather/city/DE0007840/project/testqt/cs/99da8a0c7841156f23b9234011fd9ff5";
    QUrl url = QUrl("http://api.wetter.com/forecast/weather/city/DE0007840/project/testqt/cs/99da8a0c7841156f23b9234011fd9ff5");

    startRequest(url);
}
*/
void MainWindow::timerFinished()
{

    if (waitForRefresh == false)
    {
       waitForRefresh = true;
       QUrl url;
       switch(currentModuleRefresh)
        {
        case weather:
             url = QUrl("http://api.wetter.com/forecast/weather/city/DE0007840/project/testqt/cs/99da8a0c7841156f23b9234011fd9ff5");
            startRequest(url);

            break;

        case news:


            url = QUrl("http://www.spiegel.de/netzwelt/index.rss");
            startRequest(url);

            break;


        }
    }


}

void MainWindow::parseNews(QByteArray receivedXML)
{
    QDomDocument domDoc;

    domDoc.setContent(receivedXML);

    QDomElement rssElement = domDoc.namedItem("rss").toElement();
    QDomElement channelElement = rssElement.firstChildElement("channel");

    if(channelElement.isNull())
    {
       // ui->textEditDates->append("error");
    }

    //loop through all date-child nodes
    QDomElement currentNewsElement = channelElement.firstChildElement("item");


    int counter = 0;
    while(!currentNewsElement.isNull())
    {

        QString newsTitle = currentNewsElement.firstChildElement("title").text();

        QLabel *tempLabel = new QLabel(this);
        tempLabel->setStyleSheet("QLabel { color: white; font-weight: bold; font-size: 14pt }");
        tempLabel->setTextFormat(Qt::RichText);

        tempLabel->setText(newsTitle);

        ui->gridLayoutNews->addWidget(tempLabel,counter,0,1,1);

        QString newsContent = currentNewsElement.firstChildElement("description").text();

        counter ++;
        QLabel *tempLabel2 = new QLabel(this);
        tempLabel2->setStyleSheet("QLabel { color: white; font-size: 10pt }");
        tempLabel2->setTextFormat(Qt::RichText);
        tempLabel2->setWordWrap(true);

        tempLabel2->setText(newsContent);

        ui->gridLayoutNews->addWidget(tempLabel2,counter,0,1,1);


//        labelList.at(labelCounter)->setText(curentDayStringRich);
//        labelCounter++;

//        //set the text over the icons
//        labelList.at(labelCounter)->setText("Vormittag:<br>" + weatherText11);
//        labelCounter++;

//        labelList.at(labelCounter)->setText("Nachmittag:<br>" + weatherText17);
//        labelCounter++;

//        QFile *testFile =new QFile("://icons/d_0_L.png");
//        bool test = testFile->exists();

//        //set the icons
//        labelList.at(labelCounter)->setPixmap(QPixmap(":/d/icons/d_" + weatherState11 + "_L.png").scaled(QSize(60,60),Qt::KeepAspectRatio));
//        labelCounter++;
//        labelList.at(labelCounter)->setPixmap(QPixmap(":/n/icons/n_" + weatherState17 + "_L.png").scaled(QSize(60,60),Qt::KeepAspectRatio));

//        labelCounter++;
//        //get the next element
      currentNewsElement =  currentNewsElement.nextSiblingElement("item");
      counter ++;

      if(counter > 9)
      {
          break;
      }
   }
}


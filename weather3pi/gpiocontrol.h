#ifndef GPIOCONTROL_H
#define GPIOCONTROL_H

#define IN  0
#define OUT 1

#define LOW  0
#define HIGH 1

#define PIN  24 /* P1-18 */
#define POUT 4  /* P1-07 */

#include <QFileSystemWatcher>
#include <QWidget>
#include <QList>

#include "gpiocontrol.h"


class gpioControl : public QObject
{
    Q_OBJECT

public:
    gpioControl();
    int GPIOExport(int pin);
    int GPIOUnexport(int pin);

    int GPIODirection(int pin, int dir);

    int GPIORead(int pin);
    int GPIOWrite(int pin, int value);

    int GPIOSetWatch(QList<int> pins);

    int GPIOInitInputs(QList<int> pins);

private:

    QFileSystemWatcher *m_FileSystemWatcher;

    QList<int> watchedGpioPins;

public slots:
    void checkGpio();

 signals:

    void gpioInputChanged(int pin);



};

#endif // GPIOCONTROL_H

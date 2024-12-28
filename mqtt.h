#ifndef MQTT_H
#define MQTT_H
#include <QObject>
#include <QMainWindow>
#include <qmqttclient.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MQTT : public QMqttClient
{
    Q_OBJECT
private:
    Ui::MainWindow *mainUi;
    QString hostname;
    int port;
    QString topicSubscribe;
    QString topicSubscribe_2;
    QString topicSubscribe_3;
    QString topicSubscribe_4;

public:
    MQTT(QObject *parent, Ui::MainWindow *ui,QString host, int port,
         QString topic,QString topic2,QString topic3,QString topic4 );
    void init();
    void connecter();
    void souscrire();
private slots:

};
#endif // MQTT_H

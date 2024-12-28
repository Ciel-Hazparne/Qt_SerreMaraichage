#include <QObject>
#include <QMainWindow>
#include <QtTest>
#include <qmqttclient.h>
#include "mqtt.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"


MQTT::MQTT(QObject *parent, Ui::MainWindow *ui, QString host,int port, QString topic,
           QString topic2,QString topic3,QString topic4) :
    QMqttClient(parent),
    mainUi(ui),
    hostname(host),
    port(port),
    topicSubscribe(topic),      //serre temp
    topicSubscribe_2(topic2),   //serre humid
    topicSubscribe_3(topic3),   //champ temp
    topicSubscribe_4(topic4)    //champ humid

{

}

void MQTT::init(){
    this->setHostname(hostname);
    this->setPort(port);
}

void MQTT::connecter(){
    while (this->state() == QMqttClient::Disconnected) {
        mainUi->connecter->insertPlainText("Déconnecté -> Connexion\n");
        this->connectToHost();
        QTest::qWait(1000);
    }

    mainUi->connecter->insertPlainText("Vous êtes connecté\n");
  }

void MQTT::souscrire(){
   auto subscription = this->subscribe(topicSubscribe);
   auto subscription_2 = this->subscribe(topicSubscribe_2);
   auto subscription_3 = this->subscribe(topicSubscribe_3);
   auto subscription_4 = this->subscribe(topicSubscribe_4);


   while (!subscription)
    {

        subscription = this->subscribe(topicSubscribe);
    }


   while (!subscription_2)
    {
        subscription_2 = this->subscribe(topicSubscribe_2);
    }

   while (!subscription_3)
   {
       subscription_3 = this->subscribe(topicSubscribe_3);
   }


  while (!subscription_4)
   {
        subscription_4 = this->subscribe(topicSubscribe_4);
   }
}













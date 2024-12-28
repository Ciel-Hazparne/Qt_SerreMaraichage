/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qmqttclient.h>
#include "mqtt.h"
#include "httprest.h"
#include "constantes.h"
#include <QDateTime>
#include "modbusunitronics.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mqttClient(new MQTT(this, ui, URL_BROKER, PORT_BROKER, TOPIC_SUBSCRIBE_serre_T,
                        TOPIC_SUBSCRIBE_serre_H,TOPIC_SUBSCRIBE_champ_T,TOPIC_SUBSCRIBE_champ_H )),
    timer1(new QTimer), httpRest(new HttpRest),commMODBUS(new ModbusUnitronics(this, ui))

{
    ui->setupUi(this);
    connect(timer1, &QTimer::timeout, this, &MainWindow::finTimer1);
    timer1->start(1000);
    connect(mqttClient, &QMqttClient::messageReceived, this, &MainWindow::messageMQTTRecu);
    connect(commMODBUS, &ModbusUnitronics::vitesseLue, this, &MainWindow::donneesMODBUSLues);
}


void MainWindow::finTimer1()
{
    mqttClient->init();
    mqttClient->connecter();
    mqttClient->souscrire();
    timer1->stop();
}

void MainWindow::messageMQTTRecu(const QByteArray &message, const QMqttTopicName &topic)
{



    if (topic.name()=="maraichage/Temp_serre")
   {
       const QString content =  message ;
       ui->temp_serre->clear();
       ui->temp_serre->insertPlainText(content);
   }

   else if (topic.name()=="maraichage/Humid_serre")
   {
       const QString content = message ;
       ui->humid_serre->clear();
       ui->humid_serre->insertPlainText(content);
    }

   else if (topic.name()=="maraichage/Humid_champ")
   {
       const QString content = message ;
       ui->humid_champ->clear();
       ui->humid_champ->insertPlainText(content);
    }

   else if (topic.name()=="maraichage/Temp_champ")
   {
       const QString content = message ;
       ui->temp_champ->clear();
       ui->temp_champ->insertPlainText(content);
   }
}


MainWindow::~MainWindow()
{
    delete ui;
    delete mqttClient;
    delete httpRest;
    delete commMODBUS;
}


void MainWindow::on_pushButton1_clicked()
{
    QString Date = QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss");
    QString donnee_temp_champ = "{";
    donnee_temp_champ.append("\n");
    donnee_temp_champ.append("\"valeur\":");
    donnee_temp_champ.append(ui->temp_champ->toPlainText());
    donnee_temp_champ.append(",");
    donnee_temp_champ.append("\n");
    donnee_temp_champ.append("\"maintenance\":\"oui\",");
    donnee_temp_champ.append("\n");
    donnee_temp_champ.append("\"createdAt\":\"");
    donnee_temp_champ.append(Date);
    donnee_temp_champ.append("\""",");
    donnee_temp_champ.append("\n");
    donnee_temp_champ.append("\"libelleMesures\":\"/api/libelle_mesures/1\"");
    donnee_temp_champ.append("\n");
    donnee_temp_champ.append("}");
    //httpRest->requeteRestPOST("http://10.0.5.254:8000/api/mesures",donnee_temp_champ);
    httpRest->requeteRestPOST("https://jauste.free.beeceptor.com",donnee_temp_champ);
    connect(httpRest->getReply(), &QNetworkReply::finished,
    this, &MainWindow::restFinishedPOST);

}


void MainWindow::on_pushButton2_clicked()
{
        QString Date = QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss");
        QString donnee_humid_champ = "{";
        donnee_humid_champ.append("\n");
        donnee_humid_champ.append("\"valeur\":");
        donnee_humid_champ.append(ui->humid_champ->toPlainText());
        donnee_humid_champ.append(",");
        donnee_humid_champ.append("\n");
        donnee_humid_champ.append("\"maintenance\":\"oui\",");
        donnee_humid_champ.append("\n");
        donnee_humid_champ.append("\"createdAt\":\"");
        donnee_humid_champ.append(Date);
        donnee_humid_champ.append("\""",");
        donnee_humid_champ.append("\n");
        donnee_humid_champ.append("\"libelleMesures\":\"/api/libelle_mesures/2\"");
        donnee_humid_champ.append("\n");
        donnee_humid_champ.append("}");
        httpRest->requeteRestPOST("https://jauste.free.beeceptor.com",donnee_humid_champ);
        //httpRest->requeteRestPOST("http://10.0.5.254:8000/api/mesures",donnee_humid_champ);
        connect(httpRest->getReply(), &QNetworkReply::finished,
        this, &MainWindow::restFinishedPOST);
}


void MainWindow::on_pushButton3_clicked()
{
    QString Date = QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss");
    QString donnee_temp_serre = "{";
    donnee_temp_serre.append("\n");
    donnee_temp_serre.append("\"valeur\":");
    donnee_temp_serre.append(ui->temp_serre->toPlainText());
    donnee_temp_serre.append(",");
    donnee_temp_serre.append("\n");
    donnee_temp_serre.append("\"maintenance\":\"oui\",");
    donnee_temp_serre.append("\n");
    donnee_temp_serre.append("\"createdAt\":\"");
    donnee_temp_serre.append(Date);
    donnee_temp_serre.append("\""",");
    donnee_temp_serre.append("\n");
    donnee_temp_serre.append("\"libelleMesures\":\"/api/libelle_mesures/4\"");
    donnee_temp_serre.append("\n");
    donnee_temp_serre.append("}");    
    httpRest->requeteRestPOST("http://10.0.5.254:8000/api/mesures",donnee_temp_serre);
    connect(httpRest->getReply(), &QNetworkReply::finished,
    this, &MainWindow::restFinishedPOST);        
}


void MainWindow::on_pushButton4_clicked()
{
    QString Date = QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss");
    QString donnee_humid_serre = "{";
    donnee_humid_serre.append("\n");
    donnee_humid_serre.append("\"valeur\":");
    donnee_humid_serre.append(ui->humid_serre->toPlainText());
    donnee_humid_serre.append(",");
    donnee_humid_serre.append("\n");
    donnee_humid_serre.append("\"maintenance\":\"oui\",");
    donnee_humid_serre.append("\n");
    donnee_humid_serre.append("\"createdAt\":\"");
    donnee_humid_serre.append(Date);
    donnee_humid_serre.append("\""",");
    donnee_humid_serre.append("\n");
    donnee_humid_serre.append("\"libelleMesures\":\"/api/libelle_mesures/3\"");
    donnee_humid_serre.append("\n");
    donnee_humid_serre.append("}");
    httpRest->requeteRestPOST("http://10.0.5.254:8000/api/mesures",donnee_humid_serre);
    connect(httpRest->getReply(), &QNetworkReply::finished,
    this, &MainWindow::restFinishedPOST);
}

void MainWindow::restFinishedPOST()     // fin de réception
{
    QByteArray *dataBuffer = httpRest->getDataBuffer();
    qDebug()<< dataBuffer->toStdString().c_str();
    dataBuffer->clear();
}

void MainWindow::on_write_bouton_clicked()
{
    commMODBUS->ecrireModbus(ui->valeur_consigne->toPlainText().toInt());
}

void MainWindow::on_read_bouton_clicked()
{
    commMODBUS->lireModbus();
}


void MainWindow::donneesMODBUSLues(int vitesse)
{
    QString textVitesse = QString::number(vitesse) ;
    ui->valeur_consigne->setPlainText(textVitesse);
}



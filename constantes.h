#ifndef CONSTANTES_H
#define CONSTANTES_H

#include <QString>
#include <QModbusDataUnit>

const QString URL_BROKER = "185.182.169.30";

const int PORT_BROKER = 1883;

const QString TOPIC_SUBSCRIBE_serre_T = "maraichage/Temp_serre";//4

const QString TOPIC_SUBSCRIBE_serre_H = "maraichage/Humid_serre";//3

const QString TOPIC_SUBSCRIBE_champ_T = "maraichage/Temp_champ";//1

const QString TOPIC_SUBSCRIBE_champ_H = "maraichage/Humid_champ";//2

const QString URL_SERVEUR = "http://10.0.0.77:502";

const QModbusDataUnit::RegisterType TYPE_REGISTRE = QModbusDataUnit::HoldingRegisters;

const int NUMERO_MOT_A_LIRE = 0;

const int NOMBRE_MOTS_A_LIRE = 1;

const int ADRESSE_SERVEUR = 1;


#endif // CONSTANTES_H

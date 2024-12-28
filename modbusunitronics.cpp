#include <QObject>
#include <QMainWindow>
#include <QModbusTcpClient>
#include <QStandardItemModel>
#include <QUrl>

#include "constantes.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "modbusunitronics.h"


ModbusUnitronics::ModbusUnitronics(QObject *parent,
                                   Ui::MainWindow *ui)
    : QObject(parent)
    , mainUi(ui)
    , modbusDevice(new QModbusTcpClient(this))
{
    initModbus();
}

ModbusUnitronics::~ModbusUnitronics()

{
    if (modbusDevice)
        modbusDevice->disconnectDevice();
    delete modbusDevice;
    delete mainUi;
}

void ModbusUnitronics::initModbus()
{
    if (!modbusDevice) {

        mainUi->valeur_lue->setText(tr("Could not create Modbus client.").arg(5000));
    }

    const QUrl url = QUrl::fromUserInput(URL_SERVEUR);
    modbusDevice->setConnectionParameter(QModbusDevice::NetworkPortParameter, url.port());
    modbusDevice->setConnectionParameter(QModbusDevice::NetworkAddressParameter, url.host());

    modbusDevice->setTimeout(1000);
    modbusDevice->setNumberOfRetries(3);

    if (!modbusDevice->connectDevice()) {       
        mainUi->valeur_lue->setText(tr("Connect failed: ") + modbusDevice->errorString().arg(5000));
    }
}

void ModbusUnitronics::ecrireModbus(int vitesse)
{
    if (modbusDevice->state() != QModbusDevice::ConnectedState) {
        if (!modbusDevice->connectDevice()) {
            mainUi->valeur_lue->setText(tr("Connect failed: ") + modbusDevice->errorString().arg(5000));

        }
    }
     mainUi->valeur_lue->clear();

    QModbusDataUnit writeUnit = QModbusDataUnit(TYPE_REGISTRE, NUMERO_MOT_A_LIRE, NOMBRE_MOTS_A_LIRE);
    writeUnit.setValue(0, quint16(vitesse));

    if (auto *reply = modbusDevice->sendWriteRequest(writeUnit, ADRESSE_SERVEUR))
    {
        if (!reply->isFinished())
        {
            connect(reply, &QModbusReply::finished, this, [reply,this]()    //fonction lambda
            {
                if (reply->error() == QModbusDevice::ProtocolError)
                {
                    mainUi->valeur_lue->setText(tr("Write response error: %1 (Mobus exception: 0x%2)")
                        .arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16).arg(5000));
                }
                else if (reply->error() != QModbusDevice::NoError)
                {
                    mainUi->valeur_lue->setText(tr("Write response error: %1 (code: 0x%2)").
                        arg(reply->errorString()).arg(reply->error(), -1, 16).arg(5000));

                }
                reply->deleteLater();
            });
        }
        else
        {
            reply->deleteLater();
        }
    }
    else
    {
        mainUi->valeur_lue->setText(tr("Write error: ") + modbusDevice->errorString().arg(5000));
    }

}
void ModbusUnitronics::lireModbus()
{
    if (modbusDevice->state() != QModbusDevice::ConnectedState) {
        if (!modbusDevice->connectDevice()) {
            mainUi->valeur_lue->setText(tr("Connect failed: ") + modbusDevice->errorString().arg(5000));

        }
    }

    mainUi->valeur_lue->clear();

    if (auto *reply = modbusDevice->sendReadRequest(QModbusDataUnit(TYPE_REGISTRE, NUMERO_MOT_A_LIRE, NOMBRE_MOTS_A_LIRE), ADRESSE_SERVEUR))
    {
        if (!reply->isFinished())
            connect(reply, &QModbusReply::finished, this, &ModbusUnitronics::slotReadReady);
        else
            delete reply; // broadcast replies return immediately
    }
    else
    {

        mainUi->valeur_lue->setText(tr("Read error: ") + modbusDevice->errorString().arg(5000));
    }
}
void ModbusUnitronics::slotReadReady()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply) return;

    if (reply->error() == QModbusDevice::NoError) {
        const QModbusDataUnit unit = reply->result();
        int vitesse = unit.value(0);
        emit (vitesseLue(vitesse));
        mainUi->valeur_lue->setText(tr("Vitesse lue: ") + QString::number(vitesse));
    }
    reply->deleteLater();
}


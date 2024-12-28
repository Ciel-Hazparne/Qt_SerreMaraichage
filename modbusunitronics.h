#ifndef MODBUSUNITRONICS_H
#define MODBUSUNITRONICS_H
#include <QObject>
#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QModbusClient;

class ModbusUnitronics : public QObject
{
    Q_OBJECT
private:
    Ui::MainWindow *mainUi;
    QModbusClient *modbusDevice;

signals:
    void vitesseLue (int vitesse);

public:
     ModbusUnitronics(QObject *parent,Ui::MainWindow *ui);
    ~ModbusUnitronics();
    void lireModbus();
    void slotReadReady();
    void ecrireModbus(int vit);
    void initModbus();
    void connectModbus();
};

#endif // MODBUSUNITRONICS_H


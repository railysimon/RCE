#ifndef WINDOW_H
#define WINDOW_H

#include <QDialog>
#include <QUdpSocket>
#include <QHostAddress>
#include <QTextEdit>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QVector>

class Window : public QDialog
{
    Q_OBJECT

public:
    Window(QWidget *parent = 0);
    ~Window();

private:
        QUdpSocket *socket;
        QTextEdit *terminal;
        QLineEdit *command;
        QComboBox *box;

        QVector<QHostAddress> addresses;

private:
        void Layout();

private slots:
                void SendCommand(QString command, QHostAddress address, int number);
                void Read();
                void EnterPress();
                void GetAddresses();
};

#endif // WINDOW_H

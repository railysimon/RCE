#include "window.h"

Window::Window(QWidget *parent) : QDialog(parent)
{
    socket = new QUdpSocket(this);
    socket->bind(2323);
    connect(socket, SIGNAL(readyRead()), this, SLOT(Read()));

    Layout();
}

Window::~Window()
{

}

void Window::Layout()
{
    terminal = new QTextEdit;
    terminal->setReadOnly(true);
    terminal->setStyleSheet("color: white; font-style: italic;");

    command = new QLineEdit;
    command->setStyleSheet("color: white; font-weight: bold;");
    connect(command, SIGNAL(returnPressed()), this, SLOT(EnterPress()));

    box = new QComboBox;

    for(int i=0; i<addresses.size(); i++)
        box->addItem(addresses[i].toString());

    QPushButton *get = new QPushButton("Addresses");
    connect(get, SIGNAL(clicked(bool)), this, SLOT(GetAddresses()));

    QPushButton *push = new QPushButton("Push");
    connect(push, SIGNAL(clicked(bool)), this, SLOT(EnterPress()));

    QHBoxLayout *content_lay = new QHBoxLayout;
    content_lay->addWidget(terminal);

    QHBoxLayout *bottom_lay = new QHBoxLayout;
    bottom_lay->addWidget(push);
    bottom_lay->addWidget(command);
    bottom_lay->addWidget(get);
    bottom_lay->addWidget(box);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(content_lay);
    layout->addLayout(bottom_lay);

    this->setLayout(layout);
    this->setStyleSheet("background: rgb(56, 57, 58);");
    this->setFixedSize(500, 300);
}

void Window::SendCommand(QString command, QHostAddress address, int number)
{
    QByteArray datagram;
    QDataStream out(&datagram, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_7);

    out << int(number) << command;

    socket->writeDatagram(datagram, address, 2424);
}

void Window::Read()
{
    QByteArray datagram;
    QHostAddress address;

    do
        {
            datagram.resize(socket->pendingDatagramSize());
            socket->readDatagram(datagram.data(), datagram.size(), &address);

        }
    while(socket->hasPendingDatagrams());

    QString message;
    int number;

    QDataStream in(&datagram, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_7);
    in >> number >> message;

    if(!number && message == "Reply")
    {
        addresses.push_back(address);
        box->clear();

        for(int i=0; i<addresses.size(); i++)
            box->addItem(addresses[i].toString());
    }
    else
        terminal->append(message);
}

void Window::EnterPress()
{
    terminal->append(command->text());
    SendCommand(command->text(), addresses[box->currentIndex()], 1);
    command->clear();
}

void Window::GetAddresses()
{
    addresses.clear();
    SendCommand("Request", QHostAddress::Broadcast, 0);
}

#include "clientGUI.h"
#include <QHostAddress>
#include <QMessageBox>

ClientGUI::ClientGUI(QWidget *parent) :
QMainWindow(parent)
{
	qDebug("Client UI setup");
	setupUi(this);
	move(100,22);
	
	m_tcpSocket = new QTcpSocket(this);
		
	// network widgets
	connect(pushButtonConnect, SIGNAL(clicked()), this, SLOT(hostAction()));
	connect(pushButtonSend, SIGNAL(clicked()), this, SLOT(sendToHost()));
	connect(lineEditCommand, SIGNAL(returnPressed()), this, SLOT(sendToHost()));
	
	connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(readFromHost()));
	connect(m_tcpSocket, SIGNAL(disconnected()), this, SLOT(hostDisconnect()));
	connect(m_tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
             this, SLOT(hostError(QAbstractSocket::SocketError)));

	pushButtonSend->setEnabled(false);
	pushButtonConnect->setText("Connect");
	
	ipAddress = "NULL";
	lineEditAddress->setText(QHostAddress(QHostAddress::LocalHost).toString());
	lineEditPort->setText(QString::number(50657));
	lineEditPort->setFocus();
}

ClientGUI::~ClientGUI()
{
	qDebug("Client shutdown");
	delete m_tcpSocket;
}

void ClientGUI::hostAction()
{
	if(ipAddress == "NULL"){
		pushButtonSend->setEnabled(true);
		pushButtonConnect->setText("Disconnect");
		lineEditCommand->setEnabled(true);
		ipAddress = lineEditAddress->text();
		m_tcpSocket->abort();
		m_tcpSocket->connectToHost(ipAddress, lineEditPort->text().toInt());
		textConsole->append("Connect to Host:"+ipAddress);
	}
	else{
		m_tcpSocket->abort();
		m_tcpSocket->disconnectFromHost();
	}
}
void ClientGUI::hostDisconnect()
{
	ipAddress = "NULL";
	pushButtonSend->setEnabled(false);
	pushButtonConnect->setText("Connect");
	lineEditCommand->setEnabled(false);
	textConsole->append("Host disconnected");
}

void ClientGUI::sendToHost()
{
	QByteArray block;
	QDataStream outStream(&block, QIODevice::WriteOnly);
	outStream.setVersion(QDataStream::Qt_4_0);
	outStream << (quint16)0; // reserve room for total data block size being sent
	outStream << (quint8)STRING; // set the command
	outStream << lineEditCommand->text(); // get the text from the command line
	outStream.device()->seek(0);
	outStream << (quint16)(block.size() - sizeof(quint16)); // calculate the size of the data
	textConsole->append(QString("%1 bytes out").arg(block.size() - sizeof(quint16)));
	m_tcpSocket->write(block);
}

void ClientGUI::readFromHost()
{
	textConsole->append("reading data");
}

void ClientGUI::hostError(QAbstractSocket::SocketError socketError)
{
	switch (socketError) {
	     case QAbstractSocket::RemoteHostClosedError:
	         break;
	     case QAbstractSocket::HostNotFoundError:
	         QMessageBox::information(this, tr("Rover Client"),
	                                  tr("The host was not found. Please check the "
	                                     "host name and port settings."));
	         break;
	     case QAbstractSocket::ConnectionRefusedError:
	         QMessageBox::information(this, tr("Rover Client"),
	                                  tr("The connection was refused by the peer. "
	                                     "Make sure the RoverSim server is running, "
	                                     "and check that the host name and port "
	                                     "settings are correct."));
	         break;
	     default:
	         QMessageBox::information(this, tr("Rover Client"),
	                                  tr("The following error occurred: %1.")
	                                  .arg(m_tcpSocket->errorString()));
	     }

		this->hostDisconnect();
}
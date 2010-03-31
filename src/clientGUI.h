#ifndef ClientGUI_H
#define ClientGUI_H

#include <QtGui/QMainWindow>
#include <QTcpSocket>
#include <QDataStream>
#include "ui_clientGUI.h"

// the following enum is used under the sendToHost switch
enum serverCommand {
		ROBOT,
		OBSTACLES,
		TERRAIN,
		SIMULATION,
		STRING
};

class ClientGUI : public QMainWindow, private Ui::ClientGUI
{
	Q_OBJECT
	
private:
	QString			ipAddress;
	QTcpSocket		*m_tcpSocket;
	QDataStream		inStream;
	
private slots:
	void hostAction();
	void hostDisconnect();
	void sendToHost();
	void readFromHost();
	void hostError(QAbstractSocket::SocketError socketError);
	
public:
	ClientGUI(QWidget *parent = 0);
	~ClientGUI();
	
};

#endif // ClientGUI_H
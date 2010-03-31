#include <QtGui/QApplication>
#include "src/clientGUI.h"

int main(int argc, char *argv[])
{
    //Q_INIT_RESOURCE(images);

    QApplication a(argc, argv);
	
    ClientGUI client;
    client.show();
	
    int ret = a.exec();

    return ret;
}

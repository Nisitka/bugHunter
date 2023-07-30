#include "core.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Core coreApp;
    coreApp.start();

    return a.exec();
}

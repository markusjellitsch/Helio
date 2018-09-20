#include <QCoreApplication>
#include <helio.h>
#include <Heti/heti.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    HETI::GetInstance()->open();
    HETI::GetInstance()->dumpRegisters(true);


    return a.exec();
}

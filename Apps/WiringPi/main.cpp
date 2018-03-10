#include <QCoreApplication>
#include <linux/i2c-dev.h>
#include <linux/ioctl.h>

#include <bcm2835.h>



int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    i2c_smbus_read_block_data(0,0,0);





    return a.exec();
}

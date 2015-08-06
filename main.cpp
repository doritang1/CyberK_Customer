#include "ck_customer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CK_Customer w;
    w.show();

    return a.exec();
}

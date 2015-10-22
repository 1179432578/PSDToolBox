#include "dialog.h"
#include <QApplication>
#include <iostream>
#include "psdread.h"
#include "SFPSDWriter.h"
#include <string>
#include "SimplePSD.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Dialog w;
    w.setFixedSize(w.width(), w.height());
    w.show();

    printf("psdlayer size:%d", sizeof(PSDLayer));

    return a.exec();
}

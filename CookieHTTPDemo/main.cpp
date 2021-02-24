#include "widget.h"
#include <QApplication>
#include <QTextCodec>
#include <QDebug>

int main(int argc, char *argv[])
{
    QTextCodec *utf8TexCodec = QTextCodec::codecForName("UTF-8");
        qDebug() << "lookKai change textCodec to " << utf8TexCodec->name() << utf8TexCodec->mibEnum();
        QTextCodec::setCodecForLocale(utf8TexCodec);

    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}

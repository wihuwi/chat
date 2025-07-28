#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QFile>
#include <QDir>
#include <QSettings>

/*********************************************************
*
*  @file     main.cpp
*
*  @brief    XXXX FUNCTION
*
*  @author   hubin
*
*  @data     %{CurrentData:yy/mm/dd}
*
*  @history
*
*********************************************************/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile qss(":/style/stylesheet.qss");
    if(qss.open(QFile::ReadOnly)){
        qDebug()<<"open success";
        //QByteArray -> QString
        QString style = QLatin1String(qss.readAll());
        a.setStyleSheet(style);
        qss.close();
    }
    else{
        qDebug()<<"open failed";
    }

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "chat_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    QString path = "config.ini";
    QString app_path = QCoreApplication::applicationDirPath();
    QString ini_path = QDir::toNativeSeparators(app_path + QDir::separator() + path);
    qDebug()<<ini_path;
    QSettings setting(ini_path, QSettings::IniFormat);
    QString host = setting.value("GateServer/host").toString();
    QString port = setting.value("GateServer/port").toString();
    gate_url_prefix = "http://" + host + ":" + port;
    qDebug()<<setting.allKeys();
    qDebug()<<gate_url_prefix;
    MainWindow w;
    w.show();
    return a.exec();
}

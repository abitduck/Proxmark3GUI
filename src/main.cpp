﻿#include "ui/mainwindow.h"

#include <QApplication>
#include <QSettings>
#include <QTranslator>
#include <QMessageBox>
#include <QTextCodec>
#include <QDir>

int main(int argc, char *argv[])
{
    // A trick to handle non-ascii path
    // The application cannot find the plugins when the path contains non ascii characters.
    // However, the plugins will be load after creating MainWindow(or QApplication?).
    // QDir will handle the path correctly.
    QDir* pluginDir = new QDir;
    if(pluginDir->cd("plugins")) // has plugins folder
    {
        qputenv("QT_PLUGIN_PATH", pluginDir->absolutePath().toLocal8Bit());
    }
    delete pluginDir;

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QApplication a(argc, argv);
    MainWindow w;

    QSettings* settings = new QSettings("GUIsettings.ini", QSettings::IniFormat);
    settings->setIniCodec("UTF-8");
    settings->beginGroup("language");
    QString languageFile = settings->value("extPath").toString();
    QString languageName = settings->value("name").toString();
    settings->endGroup();
    if(languageName == "")
    {
        if(Util::chooseLanguage(settings, &w))
        {
            settings->beginGroup("language");
            languageName = settings->value("name").toString();
            settings->endGroup();
        }
        else
            languageName = "en_US";
    }
    if(languageName == "(ext)")
    {
        settings->beginGroup("language");
        languageFile = settings->value("extPath").toString();
        settings->endGroup();
    }
    else
        languageFile = ":/i18n/" + languageName + ".qm";
    QTranslator* translator = new QTranslator(&w);
    if(translator->load(languageFile))
        a.installTranslator(translator);
    else
        QMessageBox::information(&w, "Error", "Can't load " + languageFile + " as translation file.");
    delete settings;
    w.initUI();
    w.show();
    return a.exec();
}



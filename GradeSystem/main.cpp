#include <QtWidgets>
#include "qtsingleapplication.h"
#include "UI/MainForm.h"
#include <QTextCodec>
#include <QObject>

void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    mutex.lock();

    QString text;
    switch(type)
    {
    case QtDebugMsg:
        text = QString("Debug:");
        break;

    case QtWarningMsg:
        text = QString("Warning:");
        break;

    case QtCriticalMsg:
        text = QString("Critical:");
        break;

    case QtFatalMsg:
        text = QString("Fatal:");
    }

    QString context_info = QString("File:(%1) Line:(%2) Fun:(%3)").arg(QString(context.file)).arg(context.line).arg(context.function);
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    QString current_date = QString("(%1)").arg(current_date_time);
    QString message = QString("%1 %2 %3 %4").arg(text).arg(context_info).arg(current_date).arg(msg);


    QString str =USERCATALOGDIR;
    str.append("/userLog.txt");
    str.replace("/","\\");
    QFile file(str);
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << message << "\r\n";
    file.flush();
    file.close();

    mutex.unlock();
}
void initCode()
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);

}
int main(int argc, char *argv[])
{
    initCode() ;
    qInstallMessageHandler(outputMessage);

    QtSingleApplication app(argc, argv);
    QTranslator qtTranslator;
    qDebug()<<app.applicationDirPath();
    qtTranslator.load(app.applicationDirPath()+"/translation_zh.qm");
    app.installTranslator(&qtTranslator);
    if(app.isRunning())
    {

        QMessageBox::warning(nullptr,
                             QObject::tr("error"),
                             QObject::tr("a program running!"));

        return 0;
    }


    MainForm w;
    w.show();

    return app.exec();
}

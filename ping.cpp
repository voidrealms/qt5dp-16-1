#include "ping.h"

Ping::Ping(QObject *parent) : QObject(parent)
{
    connect(&m_process,&QProcess::errorOccurred,this,&Ping::errorOccurred);
    connect(&m_process,&QProcess::readyReadStandardError,this,&Ping::readyReadStandardError);
    connect(&m_process,&QProcess::readyReadStandardOutput,this,&Ping::readyReadStandardOutput);
    connect(&m_process,&QProcess::started,this,&Ping::started);
    connect(&m_process,&QProcess::stateChanged,this,&Ping::stateChanged);
    connect(&m_process,&QProcess::readyRead,this,&Ping::readyRead);

    //finished is overloaded...
    connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),this,&Ping::finished);

    m_listening = false;
    m_address = "";
}

QString Ping::operatingSystem()
{
    return QSysInfo::prettyProductName();
}

QString Ping::getAddress() const
{
    return  m_address;
}

void Ping::setAddress(const QString &address)
{
    m_address = address;
}

void Ping::start()
{
    qInfo() << Q_FUNC_INFO;
    m_listening = true;
    m_process.start(getProcess());
}

void Ping::stop()
{
    qInfo() << Q_FUNC_INFO;
    m_listening = false;
    m_process.close();
}

void Ping::errorOccurred(QProcess::ProcessError error)
{
    if(!m_listening) return;
    qInfo() << Q_FUNC_INFO << error;
    emit output("Error");
}

void Ping::finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if(!m_listening) return;
    qInfo() << Q_FUNC_INFO;
    Q_UNUSED(exitCode);
    Q_UNUSED(exitStatus);
    emit output("Complete");
}

void Ping::readyReadStandardError()
{
   if(!m_listening) return;
   qInfo() << Q_FUNC_INFO;
   QByteArray data =  m_process.readAllStandardError();
   QString message = "Standard Error: ";
   message.append(m_process.readAllStandardError());
   emit output(message);
}

void Ping::readyReadStandardOutput()
{
    if(!m_listening) return;
    qInfo() << Q_FUNC_INFO;
    QByteArray data =  m_process.readAllStandardOutput();
    emit output(QString(data.trimmed()));
}

void Ping::started()
{
    //Do nothing
     qInfo() << Q_FUNC_INFO;
}

void Ping::stateChanged(QProcess::ProcessState newState)
{
    qInfo() << Q_FUNC_INFO;
    switch (newState) {
    case QProcess::NotRunning:
        emit output("Not running");
        break;
    case QProcess::Starting:
        emit output("Starting...");
        break;
    case QProcess::Running:
        emit output("Running");
        //Start the ping
        startPing();
        break;
    }
}

void Ping::readyRead()
{
    if(!m_listening) return;
    qInfo() << Q_FUNC_INFO;
    QByteArray data = m_process.readAll().trimmed();
    qInfo() << data;
    emit output(data);
}

QString Ping::getProcess()
{
    qInfo() << Q_FUNC_INFO;
    if(QSysInfo::productType() == "windows") return "cmd";
    if(QSysInfo::productType() == "osx") return "/bin/zsh";

    return "bash";
}

void Ping::startPing()
{
    QByteArray command;
    command.append("ping " + m_address);
    if(QSysInfo::productType() == "windows") command.append("\r");
    command.append("\n");
    m_process.write(command);
}

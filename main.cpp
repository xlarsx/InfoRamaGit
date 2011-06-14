#include <QtCore/QCoreApplication>
#include <QDebug>
#include <QDir>
#include <iostream>

#define DEBUG

QString obtenInfoArchivo(QString archivo);
QString obtenSHA1DeHEAD(QString texto, QDir * directorio);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
#ifdef DEBUG
    QDir directorio("D://LARS//Proyectos//iClimaLaboral2011//Desarrollo//iClimaLaboral//iClimaLaboral");
#else
    QDir directorio = QDir(QDir::currentPath());
#endif

    if(directorio.cd(".git"))
    {
        if(directorio.exists("HEAD"))
        {
            QString dirArchivoHEAD = directorio.absoluteFilePath("HEAD");

            qDebug() << obtenSHA1DeHEAD(obtenInfoArchivo(dirArchivoHEAD), &directorio);
        }
    }

    return 0;
}

QString obtenInfoArchivo(QString archivo)
{
    QString salida = "";
    QFile archivoHEAD(archivo);
    if(!archivoHEAD.open(QIODevice::ReadOnly | QIODevice::Text))
        return 0;

    QTextStream in(&archivoHEAD);
    while (!in.atEnd()) {
        salida = in.readLine();
        if(salida.length() > 0)
            break;
    }

    archivoHEAD.close();
    return salida;
}

QString obtenSHA1DeHEAD(QString texto, QDir * directorio)
{
    if(texto.contains("ref:"))
    {
        QStringList lista =  texto.split(" ");
        if(lista.size() > 1)
        {
            QString ref = lista[1];
            return obtenSHA1DeHEAD(obtenInfoArchivo(directorio->absoluteFilePath(ref)), directorio);
        }
    } else
        return texto;
}

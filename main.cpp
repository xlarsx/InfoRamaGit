#include <QtCore/QCoreApplication>
#include <QDir>
#include <QTextStream>
#include <iostream>

//#define DEBUG

QString obtenInfoArchivo(QString archivo);
QString obtenSHA1DeHEAD(QString texto, QDir * directorio, QString &branch);

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
            QString branch = "";
            QString SHA1 = obtenSHA1DeHEAD(obtenInfoArchivo(dirArchivoHEAD), &directorio, branch);

            std::cout << branch.toLatin1().data() << SHA1.toLatin1().data();
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

QString obtenSHA1DeHEAD(QString texto, QDir * directorio, QString &branch)
{
    if(texto.contains("ref:"))
    {
        QStringList lista =  texto.split(" ");
        if(lista.size() > 1)
        {
            QString ref = lista[1];
            QString archivoALeer = directorio->absoluteFilePath(ref);
            QDir dirInteres(archivoALeer);
            branch.append(dirInteres.dirName() + "\n");
            return obtenSHA1DeHEAD(obtenInfoArchivo(archivoALeer), directorio, branch);
        }
    }
    return texto;
}

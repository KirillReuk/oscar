#include "IOHelpers.hpp"

namespace ontolis {

namespace io {

namespace helpers {

QStringList getLinesFromFile(QString filename)
{
    QStringList result;
    QFile file;
    file.setFileName(filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    QString line = in.readLine();
    while (!line.isNull()) {
        if (!line.startsWith('#'))
            result << line;
        line = in.readLine();
    }
    file.close();
    return result;
}

QString escapeNonLatinInUnicode(QString text)
{
    //http://stackoverflow.com/a/3212012
    QString strNew, isAscii, tmp;
    foreach(QChar cr, text)
    {
        if (cr == QChar('/')) {
            strNew += "\\/";
        }
        else if(cr.toLatin1() != QChar(0)) {
            isAscii = static_cast<QString>(cr.toLatin1());
            strNew += cr;
        }
        else {
            tmp.setNum(cr.unicode(), 16);
            tmp.prepend("\\u0");
            strNew += tmp;
        }
    }
    return strNew;
}

QString escapeDots(QString text)
{
    return text.replace(".", "\\.");
}

void saveTextToFile(QString filename, QString text)
{
    QFile file(filename);
    if(file.open(QIODevice::WriteOnly))
    {
        QTextStream out(&file);
        out << text;
        file.close();
    }
}

void saveTextAfterEscapingToFile(QString filename, QString text)
{
    saveTextToFile(filename, escapeNonLatinInUnicode(text));
}

} // namespace helpers

} // namespace io

} // namespace ontolis


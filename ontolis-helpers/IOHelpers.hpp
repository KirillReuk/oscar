#ifndef ONTOLIS_IO_HELPERS_HPP
#define ONTOLIS_IO_HELPERS_HPP

#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QJsonArray>
#include <QJsonDocument>

namespace ontolis {

namespace io {

namespace helpers {

QStringList getLinesFromFile(QString filename);
QString escapeNonLatinInUnicode(QString text);
QString escapeDots(QString text);
void saveTextToFile(QString filename, QString text);
void saveTextAfterEscapingToFile(QString filename, QString text);

} // namespace helpers

} // namespace io

} // namespace ontolis

#endif // ONTOLIS_IO_HELPERS_HPP

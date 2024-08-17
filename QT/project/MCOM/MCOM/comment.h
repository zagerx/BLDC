#ifndef COMMENT_H
#define COMMENT_H


#include <QString>
#include <vector>
#include <QString>
#include <QStringList.h>
#include <QDebug>
std::vector<unsigned char> stringToUCharVectorOptimized(const QString &str);
void PrintBytes(const std::vector<unsigned char>& bytes);
void printFloatsFromBytes(const std::vector<unsigned char>& data);
#endif

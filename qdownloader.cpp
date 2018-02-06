
#include "qdownloader.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QUrl>
#include <QDebug>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include <QRegularExpression>

#include <map>

namespace
{
  // Lookup table of basic noun markers
  std::map<std::string, std::string> nounMarkerLUT =
  {
    std::make_pair("m", "der"),
    std::make_pair("f", "die"),
    std::make_pair("n", "das")
  };

  auto extractValueFromJson = [](QString const & rawData)
  {
    // TODO: error checks

    //  qDebug(rawData.toStdString().c_str());
    QJsonDocument jsonDocu = QJsonDocument::fromJson(rawData.toUtf8());
    QJsonObject jsonObject = jsonDocu.object()["parse"].toObject();
    QJsonArray sectionsArray = jsonObject["sections"].toArray();
    //  qDebug() << sectionsArray;
    QJsonValue value = sectionsArray.at(1).toObject().value("line");
    //  qDebug() << value.toString();

    return value;
  };

  auto extractNounMarkerFromValue = [](QString jsonValue)
  {
    QRegularExpression basicRegex(R"(.*Substantiv, (?:Eigenname, )?([mfn]).*)");

    QRegularExpressionMatch match = basicRegex.match(jsonValue);
    if (match.hasMatch())
      return match.captured(1);

    return QString();
  };
}



QDownloader::QDownloader(QObject * parent) :
  QObject(parent),
  networkAccessManager(new QNetworkAccessManager),
  reply(nullptr)
{
  connect(networkAccessManager.get(), SIGNAL(finished(QNetworkReply *)), this, SLOT(onFinished(QNetworkReply *)));
}


void QDownloader::download(QString url)
{
  qDebug("Downloading");

  QNetworkRequest request;
  request.setUrl(QUrl(url));
  reply.reset(networkAccessManager->get(request));

  if (!reply)
    return;

  connect(reply.get(), SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(onDownloadProgress(qint64, qint64)));
  connect(reply.get(), SIGNAL(readyRead()), this, SLOT(onReadyRead()));
  connect(reply.get(), SIGNAL(finished()), this, SLOT(onReplyFinished()));
}


void QDownloader::onDownloadProgress(qint64 /*bytesRead*/, qint64 /*bytesTotal*/)
{
//  qDebug(QString::number(bytesRead).toLatin1() + " - " + QString::number(bytesTotal).toLatin1());
}


void QDownloader::onFinished(QNetworkReply * reply)
{
  if (!reply)
    return;

  notifyFinishedDownload(reply->errorString());

  switch (reply->error())
  {
    case QNetworkReply::NoError:
    {
      qDebug("The information has been downloaded successfully.");
      notifyFinishedDownload("The information has been downloaded successfully.");
      break;
    }
    default:
      qDebug(reply->errorString().toLatin1());
      break;
  }
}


void QDownloader::onReadyRead()
{
  auto isEmptyString = [this](QString const & text)
  {
    if (text.isEmpty())
    {
      emit notifyNounMarker("Could not find noun marker \nfor");
      return true;
    }

    return false;
  };

  QString jsonValueAsString = extractValueFromJson(reply->readAll()).toString();

  if (isEmptyString(jsonValueAsString))
    return;

  QString nounMarker = extractNounMarkerFromValue(jsonValueAsString);

  if (isEmptyString(nounMarker))
    return;

  auto const fullNounMarkerIterator = nounMarkerLUT.lower_bound(nounMarker.toStdString());

  if (fullNounMarkerIterator == nounMarkerLUT.end()) // should not happen, otherwise it is an implementation error
    return;

  emit notifyNounMarker(QString::fromStdString(fullNounMarkerIterator->second));
}


void QDownloader::onReplyFinished()
{ /* empty */ }

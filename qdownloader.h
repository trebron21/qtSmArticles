#pragma once

#include <QObject>

#include <memory>

class QNetworkAccessManager;
class QNetworkReply;

class QDownloader : public QObject
{
  Q_OBJECT

  signals:
    void notifyFinishedDownload(QString);
    void notifyNounMarker(QString);

  public:
    explicit QDownloader(QObject * parent = 0);
    virtual ~QDownloader() = default;
    void download(QString fileURL);

  private:
    std::unique_ptr<QNetworkAccessManager> networkAccessManager;
    std::unique_ptr<QNetworkReply> reply;

  private slots:
    void onDownloadProgress(qint64, qint64);
    void onFinished(QNetworkReply *);
    void onReadyRead();
    void onReplyFinished();
};

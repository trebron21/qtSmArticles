#pragma once

#include "keypresseater.h"

#include <QMainWindow>

#include <memory>

namespace Ui {
  class qtSmArticles;
}

class QDownloader;


class qtSmArticles : public QMainWindow
{
  Q_OBJECT

  public:
    explicit qtSmArticles(QWidget * parent = 0);
    ~qtSmArticles();

  public slots:
    void printResults(QString message);
    void printNounMarker(QString nounMarker);

  private slots:
    void on_goOnButton_clicked();

  private:
    Ui::qtSmArticles * ui;

    std::unique_ptr<QDownloader> downloader;

    QString keyword;

    KeyPressEater keypressEater;
};

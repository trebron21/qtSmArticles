#include "qtsmarticles.h"
#include "ui_qtsmarticles.h"
#include "qdownloader.h"

#include "keypresseater.h"

#include <QNetworkProxyFactory>


qtSmArticles::qtSmArticles(QWidget * parent) :
  QMainWindow(parent),
  ui(new Ui::qtSmArticles),
  downloader(new QDownloader()),
  keyword("")
{
  ui->setupUi(this);
  ui->wordTextEdit->installEventFilter(&keypressEater);
  ui->wordTextEdit->setFocus();

//  QNetworkProxyFactory::setUseSystemConfiguration(true);

  connect(&keypressEater, SIGNAL(enterPressed()), this, SLOT(on_goOnButton_clicked()));
  connect(downloader.get(), SIGNAL(notifyFinishedDownload(QString)), this, SLOT(printResults(QString)));
  connect(downloader.get(), SIGNAL(notifyNounMarker(QString)), this, SLOT(printNounMarker(QString)));
}


qtSmArticles::~qtSmArticles()
{
  delete ui;
}


void qtSmArticles::printResults(QString message)
{
  ui->textEdit->setText(message);
}


void qtSmArticles::printNounMarker(QString nounMarker)
{
  if (keyword.length() == 0)
  {
    ui->resultLabel->setText(nounMarker + " \"\"");
    return;
  }

  ui->resultLabel->setText(nounMarker + " " + keyword);
}


void qtSmArticles::on_goOnButton_clicked()
{
  keyword = ui->wordTextEdit->toPlainText();
  ui->wordTextEdit->clear();

  if (keyword.length() == 0)
  {
    ui->resultLabel->setText("Type a word!");
    return;
  }

  keyword[0] = keyword.at(0).toUpper();

  downloader->download("https://de.wiktionary.org/w/api.php?action=parse&page=" + keyword + "&prop=sections&format=json");
}

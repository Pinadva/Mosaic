#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //    ui->testBtn->setVisible(false);
    ui->testBtn->setVisible(true);
    this->setAcceptDrops(true);
    this->model        = new PhotoModel();
    this->presenter    = new PhotoPresenter(model);
    this->view         = new PhotoView(presenter);
    this->add_tag_form = new AddTagForm();

    connect(model, &PhotoModel::statusChanged, this, &MainWindow::updateStatusBar);
    connect(presenter, &PhotoPresenter::statusChanged, this, &MainWindow::updateStatusBar);
    connect(view, &PhotoView::statusChanged, this, &MainWindow::updateStatusBar);
    connect(view, &PhotoView::readyView, this, &MainWindow::viewResult);
    movie = new QMovie("://loader.gif");

    add_tag_form->show();
}

MainWindow::~MainWindow()
{
    delete this->movie;
    delete this->model;
    delete this->presenter;
    delete this->view;
    delete ui;
}

void MainWindow::updateStatusBar(QString status_message = "", int error_code = 0)
{
    qDebug() << status_message;
    switch (error_code)
    {
        case -1:
            ui->statusbar->setStyleSheet("QStatusBar{color:red;}"); //:rgba(255,0,0,255)
            this->movie->stop();
            this->ui->label->clear();
            break;
        case 0:
            ui->statusbar->setStyleSheet("QStatusBar{color:black;}");
            break;
        case 1:
            ui->statusbar->setStyleSheet("QStatusBar{color:green;}");
            break;
    }
    ui->statusbar->showMessage(status_message);
}

void MainWindow::viewResult(QPixmap &result)
{
    movie->stop();
    result_pixmap = result;
    ui->label->setPixmap(result.scaled(ui->label->width(), ui->label->height()));
}

void MainWindow::chooseKeyAction(QString key)
{
    if (key == "\u000F" or key == "o") // ctrl + o
        on_actionSelect_photos_triggered();
    else if (key == "\u000E") // ctrl + n
        on_actionAdd_tags_triggered();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    if (!result_pixmap.isNull())
        ui->label->setPixmap(result_pixmap.scaled(ui->label->width(), ui->label->height()));
}

void MainWindow::on_actionSelect_photos_triggered()
{
    QStringList filenames =
    QFileDialog::getOpenFileNames(this, "Выберите фотографии", SettingsSingleton::getInstance().getPath(), "*.jpg; *.jpeg; *jpe");
    qDebug() << "action";
    qDebug() << filenames;
    if (filenames.count())
    {
        ui->label->setMovie(movie);
        movie->start();
    }
    this->presenter->process(filenames);
}

void MainWindow::on_actionAdd_tags_triggered()
{
    add_tag_form->show();
}

void MainWindow::on_pushButton_clicked()
{
    QHash<QString, QString> h1;
    h1.insert("1", "first");
    QHash<QString, QString> h2;
    h2.insert("2", "second");

    QList<QHash<QString, QString>> data;
    data.append(h1);
    data.append(h2);

    QByteArray b;
    QDataStream d(&b, QIODevice::WriteOnly);
    d << data;

    QList<QHash<QString, QString>> data_res;
    QDataStream d_res(&b, QIODevice::ReadOnly);
    d_res >> data_res;
    qDebug() << data_res;
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    event->accept();
    qDebug() << event->text();
    chooseKeyAction(event->text());
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QStringList filenames = QUrl::toStringList(event->mimeData()->urls());
    for (QString &item : filenames)
    {
        item.replace("file:///", "");
    }
    if (filenames.count())
    {
        ui->label->setMovie(movie);
        movie->start();
    }

    this->presenter->process(filenames);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void MainWindow::on_testBtn_clicked()
{
    qDebug() << "testBtn clicked";
    QHash<int, PhotoSegment> photos = presenter->getPhotos();
    qDebug() << "photos";
    for (auto i : photos)
    {
        for (auto j : i.common_empty)
            qDebug() << j->first << j->second;
    }
    QPair<QString, QString> *pair = photos.begin().value().common_empty.first();
    qDebug() << "pair before = " << pair->first << pair->second;
    pair->second                  = "132132123132";
    QPair<QString, QString> pair1 = photos.begin().value().common_extra.first();
    qDebug() << "pair after = " << pair1.first << pair1.second;
    ui->label->clear();
    this->view->paint();
    this->repaint();
}

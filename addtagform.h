#ifndef ADDTAGFORM_H
#define ADDTAGFORM_H

#include "settingssingleton.h"
#include "tagwidget.h"

#include <QDialog>
#include <QListWidget>

namespace Ui
{
class AddTagForm;
}

class AddTagForm : public QDialog
{
    Q_OBJECT

public:
    explicit AddTagForm(QWidget *parent = nullptr);
    ~AddTagForm();

    void addTag();
    void addTag(TagWidget *);
    void saveTags();
    void removeTag();
    void loadTags();
    void chooseKeyAction(QString);

public slots:

    void removeTag(QPoint);
    bool isValid();

private slots:

    void on_buttonBox_accepted();
    void on_addBtn_clicked();
    void on_testBtn_clicked();

private:
    Ui::AddTagForm *ui;

    // QDialog interface

    // QWidget interface
protected:
    void keyReleaseEvent(QKeyEvent *event) override;

    // QDialog interface
public slots:
    void accept() override;
};

#endif // ADDTAGFORM_H

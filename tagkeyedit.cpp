#include "tagkeyeditlist.h"

TagKeyEdit::TagKeyEdit(QWidget *parent) : TagBaseWidget(parent)
{
    this->exif_name = new QComboBox();
    setExifName();
    connect(exif_name, &QComboBox::currentTextChanged, this, &TagKeyEdit::checkValid);
    layout->addWidget(short_name_lineEdit);
    layout->addWidget(exif_name);
    layout->addWidget(remove_btn);
    setLayout(layout);
    setData();
}

void TagKeyEdit::setExifName()
{
    exif_name->addItems(extra.getExtra());
    exif_name->setEditable(true);
    QCompleter *c = new QCompleter(extra.getExtra(), exif_name);
    c->setCaseSensitivity(Qt::CaseInsensitive);

    exif_name->setCompleter(c);
}

void TagKeyEdit::setExifName(const QString &cur_text)
{
    setExifName();
    exif_name->setCurrentIndex(exif_name->findText(cur_text));
}

void TagKeyEdit::setData()
{
    setShortName();
    setExifName();
}

void TagKeyEdit::setData(QPair<QString, QString> data)
{
    setShortName(data.first);
    setExifName(data.second);
}

QPair<QString, QString> TagKeyEdit::getData()
{
    QPair<QString, QString> pair(short_name_lineEdit->text(), exif_name->currentText());
    return pair;
}

bool TagKeyEdit::isValid()
{
    return !(short_name_lineEdit->text().isEmpty() or exif_name->currentText().isEmpty() or
             exif_name->findText(exif_name->currentText()) == -1);
}

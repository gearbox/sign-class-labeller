#include "imglist.h"

using std::tuple;
using std::get;

QListWidgetItem * make_item(const QString &name, const QIcon &icon,
                              const QSize &item_size) {
  QListWidgetItem *item = new QListWidgetItem(icon, name);
  item->setSizeHint(item_size);
  return item;
}

ImgList::ImgList(const QSize &item_size, bool multiselect)
    : item_size_(item_size) {
  if (multiselect)
      setSelectionMode(QAbstractItemView::MultiSelection);

  setViewMode(QListWidget::IconMode);
  setDragEnabled(false);
  setIconSize(item_size);
}

void ImgList::set_imgs(const QVector<tuple<QString, QIcon>> &icons) {
  clear();
  for (const auto &icon_pair : icons)
    addItem(make_item(get<0>(icon_pair), get<1>(icon_pair), item_size_));
}

#include <QDebug>

void ImgList::set_imgs(const QVector<tuple<QString, QIcon>> &icons,
                       const QVector<bool> &marks) {
  clear();
  for (int i = 0; i < icons.size(); ++i) {
    auto item = make_item(get<0>(icons[i]), get<1>(icons[i]), item_size_);
    addItem(item);
    item->setSelected(marks.at(i));
  }
}

void ImgList::select_icon(const QString &label)
{
  for (int i = 0; i < count(); ++i) {
    QListWidgetItem *item = this->item(i);
    if (item->text() == label) {
      item->setSelected(true);
    }
  }
}

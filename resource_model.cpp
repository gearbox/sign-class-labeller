#include <QDir>
#include <QPixmap>
#include <QImage>

#include <QDebug>

#include "resource_model.h"

using std::tuple;
using std::make_tuple;
using std::get;

ResourceModel::ResourceModel() {
}

QVector<tuple<QString, QIcon>> ResourceModel::load_imgs(const QString &path,
    const QVector<QString> &filenames) {
  QVector<tuple<QString, QIcon>> imgs;
  for (const auto &filename : filenames) {
    QImage img(path + "/" + filename);
    img = img.scaled(img.width() * 2, img.height() * 2, Qt::IgnoreAspectRatio,
      Qt::SmoothTransformation);
    imgs.push_back(make_tuple(filename,
      QIcon(QPixmap::fromImage(img))));
  }
  return imgs;
}

void ResourceModel::load_sign_icons() {
  // load superclass icons
  {
    QString dirname("sign-icons/superclass/");
    QDir dir(dirname);
    for (const auto &name : dir.entryList({"*.gif"})) {
      QString filepath = dirname + name;
      superclass_icons_.push_back(make_tuple(name.left(name.size() - 4),
                                             QIcon(filepath)));
    }
  }

  // load class icons
  {
    for (const auto &superclass_pair : superclass_icons_) {
      QString superclass_name = get<0>(superclass_pair);
      QString dirname = "sign-icons/" + superclass_name + "/";
      QDir dir(dirname);
      QVector<tuple<QString, QIcon>> icons;
      for (const auto &name : dir.entryList({"*.gif"})) {
        QString filepath = dirname + name;
        icons.push_back(make_tuple(name.left(name.size() - 4),
                                   QIcon(filepath)));
      }
      class_icons_.push_back(icons);
    }
  }
}

QVector<tuple<QString, QIcon>> ResourceModel::get_superclass_icons() const {
  return superclass_icons_;
}

QVector<tuple<QString, QIcon>> ResourceModel::get_class_icons(
    const QString &superclass_name) const {
  int i = 0;
  for (; i < superclass_icons_.size(); ++i)
    if (get<0>(superclass_icons_[i]) == superclass_name)
      break;

  if (i == superclass_icons_.size())
    throw "Superclass name not found";

  return class_icons_[i];
}

QString ResourceModel::get_superclass_by_classname(const QString &classname)
  const {
  for (int i = 0; i < class_icons_.size(); ++i) {
    for (const auto &class_pair : class_icons_[i]) {
      if (get<0>(class_pair) == classname)
        return get<0>(superclass_icons_[i]);
    }
  }
  return "";
}

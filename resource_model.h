#ifndef RESOURCE_MODEL_H
#define RESOURCE_MODEL_H

#include <tuple>

#include <QVector>
#include <QString>
#include <QIcon>

class ResourceModel {
 public:
  ResourceModel();

  QVector<std::tuple<QString, QIcon>> load_imgs(const QString &path,
                                    const QVector<QString> &filenames);

  void load_sign_icons();

  QVector<std::tuple<QString, QIcon>> get_superclass_icons() const;
  QVector<std::tuple<QString, QIcon>> get_class_icons(
      const QString &superclass_name) const;

 private:
  QVector<std::tuple<QString, QIcon>> superclass_icons_;
  QVector<QVector<std::tuple<QString, QIcon>>> class_icons_;
};

#endif // RESOURCE_MODEL_H

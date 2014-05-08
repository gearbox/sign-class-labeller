#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <tuple>

#include <QObject>
#include <QString>

#include "view.h"
#include "resource_model.h"
#include "labelling_model.h"

class Controller : public QObject {
  Q_OBJECT

 public:
  Controller(View *view, bool block_opt);

 private slots:
  void open_labelling();
  void show_superclass_icons();
  void next_img();
  void prev_img();
  void icon_click(const QString &name);

 private:
  View *view_;
  ResourceModel *res_model_;
  LabellingModel *lab_model_;
  bool block_opt_;

  void change_index(int index, bool save = true);
  void set_class(const QString &name);
  void show_class_icons(const QString &superclass_name);
  void update_navigation();
};

#endif  // CONTROLLER_H_

#ifndef VIEW_H
#define VIEW_H

#include <tuple>

#include <QWidget>
#include <QVector>
#include <QIcon>
#include <QString>
#include <QPushButton>
#include <QLabel>
#include <QShortcut>
#include <QListWidgetItem>

#include "imglist.h"

class View : public QWidget {
 Q_OBJECT

 public:
  View();

  QString show_dir_dialog(const QString &label);
  void show_error(const QString &title, const QString &message);
  void show_sign_icons(const QVector<std::tuple<QString, QIcon>> &icons);
  void show_sign_imgs(const QVector<std::tuple<QString, QIcon>> &imgs,
                      const QVector<bool> &marks);

  void enable_navigation(bool value);
  void set_prev_enabled(bool value);
  void set_next_enabled(bool value);
  void set_back_enabled(bool value);
  QVector<std::tuple<QString, bool>> get_marks();

  void set_class_label(const QString &label);
  void set_count_label(int cur_val, int max_val);
  int input_number();
  double input_real_number();

  void select_icon(const QString &label);

 signals:
  void open_labelling();
  void next_img();
  void prev_img();
  void next_unknown();
  void show_superclass_icons();
  void icon_click(const QString &name);

 private slots:
  void icon_clicked(QListWidgetItem *item);

 private:
  QPushButton *open_button_, *back_button_, *prev_button_, *next_button_;
  QPushButton *unknown_button_;
  QShortcut *prev_shortcut_, *next_shortcut_;
  QLabel *class_label_, *count_label_;
  ImgList *class_icons_, *sign_imgs_;

  void set_layout();
  void set_shortcuts();
  void bind_signals();
};

#endif // VIEW_H

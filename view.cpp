#include <boost/variant/variant.hpp>
#include <boost/variant/get.hpp>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QKeySequence>
#include <QInputDialog>

#include "view.h"

using std::tuple;
using std::make_tuple;

QPushButton * make_button(QPushButton **button, const QString &name) {
  *button = new QPushButton(name);
  return *button;
}

QLabel * make_label(QLabel **label, const QString &name, int width=-1,
                    Qt::Alignment align = Qt::AlignLeft | Qt::AlignVCenter) {
  *label = new QLabel(name);

  if (width != -1)
    (*label)->setFixedWidth(width);

  (*label)->setAlignment(align);

  return *label;
}

QSpacerItem * make_spacer(int width) {
  return new QSpacerItem(width, 1);
}

View::View() : open_button_(), back_button_(), prev_button_(), next_button_(),
  prev_shortcut_(), next_shortcut_(),
  class_label_(), count_label_(), class_icons_(), sign_imgs_() {
    showMaximized();
    set_layout();
    set_shortcuts();
    bind_signals();
    enable_navigation(false);
}

void View::set_shortcuts() {
#define BIND(shortcut_member, seq_string, slot_fun) shortcut_member = \
  new QShortcut(QKeySequence(seq_string), this); \
  connect(shortcut_member, SIGNAL(activated()), this, SIGNAL(slot_fun()));

  BIND(prev_shortcut_, "Left", prev_img);
  BIND(next_shortcut_, "Right", next_img);

#undef BIND
}

void View::set_layout() {
  QVBoxLayout *mainLayout = new QVBoxLayout;

  mainLayout->setSpacing(0);
  mainLayout->setMargin(0);
  mainLayout->setContentsMargins(0, 0, 0, 0);

  QHBoxLayout *buttonLayout = new QHBoxLayout;
  buttonLayout->setAlignment(Qt::AlignLeft);

  QVector<boost::variant<QWidget *, QSpacerItem *>> objects = {
    make_button(&open_button_, "Open directory"),
    make_spacer(20),
    make_button(&back_button_, "Back to superclass"),
    make_spacer(50),
    make_label(&class_label_, "Sign class: ", 150),
    make_spacer(20),
    make_button(&prev_button_, "Prev image"),
    make_label(&count_label_, "", 100, Qt::AlignCenter),
    make_button(&next_button_, "Next image")
  };

  for (auto object : objects) {
    switch (object.which()) {
    case 0:
      buttonLayout->addWidget(boost::get<QWidget *>(object));
      break;
    case 1:
      buttonLayout->addSpacerItem(boost::get<QSpacerItem *>(object));
      break;
    }
  }

  mainLayout->addLayout(buttonLayout);

  class_icons_ = new ImgList(QSize(80, 80));
  mainLayout->addWidget(class_icons_);

  sign_imgs_ = new ImgList(QSize(128, 128), true);
  mainLayout->addWidget(sign_imgs_);

  setLayout(mainLayout);
}

void View::bind_signals() {
  connect(open_button_, SIGNAL(clicked()), this, SIGNAL(open_labelling()));
  connect(next_button_, SIGNAL(clicked()), this, SIGNAL(next_img()));
  connect(prev_button_, SIGNAL(clicked()), this, SIGNAL(prev_img()));
  connect(back_button_, SIGNAL(clicked()), this,
          SIGNAL(show_superclass_icons()));
  connect(class_icons_, SIGNAL(itemClicked(QListWidgetItem*)), this,
          SLOT(icon_clicked(QListWidgetItem*)));
}

QString View::show_dir_dialog(const QString &label) {
  return QFileDialog::getExistingDirectory(this, label, "",
                                           QFileDialog::ShowDirsOnly);
}

void View::show_error(const QString &title, const QString &message) {
  QMessageBox::warning(this, title, message);
}

void View::show_sign_icons(const QVector<tuple<QString, QIcon>> &icons) {
  class_icons_->set_imgs(icons);
}

void View::show_sign_imgs(const QVector<tuple<QString, QIcon>> &imgs,
                          const QVector<bool> &marks) {
  sign_imgs_->set_imgs(imgs, marks);
}

void View::enable_navigation(bool value) {
  set_prev_enabled(value);
  set_next_enabled(value);
}

QVector<tuple<QString, bool>> View::get_marks() {
  QVector<tuple<QString, bool>> marks;
  auto items = sign_imgs_->findItems(QString("*"),
                                     Qt::MatchWrap | Qt::MatchWildcard);
  for (auto item : items)
    marks.push_back(make_tuple(item->text(), item->isSelected()));

  return marks;
}

void View::set_prev_enabled(bool value) {
  prev_button_->setEnabled(value);
  prev_shortcut_->setEnabled(value);
}

void View::set_next_enabled(bool value) {
  next_button_->setEnabled(value);
  next_shortcut_->setEnabled(value);
}

void View::set_back_enabled(bool value) {
  back_button_->setEnabled(value);
}

void View::icon_clicked(QListWidgetItem *item) {
  emit icon_click(item->text());
}

void View::set_class_label(const QString &label) {
  class_label_->setText("Sign class: " + label);
}

void View::set_count_label(int cur_val, int max_val) {
  count_label_->setText(QString::number(cur_val) + "/"
                      + QString::number(max_val));
}

int View::input_number() {
  bool ok;
  int number = QInputDialog::getInt(this, "Input number",
    "Number on sign:", 1, 1, 1000, 1, &ok);
  return (ok ? number : -1);
}

double View::input_real_number() {
  bool ok;
  double number = QInputDialog::getDouble(this, "Input number",
    "Number on sign", 0, 0, 10, 1, &ok);
  return (ok ? number : -1);
}

void View::select_icon(const QString &label)
{
  class_icons_->select_icon(label);
}

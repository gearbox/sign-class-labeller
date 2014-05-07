#include <algorithm>
using std::min;
using std::max;

using std::tuple;

#include "./controller.h"

Controller::Controller(View *view, bool block_opt) : view_(view),
  res_model_(new ResourceModel), lab_model_(new LabellingModel),
  block_opt_(block_opt) {
  res_model_->load_sign_icons();
  show_superclass_icons();

  connect(view_, SIGNAL(open_labelling()), this, SLOT(open_labelling()));
  connect(view_, SIGNAL(show_superclass_icons()), this,
          SLOT(show_superclass_icons()));
  connect(view_, SIGNAL(next_img()), this, SLOT(next_img()));
  connect(view_, SIGNAL(prev_img()), this, SLOT(prev_img()));
  connect(view_, SIGNAL(icon_click(const QString &)), this,
          SLOT(icon_click(const QString &)));
}

void Controller::open_labelling() {
  QString dirname = view_->show_dir_dialog("Open directory");

  if (dirname == "")
    return;

  lab_model_->set_dirname(dirname);

  try {
    lab_model_->read_labelling();
  } catch (const char *err) {
    view_->show_error("Labelling reading error", err);
  }
  change_index(0, false);
}

void Controller::change_index(int index, bool save) {
  if (save) {
    lab_model_->set_marks(view_->get_marks());
    lab_model_->save_labelling();
  }

  index = max(0, min(index, lab_model_->get_labelling().size() - 1));

  lab_model_->set_sign_index(index);
  auto imgs = res_model_->load_imgs(lab_model_->get_dirname(),
      lab_model_->get_filenames());

  view_->show_sign_imgs(imgs, lab_model_->get_marks());
  view_->set_class_label(lab_model_->get_class());
  view_->set_count_label(index + 1, lab_model_->get_labelling().size());
  show_superclass_icons();
  update_navigation();
}

void Controller::update_navigation() {
  int index = lab_model_->get_sign_index();

  view_->set_prev_enabled(index != 0);
  view_->set_next_enabled(index != lab_model_->get_labelling().size() - 1);

  if (block_opt_)
    view_->set_next_enabled(index < lab_model_->get_unlabelled_ind());
}

void Controller::show_superclass_icons() {
  view_->show_sign_icons(res_model_->get_superclass_icons());
  view_->set_back_enabled(false);
}

void Controller::show_class_icons(const QString &superclass_name) {
  view_->show_sign_icons(res_model_->get_class_icons(superclass_name));
  view_->set_back_enabled(true);
}

void Controller::next_img() {
  change_index(lab_model_->get_sign_index() + 1);
}

void Controller::prev_img() {
  change_index(lab_model_->get_sign_index() - 1);
}

void Controller::icon_click(const QString &name) {
  if (name.length() == 1) {
    // superclass
    show_class_icons(name);
  } else {
    QString new_name = name;
    if (lab_model_->is_loaded()) {
      if (new_name.right(1) == "n" && new_name != "unknown") {
        int n = view_->input_number();
        if (n == -1)
          return;
        new_name += QString::number(n);
      } else if (new_name.right(1) == "r") {
        double r = view_->input_real_number();
        if (r == -1)
          return;
        new_name += QString::number(r);
      }
      set_class(new_name);
      view_->set_class_label(new_name);
    }
  }
}

void Controller::set_class(const QString &name) {
  lab_model_->set_class(name);
  update_navigation();
}
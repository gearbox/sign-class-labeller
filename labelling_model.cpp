#include <cassert>

#include <QFile>

#include "labelling_model.h"

using std::tuple;
using std::make_tuple;
using std::get;

LabellingModel::LabellingModel() : labelling_(), dirname_(), sign_index_(-1),
  loaded_(false) {
}

void LabellingModel::read_labelling(QTextStream &stream) {
  while (!stream.atEnd()) {
    QString id, filename;
    int count;
    stream >> id >> count;
    QVector<tuple<QString, bool>> sign_imgs;
    for (int i = 0; i < count; ++i) {
      stream >> filename;
      sign_imgs.push_back(make_tuple(filename, false));
    }
    labelling_.push_back(make_tuple(id, "NONE", sign_imgs));
  }
  if (!labelling_.empty()) {
    sign_index_ = 0;
    loaded_ = true;
  } else {
    sign_index_ = -1;
    loaded_ = false;
  }
}

void LabellingModel::read_out_labelling(QTextStream &stream) {
  while (!stream.atEnd()) {
    QString id, filename, class_name;
    int count, mark;
    stream >> id >> class_name >> count;
    QVector<tuple<QString, bool>> sign_imgs;
    for (int i = 0; i < count; ++i) {
      stream >> filename >> mark;
      sign_imgs.push_back(make_tuple(filename, mark));
    }
    labelling_.push_back(make_tuple(id, class_name, sign_imgs));
  }
  if (!labelling_.empty()) {
    sign_index_ = 0;
    loaded_ = true;
  } else {
    sign_index_ = -1;
    loaded_ = false;
  }
}

void LabellingModel::save_labelling(QTextStream &stream) {
  for (const auto &sign_tuple : labelling_) {
    const auto &sign_imgs = get<2>(sign_tuple);
    stream << get<0>(sign_tuple) << " " << get<1>(sign_tuple)
           << " " << sign_imgs.size();
    for (const auto &sign_img : sign_imgs)
      stream << " " << get<0>(sign_img) << " " << get<1>(sign_img);
    stream << endl;
  }
}

QVector<tuple<QString, QString, QVector<tuple<QString, bool>>>>
LabellingModel::get_labelling() const {
  return labelling_;
}

QVector<QString> LabellingModel::get_filenames() const {
  QVector<QString> filenames;
  for (const auto &sign_pairs : get<2>(labelling_.at(sign_index_)))
    filenames.push_back(get<0>(sign_pairs));
  return filenames;
}

QVector<bool> LabellingModel::get_marks() const {
  QVector<bool> marks;
  for (const auto &sign_pairs : get<2>(labelling_.at(sign_index_)))
    marks.push_back(get<1>(sign_pairs));
  return marks;
}

void LabellingModel::set_marks(const QVector<tuple<QString, bool>> &marks) {
  get<2>(labelling_[sign_index_]) = marks;
}

void LabellingModel::set_dirname(const QString &dirname) {
  dirname_ = dirname;
}

QString LabellingModel::get_dirname() const {
  return dirname_;
}

void LabellingModel::read_labelling() {
  assert(dirname_ != "");

  QFile out_labelling(dirname_ + "/out_labelling.txt");
  if (out_labelling.exists() &&
      out_labelling.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QTextStream stream(&out_labelling);
    read_out_labelling(stream);
    return;
  }

  QFile labelling(dirname_ + "/in_labelling.txt");
  if (!labelling.exists())
    throw "File in_labelling.txt not found";

  if (!labelling.open(QIODevice::ReadOnly | QIODevice::Text))
    throw "File in_labelling.txt can't be opened for reading";

  QTextStream stream(&labelling);
  read_labelling(stream);
}

void LabellingModel::save_labelling() {
  assert(dirname_ != "");

  QFile labelling(dirname_ + "/out_labelling.txt");
  if (!labelling.open(QIODevice::WriteOnly | QIODevice::Text))
    throw "File out_labelling.txt can't be opened for reading";

  QTextStream stream(&labelling);
  save_labelling(stream);
}

int LabellingModel::get_sign_index() const {
  return sign_index_;
}

void LabellingModel::set_sign_index(int index) {
  if (!(0 <= index && index < labelling_.size()))
    throw "Invalid index";
  sign_index_ = index;
}

void LabellingModel::set_class(const QString &name) {
  get<1>(labelling_[sign_index_]) = name;
}

bool LabellingModel::is_loaded() const {
  return loaded_;
}
QString LabellingModel::get_class() const {
  return get<1>(labelling_[sign_index_]);
}

int LabellingModel::get_unlabelled_ind() const {
  if (!loaded_)
    return -1;

  int i = 0;
  for (; i < labelling_.size(); ++i)
    if (get<1>(labelling_[i]) == "NONE")
      break;
  if (i != labelling_.size())
      return i;
  return -1;
}

int LabellingModel::get_unknown_ind() const {
  if (!loaded_)
      return -1;

  int i = sign_index_ + 1;
  for (; i < labelling_.size(); ++i)
    if (get<1>(labelling_[i]).endsWith("unknown"))
      break;
  if (i != labelling_.size())
      return i;

  i = 0;
  for (; i < sign_index_; ++i)
    if (get<1>(labelling_[i]).endsWith("unknown"))
      break;
  if (i != sign_index_)
      return i;

  return -1;
}

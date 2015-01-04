#ifndef LABELLING_MODEL_H_
#define LABELLING_MODEL_H_

#include <QVector>
#include <QString>
#include <QTextStream>
#include <QElapsedTimer>
#include <tuple>

class LabellingModel {
 public:
  LabellingModel();

  void read_labelling();
  void save_labelling();

  void read_labelling(QTextStream &stream);
  void read_out_labelling(QTextStream &stream);
  void save_labelling(QTextStream &stream);

  QVector<std::tuple<QString, QString, QVector<std::tuple<QString, bool>>>>
      get_labelling() const;

  QVector<QString> get_filenames() const;
  void set_marks(const QVector<std::tuple<QString, bool>> &marks);
  QVector<bool> get_marks() const;

  void set_dirname(const QString &dirname);
  QString get_dirname() const;

  int get_sign_index() const;
  void set_sign_index(int index);

  void set_class(const QString &name);
  QString get_class() const;
  bool is_loaded() const;

  int get_unlabelled_ind() const;
  int get_unknown_ind() const;

 private:
  QVector<std::tuple<QString, QString, QVector<std::tuple<QString, bool>>>>
      labelling_;
  QString dirname_;
  int sign_index_;
  bool loaded_;
  int seconds_elapsed_;
  QElapsedTimer timer_;
};

#endif  // LABELLING_MODEL_H_

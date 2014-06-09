#include <QApplication>

#include "controller.h"
#include "view.h"

class MyApplication : public QApplication {
 public:
  MyApplication(int &argc, char **argv): QApplication(argc, argv) {}

 private:
  bool notify(QObject *receiver, QEvent *event) {
    try {
      return QApplication::notify( receiver, event );
    } catch ( ... ) {
      return false;
    }
  }
};

int main(int argc, char *argv[]) {
  bool block_opt = !(argc == 2 && QString(argv[1]) == "--no-block-next");

  MyApplication app(argc, argv);

  View view;
  Controller controller(&view, block_opt);
  view.show();

  return app.exec();
}

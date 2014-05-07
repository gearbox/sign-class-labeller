#include <QApplication>

#include "controller.h"
#include "view.h"

int main(int argc, char *argv[])
{
  bool block_opt = !(argc == 2 && QString(argv[1]) == "--no-block-next");

  QApplication app(argc, argv);

  View view;
  Controller controller(&view, block_opt);
  view.show();

  return app.exec();
}

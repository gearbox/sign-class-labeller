QMAKE_CXX = g++
QMAKE_CXXFLAGS += -std=c++11

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sign-class-labeller
TEMPLATE = app


SOURCES += labelling_model.cpp \
           resource_model.cpp \
           view.cpp \
           controller.cpp \
           imglist.cpp \
           main.cpp

HEADERS += labelling_model.h \
           resource_model.h \
           view.h \
           controller.h \
           imglist.h

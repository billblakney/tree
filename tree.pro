######################################################################
# Automatically generated by qmake (2.01a) Fri Feb 13 13:59:57 2015
######################################################################

TEMPLATE = app
TARGET = tree
DEPENDPATH += .
INCLUDEPATH += /usr/include/QtXml
INCLUDEPATH += /opt/protobuf-2.4.1/include
INCLUDEPATH += /opt/boost/include
INCLUDEPATH += .
QT = core gui
greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
}

DEFINES += USE_LOG4CXX

#Bill- LIBS += -L/usr/lib \
LIBS += -L/opt/boost/lib -lboost_program_options \
        -L/usr/lib/x86_64-linux-gnu -lboost_regex \
        -llog4cxx \ 
        ../structor/structor.so \
#        ../structor/Field.o \
#        ../structor/main.o \
#        ../structor/StructorBuilder.o \
#        ../structor/Structure.o \

INCLUDEPATH += ../structor

# Input
HEADERS += \
           DataStructModel.hh \
           FieldItem.hh \
           LineConsumer.hh \
           Logger.hh \
           MainWindow.hh \
           SimpleLineConsumer.hh \
           StreamReader.hh \

SOURCES += \
           main.cc \
           DataStructModel.cc \
           FieldItem.cc \
           LineConsumer.cc \
           Logger.cc \
           MainWindow.cc \
           SimpleLineConsumer.cc \
           StreamReader.cc \

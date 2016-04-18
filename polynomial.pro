greaterThan(QT_MAJOR_VERSION, 4): QT += core widgets

FORMS += \
    mainwindow.ui

HEADERS += \
    divisors.h \
    divisorstable.h \
    factorsmap.h \
    factorstack.h \
    mainwindow.h \
    polynomial.h \
    ui_mainwindow.h \
    valuetable.h

SOURCES += \
    divisors.cpp \
    divisorstable.cpp \
    factorsmap.cpp \
    factorstack.cpp \
    main.cpp \
    mainwindow.cpp \
    polynomial.cpp \
    valuetable.cpp

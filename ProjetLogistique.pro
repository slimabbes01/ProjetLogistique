# Project Configuration
QT       += core gui widgets sql charts printsupport

CONFIG += c++17
CONFIG += lrelease embed_translations

# Disable deprecated APIs if needed
# DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

# Source files
SOURCES += \
    clientmanager.cpp \
    commandemanager.cpp \
    connection.cpp \
    main.cpp \
    mainwindow.cpp

# Header files
HEADERS += \
    clientcombobox.h \
    clientmanager.h \
    commandemanager.h \
    connection.h \
    mainwindow.h

# UI files
FORMS += \
    mainwindow.ui

# Translations
TRANSLATIONS += \
    ProjetLogistique_en_150.ts

# Deployment paths (default rules)
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

TEMPLATE = subdirs

SUBDIRS += \
    qjsonrpc \
    kontroller

kontroller.depends = qjsonrpc

DISTFILES += \
    kontroller/qml/sailfish/ServerSettingsPage.qml

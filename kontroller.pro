TEMPLATE = subdirs

SUBDIRS += \
    qjsonrpc \
    kontroller

kontroller.depends = qjsonrpc

OTHER_FILES += rpm/harbour-kontroller.yaml \
       rpm/harbour-kontroller.spec

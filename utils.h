#ifndef UTILS_H
#define UTILS_H

#include <functional>
#include <QQmlListProperty>
#include <QList>

template<typename T, typename Container>
std::function<int(QQmlListProperty<T>*)> QQmlListPropertyQListFilesPropCountHelper()
{
    return [](QQmlListProperty<T*>* list) {
        return static_cast<Container*>(list->data)->count();
    };
}

template<typename T, typename Container>
std::function<int(QQmlListProperty<T>*,int)> QQmlListPropertyQListFilesPropAtHelper()
{
    return [] (QQmlListProperty<T>* list, int index) {
        return static_cast<Container*>(list->data)->at(index);
    };
}

QUrl getImageUrl(QString kodiUrl);

#endif // UTILS_H


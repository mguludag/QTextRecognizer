#ifndef MYICON_HPP
#define MYICON_HPP

#include <QAction>
#include <QIcon>
#include <QMainWindow>
#include <QObject>

namespace Ui {
class MainWindow;
}

class MyIcon : public QObject
{
    Q_OBJECT
public:
    explicit MyIcon(QObject *parent = nullptr);
    static void setIcons(QList<QAction *> &actList, bool isDark);

signals:

public slots:

private:
};

#endif // MYICON_HPP

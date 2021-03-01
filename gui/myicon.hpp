#ifndef MYICON_HPP
#define MYICON_HPP

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
    static void setIcons(Ui::MainWindow *ui, bool isDark);

signals:

public slots:

private slots:

private:
    Ui::MainWindow *ui;
};

#endif // MYICON_HPP

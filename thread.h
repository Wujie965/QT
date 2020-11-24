#ifndef THREAD_H
#define THREAD_H

#include <QWidget>

class TestThread:  public QWidget{
    Q_OBJECT

public:
    TestThread(QWidget* parent = nullptr);
    ~TestThread();

private:
};

#endif // THREAD_H

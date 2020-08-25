#include "dialog.h"

/*
 What
 Using QProcess to call other apps

 Description
 let the OS and its applications work for you

 Why
 Don't reinvent the wheel

 Example
 Ping - hard, requires admin access, and usually a steep learning curve

 This will make people mad...everyone wants to do things as hard as possible
 But you would be suprised to know how common this really is!!!
 */

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.show();
    return a.exec();
}

#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <KF5/KScreen/kscreen/config.h>
#include <KF5/KScreen/kscreen/log.h>
#include <KF5/KScreen/kscreen/output.h>
#include <KF5/KScreen/kscreen/edid.h>
#include <KF5/KScreen/kscreen/configmonitor.h>
#include <KF5/KScreen/kscreen/getconfigoperation.h>
#include <KF5/KScreen/kscreen/setconfigoperation.h>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
protected Q_SLOTS:
    void slotRotationChanged(int index);
private:
    Ui::Widget *ui;
    void setConfig(const KScreen::ConfigPtr &config);
    void addOutput(const KScreen::OutputPtr &output);
    KScreen::ConfigPtr mConfig          = nullptr;
    KScreen::OutputPtr mOutput;
};
#endif // WIDGET_H

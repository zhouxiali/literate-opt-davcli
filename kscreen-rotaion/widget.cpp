#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->rotationBox->addItem("normal",KScreen::Output::None);
    ui->rotationBox->addItem("right",KScreen::Output::Right);
    ui->rotationBox->addItem("left",KScreen::Output::Left);
    ui->rotationBox->addItem("upside-down",KScreen::Output::Inverted);
    QObject::connect(new KScreen::GetConfigOperation(), &KScreen::GetConfigOperation::finished,
                     [&](KScreen::ConfigOperation *op) {
        this->setConfig(qobject_cast<KScreen::GetConfigOperation *>(op)->config());
    });

}

Widget::~Widget()
{
    delete ui;
}
void Widget::setConfig(const KScreen::ConfigPtr &config) {
    if (mConfig) {
        KScreen::ConfigMonitor::instance()->removeConfig(mConfig);
        for (const KScreen::OutputPtr &output : mConfig->outputs()) {
            output->disconnect(this);
        }
        mConfig->disconnect(this);
    }
    mConfig = config;
    KScreen::ConfigMonitor::instance()->addConfig(mConfig);
    for (const KScreen::OutputPtr &output : mConfig->outputs()) {
        addOutput(output);
    }
}

void Widget::addOutput(const KScreen::OutputPtr &output) {
    mOutput = output;
    connect(ui->rotationBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated),
            this, &Widget::slotRotationChanged);
}

void Widget::slotRotationChanged(int index) {
    KScreen::Output::Rotation rotation
        = static_cast<KScreen::Output::Rotation>(ui->rotationBox->itemData(index).toInt());
    mOutput->setRotation(rotation);
    const KScreen::ConfigPtr &config = mConfig;
    auto *op = new KScreen::SetConfigOperation(config);
    /* Block until the operation is completed, otherwise KCMShell will terminate
     * before we get to execute the Operation */
    op->exec();
}

#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "globalmodels.h"
#include "gamepresetmodel.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    qRegisterMetaType<OptionItem>();
    QCoreApplication::setOrganizationName("ceehex");
    QCoreApplication::setOrganizationDomain("ceehex.com");
    QCoreApplication::setApplicationName("rr_launcher");

    QQmlApplicationEngine engine;
    auto globalModels = engine.singletonInstance<GlobalModels *>("rr_launcher", "GlobalModels");
    globalModels->s_singletonInstance = globalModels;
    globalModels->setPresetParser(new PresetParser);
    globalModels->setGamePresetModel(new GamePresetModel);

    //qmlRegisterSingletonInstance("com.rr_launcher.globalModel", 1, 0, "GlobalModels", &GlobalModels::instance());

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("rr_launcher", "Main");

    return app.exec();
}

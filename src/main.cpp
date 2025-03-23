#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "globalmodels.h"
#include "gamepresetmodel.h"
#include "updater.h"


int main(int argc, char *argv[])
{

    QGuiApplication app(argc, argv);
    qRegisterMetaType<OptionItem>();
    QCoreApplication::setOrganizationName("ceehex");
    QCoreApplication::setOrganizationDomain("ceehex.com");
    QCoreApplication::setApplicationName("hex_launcher");

    QQmlApplicationEngine engine;

    auto globalModels = engine.singletonInstance<GlobalModels *>("hex_launcher", "GlobalModels");
    globalModels->s_singletonInstance = globalModels;
    globalModels->setPresetParser(new PresetParser);
    globalModels->setGamePresetModel(new GamePresetModel);

    RetroRewind::Updater *updater = new RetroRewind::Updater(globalModels->presetParser());
    globalModels->setUpdater(updater);


    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("hex_launcher", "Main");

    return app.exec();
}

#include "globalmodels.h"
#include "gamepresetmodel.h"
#include "updater.h"
#include "gamesmodel.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>



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

    GamesModel *gamesModel = new GamesModel;
    globalModels->setPresetParser(new PresetParser(gamesModel));
    globalModels->setGamePresetModel(new GamePresetModel);
    globalModels->setGamesModel(gamesModel);

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

/****************************************************************************
** Generated QML type registration code
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <QtQml/qqml.h>
#include <QtQml/qqmlmoduleregistration.h>

#if __has_include(<gamepreset.h>)
#  include <gamepreset.h>
#endif
#if __has_include(<gamepresetmodel.h>)
#  include <gamepresetmodel.h>
#endif
#if __has_include(<globalmodels.h>)
#  include <globalmodels.h>
#endif
#if __has_include(<presetparser.h>)
#  include <presetparser.h>
#endif


#if !defined(QT_STATIC)
#define Q_QMLTYPE_EXPORT Q_DECL_EXPORT
#else
#define Q_QMLTYPE_EXPORT
#endif
Q_QMLTYPE_EXPORT void qml_register_types_rr_launcher()
{
    QT_WARNING_PUSH QT_WARNING_DISABLE_DEPRECATED
    qmlRegisterTypesAndRevisions<GamePreset>("rr_launcher", 1);
    qmlRegisterTypesAndRevisions<GamePresetModel>("rr_launcher", 1);
    qmlRegisterAnonymousType<QAbstractItemModel, 254>("rr_launcher", 1);
    qmlRegisterTypesAndRevisions<GlobalModels>("rr_launcher", 1);
    qmlRegisterTypesAndRevisions<PresetParser>("rr_launcher", 1);
    QT_WARNING_POP
    qmlRegisterModule("rr_launcher", 1, 0);
}

static const QQmlModuleRegistration rrlauncherRegistration("rr_launcher", qml_register_types_rr_launcher);

/*
 * Copyright (c) 2021 Daniel Volk <mail@volkarts.com>
 *
 * This work is licensed under the terms of the MIT license.
 * For a copy, see LICENSE or <https://opensource.org/licenses/MIT>.
 */

#include "DisplayManager.h"
#include "Dpms.h"
#include "Logging.h"
#include "SessionListener.h"
#include <QGuiApplication>
#include <QCommandLineParser>
#include <QTimer>
#include <csignal>
#include <iostream>
#include <functional>

using namespace std::placeholders;

namespace {

enum class CmdLineResult
{
    OK,
    Help,
    Version,
    Error,
};

struct Options
{
    QString displayName;
};

void catchUnixSignals(std::initializer_list<int> quitSignals)
{
    auto handler = [](int sig) -> void {
        LOG(DEBUG) << "Received signal " << sig;
        QTimer::singleShot(0, []() { QGuiApplication::quit(); });
    };

    sigset_t blocking_mask;
    sigemptyset(&blocking_mask);
    for (auto sig : quitSignals)
    {
        sigaddset(&blocking_mask, sig);
    }

    struct sigaction sa{};
    sa.sa_handler = handler;
    sa.sa_mask = blocking_mask;
    sa.sa_flags = 0;

    for (auto sig : quitSignals)
    {
        sigaction(sig, &sa, nullptr);
    }
}

CmdLineResult parseCommandLine(QCommandLineParser& parser, Options& options, QString& errorMessage)
{


    QCommandLineOption displayOpt{{QStringLiteral("d"), QStringLiteral("display")},
                                  QStringLiteral("The display to connect to"),
                                  QStringLiteral("display")};
    parser.addOption(displayOpt);

    QCommandLineOption helpOpt = parser.addHelpOption();
    QCommandLineOption versionOpt = parser.addVersionOption();

    parser.parse(QGuiApplication::arguments());

    if (parser.isSet(helpOpt))
        return CmdLineResult::Help;
    if (parser.isSet(versionOpt))
        return CmdLineResult::Version;

    if (parser.isSet(displayOpt))
    {
        options.displayName = parser.value(displayOpt);
    }

    return CmdLineResult::OK;
}

} // namespace

int main(int argc, char* argv[])
{
    catchUnixSignals({SIGQUIT, SIGINT, SIGTERM, SIGHUP});

    QGuiApplication::setOrganizationName(QStringLiteral("volkarts.com"));
    QGuiApplication::setApplicationName(QStringLiteral("LockScreenTimeout"));
    QGuiApplication::setApplicationVersion(QStringLiteral("1.0"));

    QGuiApplication app(argc, argv);

    QCommandLineParser parser;
    Options options;
    QString errorMessage;
    switch (parseCommandLine(parser, options, errorMessage))
    {
        case CmdLineResult::OK:
            break;

        case CmdLineResult::Help:
            parser.showHelp();
            Q_UNREACHABLE();
            break;

        case CmdLineResult::Version:
            std::cout << qPrintable(QGuiApplication::applicationName()) << " " <<
                         qPrintable(QGuiApplication::applicationVersion()) << std::endl;
            return 0;

        case CmdLineResult::Error:
            std::cerr << qPrintable(errorMessage) << "\n\n" <<
                         qPrintable(parser.helpText()) << std::endl;
            return 1;
    }

    auto dpms = Dpms::create();
    DisplayManager displayManager(dpms.get());
    SessionListener listener;

    QObject::connect(&listener, &SessionListener::sessionStateChanged,
                     &displayManager, &DisplayManager::onSessionStateChanged);

    LOG(INFO) << "Service started. Waiting for screen saver state changes";

    int ret = QGuiApplication::exec();

    LOG(INFO) << "Shutdown";

    return ret;
}

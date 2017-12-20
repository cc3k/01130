#include "global.h"

QString Global::SysDiagnosticAddress = "234.0.0.75";
quint16 Global::SysDiagnosticPortSend = 10000;
quint16 Global::SysDiagnosticPortListen = 10000;
QString Global::DiagnosticAddress = "234.0.0.75";
quint16 Global::DiagnosticPortSend = 10000;
quint16 Global::RemoteControlPort = 10001;

Global::Global()
{

}

QString Global::State::state(Global::State::states state)
{
    if (state == Global::State::good)
    {
        return "Исправно";
    }

    if (state == Global::State::bad)
    {
        return "Отказ";
    }

    if (state == Global::State::average)
    {
        return "Ухудшение";
    }

    if (state == Global::State::na)
    {
        return "Не определено";
    }

    if (state == Global::State::offcheck)
    {
        return "Снят с контроля";
    }

    if (state == Global::State::none)
    {
        return "Не контролируется";
    }

    return "";
}

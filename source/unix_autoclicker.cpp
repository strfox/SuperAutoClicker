#include "autoclicker.h"

#include <QDebug>

using sac::kb::keycomb_t;

keycomb_t sac::_bindings[CFGKEYS_AMOUNT] = {{0, false, false, false, false},
                                            {0, false, false, false, false},
                                            {0, false, false, false, false}};

void sac::AutoClicker::mainWindowPutMsg() {}

void sac::AutoClicker::startClickThread() {}

void sac::AutoClicker::stopClickThread() {}

sac::kb::keycomb_t getDefaultKeys() {}

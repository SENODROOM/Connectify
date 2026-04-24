#include "Session.h"

Session& Session::instance() {
    static Session s;
    return s;
}

#include "global.h"

std::function<void(QWidget*)> repolish = [](QWidget* wid){
    wid->style()->unpolish(wid);
    wid->style()->polish(wid);
};

QString gate_url_prefix = "";


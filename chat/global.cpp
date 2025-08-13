#include "global.h"

std::function<void(QWidget*)> repolish = [](QWidget* wid){
    wid->style()->unpolish(wid);
    wid->style()->polish(wid);
};

QString gate_url_prefix = "";

std::vector<QString>  strs ={
    "hello world !",
    "nice to meet u",
    "New year，new life",
    "You have to love yourself",
    "My love is written in the wind ever since the whole world is you"};
std::vector<QString> heads = {
    ":/icons/head_1.jpg",
    ":/icons/head_2.jpg",
    ":/icons/head_3.jpg",
    ":/icons/head_4.jpg",
    ":/icons/head_5.jpg"
};
std::vector<QString> names = {
    "llfc",
    "zack",
    "golang",
    "cpp",
    "java",
    "nodejs",
    "python",
    "rust"
};

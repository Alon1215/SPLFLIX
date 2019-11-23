//
// Created by alonmichaeli on 22/11/2019.
//
#include "../include/User.h"
class Watchable;

User::User(const std::string &name): name(name),history(new std::vector<Watchable*> ) {
    //check HISTORY
}
std::vector<Watchable*> User::get_history() const {
    return history;
}
std::string User::getName() const {
    return name;
}

User User::sub_Dup




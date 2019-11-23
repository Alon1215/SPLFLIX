//
// Created by alonmichaeli on 22/11/2019.
//

#include "../include/Action.h"



BaseAction::BaseAction() {
    status = PENDING;
}

void BaseAction:: complete() {
    status = COMPLETED;

}
void BaseAction::error(const std::string &errorMsg) {
    status = ERROR;
    this->errorMsg = errorMsg;
    std:: cout<<"Error -" + errorMsg ;
}

std::string BaseAction::getErrorMsg() const {
    //check if legal
    return errorMsg;
}
ActionStatus BaseAction::getStatus() const {
    return status;
}
std::string BaseAction::toString() const {} //check if right


void CreateUser::act(Session &sess) {}
CreateUser::BaseAction() {}



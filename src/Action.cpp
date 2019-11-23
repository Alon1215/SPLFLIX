//
// Created by alonmichaeli on 22/11/2019.
//

#include <cstring>
#include "../include/Action.h"
#include "../include/User.h"
#include "../include/Session.h"


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


void CreateUser::act(Session &sess) {
    std::unordered_map<std::string,User*> map = sess.getMap();
    std::string prefAlgo = sess.getPrefAlgo();
    std::string userName = sess.getUserName();

    if (sess.isInMap(userName)){
        error("The user name is already taken");
    } else{

        if(prefAlgo.compare("len") == 0){
            sess.insertMap(userName,new LengthRecommenderUser(userName));
        } else if (prefAlgo.compare("rer") == 0) {
            sess.insertMap(userName,new RerunRecommenderUser(userName));
        } else if (prefAlgo.compare("gen") == 0) {
            sess.insertMap(userName, new GenreRecommenderUser(userName));
        }else {error("Algorithm is not valid");}


    }
}
std::string CreateUser::toString() const {
    std::string ret = "CreateUser " + getStatus();
    if(getStatus()==ERROR){
        ret = ret + ": " + getErrorMsg();
    }
    return ret;
}
void ChangeActiveUser::act(Session &sess) {
    if (!sess.isInMap(sess.getUserName())){
        error("User is nont exist");
    } else
    User* newActiveUser =
}









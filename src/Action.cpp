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
std::string BaseAction::sub_ToString(const std::string & action_Name) const {
    std::string ret = action_Name + " ";
    if(getStatus()==ERROR){
        ret = ret + "ERROR: " + getErrorMsg();
    } else ret = ret + "COMPLETE";
    return ret;
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

void ChangeActiveUser::act(Session &sess) {
    if (!sess.isInMap(sess.getUserName())){
        error("User is not exist");
    } else {
        sess.set_Active_user(sess.getMap()[sess.getUserName()]);
        //check line while deBugging
    }
}
void DeleteUser::act(Session &sess) {
    //delete the user, then remove from userMap:
    std::string _user=sess.getUserName();
    if(!sess.isInMap(_user)){
        error("user is not exist");
    } else{
        delete sess.getMap().at(_user);
        sess.getMap().erase(_user);
    }
}
void DuplicateUser::act(Session &sess) {
    if (!sess.isInMap(sess.getUserName())){
        error("User is not exist");
    }else if(sess.isInMap(sess.getNameOfClone())){
        error("name is already taken");
    }else {
        // EMPTY ACTION -complete!
    }
}
void PrintContentList::act(Session &sess) {
    int i = 1;
    for (Watchable* x :sess.get_content()) {
            std::cout << i +". " + x.toString()<<endl;
            i++;
    }
}
void PrintWatchHistory::act(Session &sess) {
    int i = 1;
    for (Watchable* x :sess.get_Active_User().get_history()) {
        std::cout << i + ". " + x.toString()<<endl;
        i++;
    }
}
void Watch::act(Session &sess) {
    int id = sess.getIdToWatch();
    if (id<1 || id >= sess.get_content().size()){
        error("index is not valid");
    } else{
        // to complete
    }
}










// toString() functions:
std::string ChangeActiveUser::toString() const { return sub_ToString("ChangeActiveUser"); }
std::string DuplicateUser::toString() const {return sub_ToString("DuplicateUser");}
std::string CreateUser::toString() const { return sub_ToString("CreatUser");}
std::string DeleteUser::toString() const {return sub_ToString("DeleteUser");}
std::string PrintActionsLog::toString() const {return sub_ToString("PrintActionsLog");}
std::string PrintContentList::toString() const {return sub_ToString("PrintContentList");}
std::string PrintWatchHistory::toString() const {return sub_ToString("PrintWatchHistory");}
std::string Watch::toString() const {return sub_ToString("Watch");}









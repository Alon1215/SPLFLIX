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
std::string BaseAction::sub_ToString() const {
    std::string ret = this->toString() + " ";
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
        error("User is not exist");
    } else{
        delete sess.getMap().at(_user);
        sess.getMap().erase(_user);
    }
}

void PrintContentList::act(Session &sess) {
    int i = 1;
    for (Watchable* x :sess.get_content()) {
        printf("%d. %s",i,x->toString()); //check if prints legal line
        i++;
    }
}
void PrintWatchHistory::act(Session &sess) {
    int i = 1;
    for (Watchable* x :sess.get_Active_User().get_history()) {
        std::cout << i + ". " + x->toString()<<endl;
        i++;
    }
}

void PrintActionsLog::act(Session &sess) {
    std::string output;
    for (int i = sess.get_ActionsLog().size() - 1; i >= 0; i = i-1) {
        std::cout << sess.get_ActionsLog().at(i)->sub_ToString()<<std::endl; //check
    }
}

//empty actions:

void Watch::act(Session &sess) {
    int id = sess.getIdToWatch();
    //id already updated to place in content
    if (id<0 || id >= sess.get_content().size()){
        error("index is not valid");
    } else{
        Watchable& toWatch = sess.get_content().at(id);
        printf("Watching %s", toWatch.toString()); //check
        sess.get_Active_User().get_history().push_back(sess.get_content().at(id)); //check
        sess.get_Active_User().watch_handle_algo(id);

        printf("We recommend watching %s, continue watching?[y/n]")
        // to complete


        //sess.get_Active_User().fix_avg(sess.get_content().at(id-1)->get_length()); //
    }
}

void DuplicateUser::act(Session &sess) {
    if (!sess.isInMap(sess.getUserName())){
        error("User does not exist");
    }else if(sess.isInMap(sess.getNameOfClone())){
        error("Name is already taken");
    }else {
        // EMPTY ACTION -complete!
    }

}



//ActiveLog per user wasn't handled - actions are not added to log

// toString() functions:
std::string ChangeActiveUser::toString() const { return "ChangeActiveUser"; }
std::string DuplicateUser::toString() const {return "DuplicateUser";}
std::string CreateUser::toString() const { return "CreatUser";}
std::string DeleteUser::toString() const {return "DeleteUser";}
std::string PrintActionsLog::toString() const {return "PrintActionsLog";}
std::string PrintContentList::toString() const {return "PrintContentList";}
std::string PrintWatchHistory::toString() const {return "PrintWatchHistory";}
std::string Watch::toString() const {return "Watch";}








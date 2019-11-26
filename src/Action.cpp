//
// Created by alonmichaeli on 22/11/2019.
//
#include <string>
#include <cstring>
#include "../include/Action.h"
#include "../include/User.h"
#include "../include/Session.h"
#include "../include/Watchable.h"


BaseAction::BaseAction() {
    status = PENDING;
}

void BaseAction:: complete() {
    status = COMPLETED;

}

void BaseAction::error(const std::string &errorMsg) {
    status = ERROR;
    this->errorMsg = errorMsg;
    std:: cout<<"Error -" + errorMsg << std::endl;
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
        complete(); //unless algo is invalid. if so - corrects in last case
        if(prefAlgo.compare("len") == 0){
            LengthRecommenderUser *p=new LengthRecommenderUser(userName);
            sess.insertMap(userName,p);
        } else if (prefAlgo.compare("rer") == 0) {
            RerunRecommenderUser *p=new RerunRecommenderUser(userName);
            sess.insertMap(userName,p);
        } else if (prefAlgo.compare("gen") == 0) {
            GenreRecommenderUser *p=new GenreRecommenderUser(userName);
            sess.insertMap(userName, p);
        }else {error("Algorithm is not valid");}


    }
}

void ChangeActiveUser::act(Session &sess) {
    if (!sess.isInMap(sess.getUserName())){
        error("User is not exist");
    } else {
        sess.set_Active_user(sess.getMap()[sess.getUserName()]);
        //check line while deBugging
        complete();
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
        complete();
    }
}

void PrintContentList::act(Session &sess) {
    int i = 1;
    for (Watchable* x :sess.get_content()) {
        printf("%d. %s",i,x->content_string().c_str()); //fix
        i++;
    }
    complete();
}
void PrintWatchHistory::act(Session &sess) {
    int i = 1;
    for (Watchable* x :sess.get_Active_User().get_history()) {
        printf("%d. %s",i,x->toString().c_str());
        i++;
    }
    complete();
}

void PrintActionsLog::act(Session &sess) {
    std::string output;
    for (int i =(int) sess.get_ActionsLog().size() - 1; i >= 0; i = i-1) {
        std::cout << sess.get_ActionsLog().at(i)->sub_ToString()<<std::endl; //check
    }
    complete();
}

//empty actions:

void Watch::act(Session &sess) {
    int id = sess.getIdToWatch();
    //id already updated to place in content
    if (id<0 || id >= sess.get_content().size()){
        error("index is not valid");
    } else{
        complete();
        Watchable *toWatch = sess.get_content().at(id);
        printf("Watching %s", toWatch->toString().c_str()); //check
        sess.get_Active_User().watch_handle_algo(toWatch);
        Watchable *next = toWatch->getNextWatchable(sess);
        printf("We recommend watching %s, continue watching?[y/n]",next->toString().c_str());
        std::string input;
        std::cin >> input;
        if (input.compare("y")==0){
            sess.set_next_id(next->get_id());
            Watch *c1=new Watch();
            c1->act(sess);
            sess.get_ActionsLog().push_back(c1);
        }
        else if(input!= "n"){
            printf("wrong input, should be y or n");
        }
        // to complete


    }
}

void DuplicateUser::act(Session &sess) {
    if (!sess.isInMap(sess.getUserName())){
        error("User does not exist");
    }else if(sess.isInMap(sess.getPrefAlgo())){
        error("Name is already taken");
    }else {
        complete();
        User *newuser = sess.get_Active_User().duplicate(sess.getUserName());
        sess.insertMap(sess.getUserName(),newuser);
    }
}
void Exit::act(Session &sess) {
    printf("you chose to exit, goodbye!");
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
std::string Exit::toString() const {return "Exit";}







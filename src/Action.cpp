//
// Created by alonmichaeli on 22/11/2019.
//
#include <string>
#include "../include/Action.h"
#include "../include/User.h"
#include "../include/Session.h"
#include "../include/Watchable.h"


BaseAction::BaseAction() {
    status = PENDING;
}
BaseAction::~BaseAction() {}

void BaseAction:: complete() {
    status = COMPLETED;

}

void BaseAction::error(const std::string &errorMsg) {
    status = ERROR;
    this->errorMsg = errorMsg;
    std:: cout<<"Error - " + errorMsg << std::endl;
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
    if ((int )sess.get_vector_for_actions().size() != 3) {
        error("invalid input");
    }else {
        std::unordered_map<std::string, User *> map = sess.getMap();
        std::string prefAlgo = sess.get_vector_for_actions().at(2);
        std::string userName = sess.get_vector_for_actions().at(1);

        if (sess.isInMap(userName)) {
            error("The user name is already taken");
        } else {
            complete(); //unless algo is invalid. if so - corrects in last case
            if (prefAlgo == "len") {
                LengthRecommenderUser *p = new LengthRecommenderUser(userName);
                sess.insertMap(userName, p);
            } else if (prefAlgo == "rer") {
                RerunRecommenderUser *p = new RerunRecommenderUser(userName);
                sess.insertMap(userName, p);
            } else if (prefAlgo == "gen") {
                GenreRecommenderUser *p = new GenreRecommenderUser(userName);
                sess.insertMap(userName, p);
            } else { error("Algorithm is not valid"); }


        }
    }
}

void ChangeActiveUser::act(Session &sess) {
    if ((int )sess.get_vector_for_actions().size() != 2) {
        error("invalid input");
    }else {
        if (!sess.isInMap(sess.get_vector_for_actions().at(1))) {
            error("User is not exist");
        } else {
            sess.set_Active_user(sess.getMap()[sess.get_vector_for_actions().at(1)]);
            //check line while deBugging
            complete();
        }
    }
}
void DeleteUser::act(Session &sess) {
    //delete the user, then remove from userMap:
    if ((int )sess.get_vector_for_actions().size() != 2) {
        error("invalid input");
    }else {
        std::string _user = sess.get_vector_for_actions().at(1);
        if (!sess.isInMap(_user)) {
            error("User is not exist");
        } else {
            delete sess.getMap().at(_user);
            sess.erase_from_map(_user);
            complete();
        }
    }
}

void PrintContentList::act(Session &sess) {
    if ((int )sess.get_vector_for_actions().size() != 1) {
        error("invalid input");
    }else {
        int i = 1;
        for (Watchable *x :sess.get_content()) {
            printf("%d. %s\n", i, x->content_string().c_str()); //fix
            i++;
        }
        complete();
    }
}
void PrintWatchHistory::act(Session &sess) {
    if ((int )sess.get_vector_for_actions().size() != 1) {
        error("invalid input");
    }else {
        printf("Watch history for %s\n",sess.get_Active_User().getName().c_str());
        int i = 1;
        for (Watchable *x :sess.get_Active_User().get_history()) {
            printf("%d. %s\n", i, x->toString().c_str());
            i++;
        }
        complete();
    }
}

void PrintActionsLog::act(Session &sess) {
    if ((int )sess.get_vector_for_actions().size() != 1) {
        error("invalid input");
    }else {
        std::string output;
        for (int i = (int) sess.get_ActionsLog().size() - 1; i >= 0; i = i - 1) {
            std::cout << sess.get_ActionsLog().at(i)->sub_ToString() << std::endl; //check
        }
        complete();
    }
}

//empty actions:

void Watch::act(Session &sess) {
    if ((int)sess.get_vector_for_actions().size() != 2) {
        error("invalid input");
    }
    else {
        int id = sess.getIdToWatch();
        //id already updated to place in content
        if (id < 0 || id >= sess.get_content().size()) {
            error("index is not valid");
        } else {
            complete();
            Watchable *toWatch = sess.get_content().at(id);
            printf("Watching %s\n", toWatch->toString().c_str()); //check
            sess.get_Active_User().watch_handle_algo(toWatch);
            Watchable *next = toWatch->getNextWatchable(sess);
            printf("We recommend watching %s, continue watching?[y/n]\n", next->toString().c_str());
            std::string input;
            std::cin >> input;
            if (input == "y") {
                sess.set_next_id(next->get_id());
                Watch *c1 = new Watch();
                sess.get_ActionsLog().push_back(c1);
                c1->act(sess);
            } else if (input != "n") {
                printf("wrong input, should be y/n\n");
            }
        }
    }
}

void DuplicateUser::act(Session &sess) {
    if ((int )sess.get_vector_for_actions().size() != 3) {
        error("invalid input");
    }else {
        if (!sess.isInMap(sess.get_vector_for_actions().at(1))) {
            error("User does not exist");
        } else if (sess.isInMap(sess.get_vector_for_actions().at(2))) {
            error("Name is already taken");
        } else {
            complete();
            User *newuser = sess.get_Active_User().duplicate(sess.get_vector_for_actions().at(2));
            sess.insertMap(sess.get_vector_for_actions().at(2), newuser); //check if right index in vector
        }
    }
}
void Exit::act(Session &sess) {
    printf("you chose to exit, goodbye!\n");
    complete();
}

void BaseAction::sub_toClone(BaseAction &other) {
    errorMsg = other.errorMsg;
    status = other.status;
}

//cloning:
BaseAction* CreateUser::clone() {
    CreateUser* c1 = new CreateUser();
    c1->sub_toClone(*this);
    return  c1;
}
BaseAction* DuplicateUser::clone() {
    DuplicateUser* c1 = new DuplicateUser();
    c1->sub_toClone(*this);
    return  c1;
}

BaseAction* DeleteUser::clone() {
    DeleteUser* c1 = new DeleteUser();
    c1->sub_toClone(*this);
    return  c1;
}

BaseAction* ChangeActiveUser::clone() {
    ChangeActiveUser* c1 = new ChangeActiveUser();
    c1->sub_toClone(*this);
    return  c1;
}

BaseAction* PrintActionsLog::clone() {
    PrintActionsLog* c1 = new PrintActionsLog();
    c1->sub_toClone(*this);
    return  c1;
}

BaseAction* PrintWatchHistory::clone() {
    PrintWatchHistory* c1 = new PrintWatchHistory();
    c1->sub_toClone(*this);
    return  c1;
}

BaseAction* PrintContentList::clone() {
    PrintContentList* c1 = new PrintContentList();
    c1->sub_toClone(*this);
    return  c1;
}

BaseAction* Watch::clone() {
    Watch* c1 = new Watch();
    c1->sub_toClone(*this);
    return  c1;
}

BaseAction* Exit::clone() {
    Exit* c1 = new Exit();
    c1->sub_toClone(*this);
    return c1;
}




//ActiveLog per user wasn't handled - actions are not added to log

// toString() functions:
std::string ChangeActiveUser::toString() const { return "ChangeActiveUser"; }
std::string DuplicateUser::toString() const {return "DuplicateUser";}
std::string CreateUser::toString() const { return "CreateUser";}
std::string DeleteUser::toString() const {return "DeleteUser";}
std::string PrintActionsLog::toString() const {return "PrintActionsLog";}
std::string PrintContentList::toString() const {return "PrintContentList";}
std::string PrintWatchHistory::toString() const {return "PrintWatchHistory";}
std::string Watch::toString() const {return "Watch";}
std::string Exit::toString() const {return "Exit";}






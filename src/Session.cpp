//
// Created by michalon@wincs.cs.bgu.ac.il on 21/11/2019.
//

#include <fstream>
#include <cstdio>
#include "../include/Session.h"
#include "../include/Watchable.h"
#include "../include/User.h"
#include <vector>
#include <string>




Session::Session(const std::string &configFilePath):content(), actionsLog(),userMap(),activeUser(),vector_for_actions() {


    std::fstream ifs(configFilePath);
    nlohmann::json j = nlohmann::json::parse(ifs);
    nlohmann::json movies = j["movies"];
    int id = 1;
    for (auto &x : movies.items()) {
        nlohmann::json movie = x.value();
        Movie *newMovie = new Movie(id, movie["name"], movie["length"], movie["tags"]);
        content.push_back(newMovie);
        id++;
    }
    nlohmann::json tv_series = j["tv_series"];
    for (auto &x : tv_series.items()) {
        nlohmann::json series = x.value();
        std::vector<int> seasons = series["seasons"];
        int seasonNumber = 1;
        for (int y :seasons) {
            for (int episodeNumber = 1; episodeNumber <= y; episodeNumber++) {
                Episode *newEpisode = new Episode(id, series["name"], series["episode_length"], seasonNumber,
                episodeNumber, series["tags"]);
                content.push_back(newEpisode);
                id++;
            }
            seasonNumber++;
        }
    }
    User* p= new LengthRecommenderUser("default");
    insertMap("default",p);
    activeUser = p;
}



Session::Session(const Session &other): content(), actionsLog(),userMap(),activeUser(other.activeUser),vector_for_actions() { // copy consructor
    copy(other);
}

void Session::copy(const Session &other) { //for copying
    for (int i=0; i < (int) other.content.size(); i++){
        content.push_back(other.content.at(i)->clone());
    }
    for(int i=0; (unsigned) i<other.actionsLog.size();i++){
        BaseAction *p1 = other.actionsLog.at(i)->clone();
        actionsLog.push_back(p1);
    }
    for(auto i: other.userMap){
        User* newClone = i.second->clone();
        newClone->fix_History(*this);
        insertMap(i.first,newClone);
    }
    activeUser = userMap[other.activeUser->getName()];
}
void Session::steal(Session &other) {
    std::string activeName = other.activeUser->getName(); //name of active user
    for(int i=0;(unsigned)i<other.get_content().size();i++){
        Watchable* p=other.get_content().at(i);
        content.push_back(p);
        other.content.at(i)=nullptr;
    }
    for(int i=0;(unsigned) i<other.actionsLog.size();i++){
        actionsLog.push_back(other.actionsLog.at(i));
        other.actionsLog.at(i)=nullptr;
    }
    for(auto e:other.userMap){
        insertMap(e.first,e.second);
         other.userMap[e.first]=nullptr;
    }
    other.activeUser= nullptr;
    activeUser = userMap[activeName];
}

Session::Session(Session &&other):  content(), actionsLog(),userMap(),activeUser(other.activeUser),vector_for_actions() { //move constructor
   steal(other);
}
Session& Session::operator=(Session &&other) { //move assignment operator
    if(this!=&other){
        clear();
        //delete activeUser;
        steal(other);
    }
    return *this;
}



Session& Session::operator=(const Session &other) { //copy assignment operator
    if(this != &other){
        clear();
        activeUser = nullptr;
        copy(other);
    }
    return *this;
}


 User& Session::get_Active_User() const   {
    return *activeUser ;
}
std::vector<Watchable*> Session::get_content() const {
    return content;
}
void Session::clear() {
    for(auto x:content) {
        delete x;
    }for(auto y:actionsLog){
        delete y;
    }
    for(auto i:userMap){
        delete i.second;
    }
    content.clear();
    actionsLog.clear();
    vector_for_actions.clear();
    userMap.clear();

}
Session::~Session() {
    activeUser= nullptr;
    clear();
//    for(auto z:userMap){
//        delete z.second;
//        z.second = nullptr;
//    }
}
void Session::start() {
    printf("SPLFLIX is now on!\n");

    while(true){
        std::string input_string;
        printf("what would you like to do?\n");
        getline(std::cin,input_string);
        vector_for_actions = input_to_vector(input_string);

        if (((int)vector_for_actions.size())==0 || ((int)vector_for_actions.size()) >3 ){
            std::cout << "input is not valid"<< std::endl;

        }else if(vector_for_actions.at(0)=="createuser"){
            //std::cin >> third;
            CreateUser *p=new CreateUser() ;
            p->act(*this);
            actionsLog.push_back(p);
        }
        else if(vector_for_actions.at(0)=="changeuser"){
            ChangeActiveUser *p=new ChangeActiveUser();
            p->act(*this);
            actionsLog.push_back(p);

        }
        else if(vector_for_actions.at(0)=="deleteuser"){
            DeleteUser *p=new DeleteUser();
            p->act(*this);
            actionsLog.push_back(p);
        }
        else if(vector_for_actions.at(0)=="dupuser"){
            DuplicateUser *p=new DuplicateUser();
            p->act(*this);
            actionsLog.push_back(p);
        }
        else if(vector_for_actions.at(0)=="content"){
            PrintContentList *p=new PrintContentList();
            p->act(*this);
            actionsLog.push_back(p);
        }
        else if(vector_for_actions.at(0)=="watchhist") {
            PrintWatchHistory *p = new PrintWatchHistory();
            p->act(*this);
            actionsLog.push_back(p);
        }else if(vector_for_actions.at(0)=="log") {
            PrintActionsLog *p = new PrintActionsLog();
            p->act(*this);
            actionsLog.push_back(p);
        }else if(vector_for_actions.at(0)=="watch"){
            Watch *p=new Watch();
            actionsLog.push_back(p);
            p->act(*this);
            //prepare for next action:
            getline(std::cin,input_string);
            vector_for_actions = input_to_vector(input_string);
        }else if(vector_for_actions.at(0)=="exit") {
            Exit *p = new Exit();
            p->act(*this);
            actionsLog.push_back(p);
            break;
        } else {printf("invalid input\n");}
    }

}
void Session::set_Active_user(User* user_Ptr) {
    //recieves ptr to user and replace the ptr "active user"
    activeUser = user_Ptr;
}
int Session::getIdToWatch() {return stoi(vector_for_actions.at(1)) -1;} //-1 to Match content's index
void Session::set_next_id(int id) {
    vector_for_actions.at(1)=std::to_string(id);
}

std::vector<BaseAction*>& Session::get_ActionsLog() {

    return  actionsLog;
}
std:: unordered_map<std::string,User*> Session::getMap() {
    return userMap;
}
bool Session::isInMap(std::string s) {
    std::unordered_map<std::string,User*>::const_iterator got = userMap.find(s);

    return !(got == userMap.end());
}
void Session::insertMap(std::string s, User * u) {
    std::pair<std::string,User*> toinsert (s,u);
    userMap.insert(toinsert);
}



std::vector<std::string> Session::input_to_vector(std::string str) {
    std::string word = "";
    std::vector<std::string> output;
    for (auto x : str)
    {
        if (x == ' ')
        {
            std::string newWord = word;
            output.push_back(word);
            word = "";
        }
        else
        {
            word = word + x;
        }
    }
    output.push_back(word);
    return output;
}
std::vector<std::string> Session::get_vector_for_actions() { return vector_for_actions;}
void Session::erase_from_map(std::string st) {
    userMap.erase(st);
}

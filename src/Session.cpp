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




Session::Session(const std::string &configFilePath) {


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
}



Session::Session(const Session &other):activeUser(other.activeUser) { // copy consructor
    copy(other);
    userMap=other.userMap; //use copy assignment operator of unordered_map
}
Session::Session(Session &&other):activeUser(other.activeUser) { //move constructor
    other.activeUser= nullptr;
    userMap=other.userMap; //use move operator of unordered_map
    copy(other);
    other.activeUser= nullptr;
}
Session& Session::operator=(Session &&other) { //move assignment operator
    if(this!=&other){
        clear();
        delete activeUser;
        for(auto z:userMap)
            delete z.second;
        userMap=other.userMap; //move assignment operator
        copy(other);
        activeUser=other.activeUser;
        other.activeUser=nullptr;

    }

}

void Session::copy(const Session &other) {
    for (int i=0; i<other.content.size(); i++)
        content.push_back(other.content.at(i));
    for(int i=0; i<other.actionsLog.size();i++)
        actionsLog.push_back(other.actionsLog.at(i));

}

Session& Session::operator=(const Session &other) { //copy assingment operator
    if(this != &other){
        clear();
        delete activeUser;
        copy(other);
        activeUser=other.activeUser;
        userMap=other.userMap; //use copy assignment operator of unordered_map
    }
    return *this;
}


User& Session::get_Active_User()   {
    return *activeUser ;
}
const std::vector<Watchable*> Session::get_content() const {
    return content;
}
void Session::clear() {
    for(auto x:content)
        delete x;
    for(auto y:actionsLog)
        delete y;


}
Session::~Session() {
    delete activeUser;
    clear();
    for(auto z:userMap)
        delete z.second;
}
void Session::start() {
    printf("SPLFLIX is now on!\n");
    //std::cout <<"SPLFLIX is now on!" << std::endl;
    activeUser= new LengthRecommenderUser("default");
    while(true){
        printf("what would you like to do?\n");
        //std::cin >> command;
        //std::cin >> second;

        //ALON: new input action:
        std::string input_string;
        getline(std::cin,input_string);
        //std::vector<std::string> input_vector = input_to_vector(input_string);
        vector_for_actions = input_to_vector(input_string);
        //vector is now the words of the input line separated

        if (((int)vector_for_actions.size())==0 || ((int)vector_for_actions.size()) >3 ){
            std::cout << "in"
                         "put is not valid" << std::endl;

        }else if(vector_for_actions.at(0)=="createuser"){
            //std::cin >> third;
            CreateUser *p=new CreateUser() ;
            p->act(*this);
            actionsLog.push_back(p);
        }
        else if(vector_for_actions.at(0)=="changeuser"){
            if(activeUser->getName()=="default"){
                delete activeUser;
                activeUser=nullptr;
            }

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
            bool isWatchNext = true;
            actionsLog.push_back(p);
            p->act(*this);
        }else if(vector_for_actions.at(0)=="exit") {
            Exit *p = new Exit();
            p->act(*this);
            actionsLog.push_back(p);
            break;
        }else {(printf("invalid input\n"));}
    }

}
void Session::set_Active_user(User* user_Ptr) {
    //recieves ptr to user and replace the ptr "active user"
    activeUser = user_Ptr;
    //check if function is legal
}
int Session::getIdToWatch() {return stoi(vector_for_actions.at(1)) -1;} //-1 to Match content's index
void Session::set_next_id(int id) {
    vector_for_actions.at(1)=std::to_string(id);
}

std::vector<BaseAction*>& Session::get_ActionsLog() {

    return  actionsLog; //syntax is not valid
}
std:: unordered_map<std::string,User*> Session::getMap() {
    return userMap;
}
bool Session::isInMap(std::string s) {
    std::unordered_map<std::string,User*>::const_iterator got = userMap.find(s);

    if ( got == userMap.end() )
        return false;
    else
        return true;
}
void Session::insertMap(std::string s, User * u) {
    std::pair<std::string,User*> toinsert (s,u);
    userMap.insert(toinsert);
}


//new way to receive input
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

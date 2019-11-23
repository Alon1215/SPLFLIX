//
// Created by michalon@wincs.cs.bgu.ac.il on 21/11/2019.
//

#include <fstream>
#include "../include/Session.h"
#include "../include/Watchable.h"
class Movie;
class Episode;


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
    other.clear();
}
Session& Session::operator=(Session &&other) { //move assignment operator
    if(this!=&other){
        clear();
        delete activeUser;
        for(auto z:userMap)
            delete z.second;
        userMap=other.userMap; //move assignment operator
        copy(other);
        other.clear();
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


User& Session::get_User()   {
    return *activeUser ;
}
const std::vector<Watchable *> Session::get_content() const {
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





;
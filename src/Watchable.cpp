//
// Created by michalon@wincs.cs.bgu.ac.il on 22/11/2019.
//

#include "../include/Watchable.h"
#include <string>
#include "../include/Session.h"
#include "../include/User.h"

class User;
class Session;

Watchable::Watchable(long id, int length, const std::vector <std::string> &tags):id(id),length(length),tags(tags) {}

Movie::Movie(long id, const std::string &name, int length, const std::vector <std::string> &tags):Watchable(id,length,tags) ,name(name){}
long Watchable::get_id() const {
    return id;
}
int Watchable::get_length() const {
    return length;
}

const std::vector<std::string>& Watchable::get_tags() const {
    return tags ;
}
std::string Movie::toString() const {
    std::string st=name+" ";

}
Watchable* Movie::getNextWatchable(Session & s) const {
return s.get_Active_User().getRecommendation(s);

}

Episode::Episode(long id, const std::string &seriesName, int length, int season, int episode,
        const std::vector<std::string> &tags):Watchable(id,length,tags),seriesName(seriesName),season(season),episode(episode),nextEpisodeId(id+1){}

std::string Episode::toString() const {
    std::string st=seriesName+" S0"+std::to_string(season)+"E0"+std::to_string(episode)+" ";
    return st;
}
std::string Watchable::content_string() {
    std::string st=std::to_string(get_length())+" minutes [";
    std::vector<std::string> tags=get_tags();
    int i=0;
    for(auto tag = tags.begin();tag!=tags.end();++tag){ //adding the tags to the string
        if(i < tags.size()-1) { //if this is not the final tag add  a comma
            st += *tag + ", ";
        }
        else{
            st += *tag+"]";
        }
        ++i;
    }
    return st;
}


const std::string Episode::get_name() const {
    return seriesName;
}

const std::string Movie::get_name() const {
    return name;
}


Watchable* Episode::getNextWatchable(Session &s) const {
    Watchable *out = nullptr;
    if (nextEpisodeId < s.get_content().size()) { //check if there is a next episode available, idf so return it
        if (nextEpisodeId < s.get_content().size()) { //check if there is a next episode available, idf so return it
            Watchable *p = s.get_content().at(nextEpisodeId);
            if (seriesName == p->get_name()) {
                return p;

            }
        }
        return s.get_Active_User().getRecommendation(s); // else return algo' recommendtaion
    }

}
Watchable::~Watchable() {}

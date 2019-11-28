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
    return name;

}
Watchable* Movie::getNextWatchable(Session & s) const {
    return s.get_Active_User().getRecommendation(s);
}

Episode::Episode(long id, const std::string &seriesName, int length, int season, int episode,
        const std::vector<std::string> &tags):Watchable(id,length,tags),seriesName(seriesName),season(season),episode(episode),nextEpisodeId(id+1){}

std::string Episode::toString() const {
    //fix index for st:
    std::string e = "E";
    std::string s = "S";
    if (episode < 10){
        e = e + "0";
    }
    if (season < 10){
        s = s + "0";
    }
    //create st:
    std::string st=seriesName +" "+ s + std::to_string(season) + e + std::to_string(episode);
    return st;
}
std::string Watchable::content_string() {
    std::string st=std::to_string(get_length())+" minutes [";
    std::vector<std::string> _tags=get_tags();
    int i=0;
    for(auto tag = _tags.begin(); tag != _tags.end(); ++tag){ //adding the _tags to the string
        if((unsigned) i < _tags.size() - 1) { //if this is not the final tag add  a comma
            st += *tag + ", ";
        }
        else{
            st += *tag+"]";
        }
        i++;
    }
    st=" "+st;
    st =toString().c_str() + st;
    return st;
}


const std::string Episode::get_name() const {
    return seriesName;
}

const std::string Movie::get_name() const {
    return name;
}


Watchable* Episode::getNextWatchable(Session &s) const {
    if ((unsigned) nextEpisodeId < s.get_content().size()) {
        Watchable *p = s.get_content().at(nextEpisodeId-1);
        if (seriesName == p->get_name()) { //check if there is a next episode available, if so return it
            return p;
        }
    }
    return s.get_Active_User().getRecommendation(s);
    // else return algo' recommendtaion
}
Watchable::~Watchable() {}

Watchable* Episode::clone() {
    std::vector<std::string> newtags;
    for(int i=0;i<(int)get_tags().size();i++){
        newtags.push_back(get_tags().at(i));
    }
    Episode *e=new Episode(get_id(),seriesName,get_length(),season,episode,newtags);
    return e;
}
Watchable* Movie::clone() {
    std::vector<std::string> newtags;
    for(int i=0;i<(int)get_tags().size();i++){
        newtags.push_back(get_tags().at(i));
    }
    Movie *e=new Movie(get_id(),name,get_length(),newtags);
    return e;
}

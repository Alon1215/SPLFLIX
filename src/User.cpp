//
// Created by alonmichaeli on 22/11/2019.
//
#include "../include/User.h"
#include "../include/Watchable.h"
#include "../include/Session.h"
#include <utility>
#include <vector>

#include <stdlib.h>
class Watchable;

User::User(const std::string &name): name(name),history( std::vector<Watchable*> ()) {
    //check HISTORY
}

std::vector<Watchable*> User::get_history() const {
    return history;
}
std::string User::getName() const {
    return name;
}

//LengthRecommenderUser

LengthRecommenderUser::LengthRecommenderUser(const std::string &name): User(name),avg(0) {

}

std::vector<Watchable*> User::get_unwatched(Session& s) {
    std::vector<Watchable*> output;
    for(int i=0;i<s.get_content().size();i++){ //iterate through all content
        bool watched=false;
        Watchable *e=(s.get_content().at((i)));
        for(int j=0;j<history.size() and !watched;j++) { //iterate through history
            if(e->get_id() == history.at(j)->get_id()) //check if we watched this content
                watched=true;
        }
        if(!watched){ //if this content was not watched check if it the new potential recommendation
           output.push_back(s.get_content().at(i));
            }
        }
    return output;
}


Watchable* LengthRecommenderUser::getRecommendation(Session &s) {
    float mindif=-1;
    Watchable *out=nullptr;
    for(int i=0;i<s.get_content().size();i++){ //iterate through all content
        bool watched=false;
        Watchable *e=(s.get_content().at((i)));
        for(int j=0;j<history.size() and !watched;j++) { //iterate through history
            if(e->get_id() == history.at(j)->get_id()) //check if we watched this content
                watched=true;
        }
        if(!watched){ //if this content was not watched check if it the new potential recommendation
            float dif=abs(e->get_length()-avg);
            if(abs(dif<mindif or mindif==-1)){
                mindif=dif;
                out=e;
            }
        }
    }
    return out;
}
Watchable* RerunRecommenderUser::getRecommendation(Session &s) {
    if(lastrec==-1) //check if this is the first reccomendtaion
        lastrec = 0;
    else
        lastrec= (lastrec+1)%(int)history.size();
    return history.at(lastrec);

}
std::string GenreRecommenderUser::get_next_tag(std::vector<std::pair<std::string, int>> &tags) { //get next popular tag
    std::pair<std::string,int> &max=tags.at(0);
    int maxindex=0;
    for(int i=1;i<tags.size();i++){
        std::pair<std::string,int> &p=tags.at(i);
        if(p.second>max.second){
            max=p;
            maxindex=i;
        }
        else if(p.second==max.second){
            if(p.first.compare(p.first)>0){
                max=p;
                maxindex=i;
            }
        }
    }
    std::string output=max.first;
    tags.erase(tags.begin()+maxindex);
    return output;
}

Watchable* GenreRecommenderUser::getRecommendation(Session &s) {
    std::vector<std::pair <std::string, int>> tagcount; //vector of pairs containing tag and count
    Watchable* out=nullptr;
    for(int i=0;i<history.size();i++){
        Watchable *e=(history.at((i)));
        for(int j=0;j<e->get_tags().size();j++){ //iterate through tags of e
            bool found=false;
            std::string const &tag=e->get_tags().at((j));
            for(int k=0;k<tagcount.size()&!found;k++){
                if(tagcount.at(k).first== tag){ //this tag was already found
                    found=true;
                    tagcount.at(k).second++; //update tag counter
                }
            if(!found){ //if this is a new tag create a pair with count 1 and add to tags vector
                std::pair<std::string,int> p =std::make_pair(tag,1);
                tagcount.push_back(p);
            }
            }
        }
    }
        bool found=false;
        std::vector <Watchable*> unwatched=get_unwatched(s); //get unwatched content
        std::string next_tag;
        while(!found and !tagcount.empty()){
            next_tag=get_next_tag(tagcount); //get most popular tag
            for(int i=0;i<unwatched.size();i++){ //look for a unwatched Watchable with this tag
                Watchable* p=unwatched.at(i);
                for(int j=0;j<p->get_tags().size();j++){ //iterate through this Watchable's tags
                    if(p->get_tags().at(j)==next_tag){ //if we found the tag this will be recommended
                        found=true;
                        out=p;
                    }
                }
            }
        }
        return out;
}
void LengthRecommenderUser::fix_avg(int length) {
    avg= (avg*(get_history().size()-1) +length)/get_history().size();
}

void LengthRecommenderUser::watch_handle_algo( Watchable *watched) {
    addToHistory(watched);
    fix_avg(watched->get_length());
}
void GenreRecommenderUser::watch_handle_algo(Watchable *watched) {addToHistory(watched);}
void RerunRecommenderUser::watch_handle_algo(Watchable *watched) {addToHistory(watched);}
void User::addToHistory(Watchable *toAdd) {
    history.push_back(toAdd);
}





//RerunRecommenderUser:


//GenreRecommenderUser:


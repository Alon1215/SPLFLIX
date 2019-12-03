#include <iostream>
#include "include/Session.h"

int main() {
    //Session *s=new Session("/users/studs/bsc/2020/mosesofe/Desktop/SPL/config1.json");
    /*s->start();
    Session *s2= s;
    delete s;
    s2->start();
    delete s2;*/
    Session* s1 = new Session("/users/studs/bsc/2020/mosesofe/Desktop/SPL/config1.json"); //const
    s1->start();
    Session* s2 = new Session("/users/studs/bsc/2020/mosesofe/Desktop/SPL/config1.json");
    s2->start();
    *s2 = std::move(*s1); //move assignment
    delete(s1);
    s2->start();
    Session* s3(new Session("/users/studs/bsc/2020/mosesofe/Desktop/SPL/config1.json")); //move const
    s3->start();
    *s2 = *s3; //operator=
    s2->start();
    delete (s3);
    Session* s4 = new Session("/users/studs/bsc/2020/mosesofe/Desktop/SPL/config1.json");
    s4->start();
    Session* s5 = s4;
    *s5 = std::move(*s4); //self assignment
    s5->start();
    *s2 = * s4; //operator=
    delete(s4);
    s2->start();
    delete(s2);
    return 0;
}
/*

int main(int argc, char** argv){
    if(argc!=2)
    {
        std::cout << "usage splflix input_file" << std::endl;
        return 0;
    }
    Session* s = new Session("/users/studs/bsc/2020/mosesofe/Desktop/SPL/config3 (copy).json");
    s->start();
    Session s2("/users/studs/bsc/2020/mosesofe/Desktop/SPL/config3 (copy).json");
    s2 = std::move(s2.forTestOnly());
    s2=*s;
    delete(s);
    s2.start();

    return 0;
}
*/

#include <string>
#include <iostream>
#include <fstream>
#include "array_set.hpp"
#include "array_map.hpp"
#include "q1solution.hpp"


//////////
//
// Do not edit this file
//
//////////

int main() {
  try{
    //Testing swap
    typedef ics::ArraySet<std::string> string_set;

    std::cout << std::endl << "Testing swap" << std::endl;
    ics::ArrayMap<std::string,string_set> names_set (
        {
         ics::pair<std::string,string_set>("Boy", string_set({"Mary","Betty","Mimsi"})),
         ics::pair<std::string,string_set>("Girl",string_set({"Peter","Joey","Joe","Carl"}))
        }
    );
    std::cout << "Original Map = " << names_set << std::endl;
    //Oops, got the names backwards; let's swap values mapped to/from "Boy" and "Girl"
    swap(names_set, std::string("Boy"), std::string("Girl"));
    std::cout << "Swapped  Map = " << names_set << std::endl;


    //Testing values_set_to_queue
    typedef ics::ArrayQueue<std::string> string_queue;

    std::cout << std::endl << "Testing values_set_to_queue" << std::endl;
    std::cout << "Original Map = " << names_set << std::endl;
    ics::ArrayMap<std::string,string_queue> names_queue;
    values_set_to_queue(names_set, names_queue);
    std::cout << "New Map = " << names_queue << std::endl;


    //Testing sort_xys, sort_angle, points
    typedef int                      ordinal;
    typedef ics::pair<ordinal,Point> op_entry;

    ics::ArrayMap<ordinal,Point> ps1(
      {
         op_entry(1,Point(1,1)),
         op_entry(2,Point(3,2)),
         op_entry(3,Point(3,-3)),
         op_entry(4,Point(-3,4)),
         op_entry(5,Point(-2,-2))
      }
    );

    ics::ArrayMap<ordinal,Point> ps2(
      {
         op_entry(1,Point(0,5)),
         op_entry(2,Point(2,3)),
         op_entry(3,Point(-3,2)),
         op_entry(4,Point(-5,1)),
         op_entry(5,Point(-3,-2)),
         op_entry(6,Point(4,-2)),
         op_entry(7,Point(5,0)),
         op_entry(8,Point(0,-5))
      }
    );

    std::cout << std::endl << "Testing sort_xys" << std::endl;
    std::cout << "sort_xys(ps1) = " << sort_xys(ps1) << std::endl;
    std::cout << "sort_xys(ps2) = " << sort_xys(ps2) << std::endl;


    std::cout << std::endl << "Testing sort_angle" << std::endl;
    std::cout << "sort_angle(ps1) = " << sort_angle(ps1) << std::endl;
    std::cout << "sort_angle(ps2) = " << sort_angle(ps2) << std::endl;


    std::cout << std::endl << "Testing points" << std::endl;
    std::cout << "points(ps1) = " << points(ps1) << std::endl;
    std::cout << "points(ps2) = " << points(ps2) << std::endl;


    std::cout << std::endl << "Testing first_quad" << std::endl;
    std::cout << "first_quad(ps1) = " << first_quad(ps1) << std::endl;
    std::cout << "first_quad(ps2) = " << first_quad(ps2) << std::endl;


    //Testing near
    std::cout << std::endl << "Testing near" << std::endl;
    std::cout << near("radar",1) << std::endl;
    std::cout << near("radar",2) << std::endl;
    std::cout << near("radar",3) << std::endl;
    std::cout << near("whiplash",1) << std::endl;
    std::cout << near("whiplash",2) << std::endl;
    std::cout << near("whiplash",3) << std::endl;
    std::cout << near("whiplash",4) << std::endl;
    std::cout << near("whiplash",5) << std::endl;
    std::cout << near("whiplash",6) << std::endl;
    std::cout << near("whiplash",7) << std::endl;


    //Testing got_called and invert
    typedef std::string                       caller;
    typedef std::string                       callee;
    typedef ics::ArrayMap<callee,int>         call_count;
    typedef ics::ArrayMap<caller, call_count> db;
    typedef ics::pair<callee,int>             cc_entry;
    typedef ics::pair<caller,call_count>      db_entry;


    db calls1 (
        {
          db_entry("a",call_count({cc_entry("b",2),cc_entry("c",1)})),
          db_entry("b",call_count({cc_entry("a",3),cc_entry("c",1)})),
          db_entry("c",call_count({cc_entry("a",1),cc_entry("d",2)}))
        }
    );

    db calls2 (
        {
          db_entry("a",call_count({cc_entry("b",2),cc_entry("c",1),cc_entry("e",2)})),
          db_entry("b",call_count({cc_entry("a",3),cc_entry("c",1)})),
          db_entry("c",call_count({cc_entry("a",1),cc_entry("d",2),cc_entry("e",6)})),
          db_entry("e",call_count({cc_entry("a",3),cc_entry("c",2),cc_entry("d",1)}))
        }
    );

    std::cout << std::endl << "Testing got_called" << std::endl;
    std::cout << "got_called(calls1) = " << got_called(calls1) << std::endl;
    std::cout << "got_called(calls2) = " << got_called(calls2) << std::endl;

    std::cout << std::endl << "Testing invert" << std::endl;
    std::cout << "invert(calls1) = " << invert(calls1) << std::endl;
    std::cout << "invert(calls2) = " << invert(calls2) << std::endl;



} catch (ics::IcsError& e) {
    std::cout << e.what() << std::endl;}
  return 0;
}



/*
Here is the output that the driver should produce:


Testing swap
Original Map = map[Boy->set[Mary,Betty,Mimsi],Girl->set[Peter,Joey,Joe,Carl]]
Swapped  Map = map[Boy->set[Peter,Joey,Joe,Carl],Girl->set[Mary,Betty,Mimsi]]

Testing values_set_to_queue
Original Map = map[Boy->set[Peter,Joey,Joe,Carl],Girl->set[Mary,Betty,Mimsi]]
New Map = map[Boy->queue[Peter,Joey,Joe,Carl]:rear,Girl->queue[Mary,Betty,Mimsi]:rear]

Testing sort_xys
sort_xys(ps1) = queue[pair[4,(-3,4)],pair[5,(-2,-2)],pair[1,(1,1)],pair[2,(3,2)],pair[3,(3,-3)]]:rear
sort_xys(ps2) = queue[pair[4,(-5,1)],pair[3,(-3,2)],pair[5,(-3,-2)],pair[1,(0,5)],pair[8,(0,-5)],pair[2,(2,3)],pair[6,(4,-2)],pair[7,(5,0)]]:rear

Testing sort_angle
sort_angle(ps1) = queue[(-2,-2),(3,-3),(3,2),(1,1),(-3,4)]:rear
sort_angle(ps2) = queue[(-3,-2),(0,-5),(4,-2),(5,0),(2,3),(0,5),(-3,2),(-5,1)]:rear

Testing points
points(ps1) = queue[(1,1),(3,2),(3,-3),(-3,4),(-2,-2)]:rear
points(ps2) = queue[(0,5),(2,3),(-3,2),(-5,1),(-3,-2),(4,-2),(5,0),(0,-5)]:rear

Testing first_quad
first_quad(ps1) = map[(1,1)->1.41421,(3,2)->3.60555]
first_quad(ps2) = map[(0,5)->5,(2,3)->3.60555,(5,0)->5]

Testing near
map[r->set[r,a],a->set[r,a,d],d->set[a,d]]
map[r->set[r,a,d],a->set[r,a,d],d->set[r,a,d]]
map[r->set[r,a,d],a->set[r,a,d],d->set[r,a,d]]
map[w->set[w,h],h->set[w,h,i,s],i->set[h,i,p],p->set[i,p,l],l->set[p,l,a],a->set[l,a,s],s->set[a,s,h]]
map[w->set[w,h,i],h->set[w,h,i,p,a,s],i->set[w,h,i,p,l],p->set[h,i,p,l,a],l->set[i,p,l,a,s],a->set[p,l,a,s,h],s->set[l,a,s,h]]
map[w->set[w,h,i,p],h->set[w,h,i,p,l,a,s],i->set[w,h,i,p,l,a],p->set[w,h,i,p,l,a,s],l->set[h,i,p,l,a,s],a->set[i,p,l,a,s,h],s->set[p,l,a,s,h]]
map[w->set[w,h,i,p,l],h->set[w,h,i,p,l,a,s],i->set[w,h,i,p,l,a,s],p->set[w,h,i,p,l,a,s],l->set[w,h,i,p,l,a,s],a->set[h,i,p,l,a,s],s->set[i,p,l,a,s,h]]
map[w->set[w,h,i,p,l,a],h->set[w,h,i,p,l,a,s],i->set[w,h,i,p,l,a,s],p->set[w,h,i,p,l,a,s],l->set[w,h,i,p,l,a,s],a->set[w,h,i,p,l,a,s],s->set[h,i,p,l,a,s]]
map[w->set[w,h,i,p,l,a,s],h->set[w,h,i,p,l,a,s],i->set[w,h,i,p,l,a,s],p->set[w,h,i,p,l,a,s],l->set[w,h,i,p,l,a,s],a->set[w,h,i,p,l,a,s],s->set[w,h,i,p,l,a,s]]
map[w->set[w,h,i,p,l,a,s],h->set[w,h,i,p,l,a,s],i->set[w,h,i,p,l,a,s],p->set[w,h,i,p,l,a,s],l->set[w,h,i,p,l,a,s],a->set[w,h,i,p,l,a,s],s->set[w,h,i,p,l,a,s]]

Testing got_called
got_called(calls1) = map[b->2,c->2,a->4,d->2]
got_called(calls2) = map[b->2,c->4,e->8,a->7,d->3]

Testing invert
invert(calls1) = map[b->map[a->2],c->map[a->1,b->1],a->map[b->3,c->1],d->map[c->2]]
invert(calls2) = map[b->map[a->2],c->map[a->1,b->1,e->2],e->map[a->2,c->6],a->map[b->3,c->1,e->3],d->map[c->2,e->1]]
*/
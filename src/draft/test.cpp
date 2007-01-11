/***************************************************************************
 *   This file is part of the FrB project                                  *
 *                                                                         *
 *   FrB project                                                           *
 *                                                                         *
 *   Copyright (C) 2006, 2007 by GUILLAUME Nicolas                         *
 *   ng@ngsoft-fr.com                                                      *
 *                                                                         *
 *   http://frb.ngsoft-fr.com/                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License only.          *
 *   See the COPYING file.                                                 *
 ***************************************************************************/





#include <iostream>
#include <map>
#include <vector>




#include <iterator>

template <class C>
class map_snd_iterator
{
public:
    typedef typename C::iterator base_iterator_type;
    
private:
    base_iterator_type _it;
        
public:
    typedef typename base_iterator_type::iterator_category iterator_category;
    typedef typename base_iterator_type::value_type        value_type;
    typedef typename base_iterator_type::difference_type   difference_type;
    typedef typename base_iterator_type::pointer           pointer;
    typedef typename base_iterator_type::reference         reference;
    typedef typename base_iterator_type::difference_type   difference_type;

    
    map_snd_iterator(base_iterator_type it) : _it(it) {}
        
    const typename C::key_type & key() { return _it->first; }
    typename C::mapped_type & operator*() { return _it->second; }
    bool operator != (const map_snd_iterator & m) const { return _it != m._it; }
    map_snd_iterator & operator++()  { ++_it; return * this; }


};

template <class C>
class const_map_snd_iterator
{
public:
    typedef typename C::const_iterator base_iterator_type;
    
private:
    base_iterator_type _it;
        
public:
    typedef typename base_iterator_type::iterator_category iterator_category;
    typedef typename base_iterator_type::value_type        value_type;
    typedef typename base_iterator_type::difference_type   difference_type;
    typedef typename base_iterator_type::pointer           pointer;
    typedef typename base_iterator_type::reference         reference;
    typedef typename base_iterator_type::difference_type   difference_type;

    const_map_snd_iterator(base_iterator_type it) : _it(it) {}
        
    const typename C::key_type & key() { return _it->first; }
    const typename C::mapped_type & operator*() { return _it->second; }
    bool operator != (const const_map_snd_iterator & m) const { return _it != m._it; }
    const_map_snd_iterator & operator++() { ++_it; return * this; }


};




using namespace std;




template<class T>
void printi(T begin,T end)
{
    while(begin != end)
    {
        cout << "e=" << *begin << endl;
        
        ++begin;
    }

}


int main()
{
    vector<int> a1;
    
    a1.push_back(5);
    a1.push_back(8);
    a1.push_back(5);
    a1.push_back(6);
    a1.push_back(-1);
    
    
    map<string, int> m1;
    
    m1["salut"] = 77;
    m1["vous"] = 76;
    m1["test1"] = 1;
    m1["test3"] = 2;
    m1["bouc"] = 18;
    m1["dé"] = 6;
    
    
    cout << "a1:" << endl;
    printi(a1.begin(), a1.end());
    
    cout << "m1:" << endl;

    printi(map_snd_iterator< map<string, int> >(m1.begin()), map_snd_iterator< map<string, int> >(m1.end()));
}

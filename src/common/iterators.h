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


#include <iterator>

/** Map iterator returning 'second' only */
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

/** Map const_iterator returning 'second' only */
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


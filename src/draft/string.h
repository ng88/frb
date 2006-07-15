
#ifndef STRING_H
#define STRING_H

#include <sstream>

#include <string>
#include <vector>

#include <cctype>

typedef std::string String;
typedef std::vector<String> StringList;

namespace StringEx /* extended functions on string */
{
    /* split source in container using delim as delimiter */
    template<class T>
    size_t split(T &container, const String& source, char delim = '\n', bool allowEmptyEntries = true)
    {
        size_t found = 0;
        String::const_iterator left = source.begin();
        for( String::const_iterator right = std::find(left, source.end(), delim);
             right != source.end(); right = std::find(left, source.end(), delim) )
        {
            if( allowEmptyEntries || (!allowEmptyEntries && left != right) )
                container.push_back( String(left,right) );
                
            left = right + 1;
            found++;
        }
        return found;
    }
    
    /* return true if stris only made of ' ' and '\t' */
    bool isSpacesOnly(const String& str);
    
    
    inline String left(const String& str, const String::size_type& pos)
    {
        return String( str, 0, pos );
    }
    
    inline String right(const String& str, const String::size_type& pos)
    {
        return String( str, str.length() - 1 - pos, pos );
    }
    
    inline bool isSpace(const char& c)
    {
        return isspace(c);
    }
    
    String ltrim(const String& str);
    String rtrim(const String& str);
    String trim(const String& str);
    
    inline String int2string(int v)
    {
        std::ostringstream tmp;
        tmp << v;
        return tmp.str();
    }
    
    inline String double2string(double v)
    {
        std::ostringstream tmp;
        tmp << v;
        return tmp.str();
    }
}


#endif



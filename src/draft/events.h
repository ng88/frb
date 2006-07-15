
#ifndef EVENT_H
#define EVENT_H

// d�clare un �v�nement 
#define DEC_EVENT(name) name##Callback __eventsys_##name##Event

// connecte un �v�nement
#define CONNECT_EVENT(name, fn) __eventsys_##name##Event = (fn)

// appelle un �v�nement                 
#define EMIT_EVENT(name, param) \
    { if( __eventsys_##name##Event != NULL ) \
          __eventsys_##name##Event(param); }

// d�clare les param�tres de la fonction cliente
#define PARAM_FN_EVENT(name) const name##Infos&

struct ErrorInfos
{
    int line;
    int col;
    int errid;
    String errstr;
    String code;
};

typedef void (*ErrorCallback)(const ErrorInfos& e);

struct DebugInfos
{
    String caller;
    String debugstr;
};

typedef void (*DebugCallback)(const DebugInfos& e);

#endif

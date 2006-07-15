#ifndef FRBSYNTAXER_H
#define FRBSYNTAXER_H


#ifndef YY_FrBSyntaxicalParser_MEMBERS
#define YY_FrBSyntaxicalParser_MEMBERS \
     public: \
        FrBFlexLexerEx * frb_lexer; \
        FrBClassMap * frb_classes; \
        FrBErrorCollector * frb_error; \
        inline String currentClassName() \
        {  \
            if(class_stack.empty()) \
                return ""; \
            else \
                return class_stack.top()->name(); \
        } \
        inline String currentFnName() \
        {  \
                return ""; \
        } \
     private: \
         FrBCodeClassStack class_stack; \
         FrBCodeFunctionStack fn_stack; \
         CStringList id_list; \
         inline FrBCodeClass* current_class() \
         { \
             frb_assert2(!class_stack.empty(), "frbsyntaxer.h::FrBSynater::current_class()");  \
             return class_stack.top(); \
         } \
         inline FrBCodeFunction* current_fn() \
         { \
             frb_assert2(!fn_stack.empty(), "frbsyntaxer.h::FrBSynater::current_fn()");  \
             return fn_stack.top(); \
         } \
     public: \
        virtual ~FrBSyntaxicalParser() {} \
        void switch_lexer_streams( std::istream* new_in = 0, std::ostream* new_out = 0 )\
        { \
            frb_lexer->switch_streams(new_in, new_out); \
        } \
        \


#endif

#ifndef YY_FrBSyntaxicalParser_ERROR_BODY
#define YY_FrBSyntaxicalParser_ERROR_BODY \
        {  \
            FrBErrorDescriptor err = FrBErrorDescriptor::fromYYError(msg, frb_lexer->YYText()); \
             \
            err.setFileName(""); \
            err.setClassName(""); \
            err.setFunctionName(""); \
            err.setLine(frb_lexer->lineno()); \
     \
            frb_error->error(err);\
        }
#endif


#ifndef  YY_FrBSyntaxicalParser_LEX_BODY
#define YY_FrBSyntaxicalParser_LEX_BODY { return frb_lexer->yylex(); }
#endif

#endif

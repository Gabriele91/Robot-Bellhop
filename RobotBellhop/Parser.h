//
//  Parser.h
//  RobotFattorino
//
//  Created by Gabriele Di Bari on 28/03/15.
//  Copyright (c) 2015 Gabriele Di Bari. All rights reserved.
//

#ifndef RobotFattorino_Parser_h
#define RobotFattorino_Parser_h

#include "Config.h"
#include "Object.h"
#include "Room.h"
#include "House.h"
#include "ConfigFile.h"

#define CSTRCMP( x, y ) ( std::strncmp( x, y, sizeof(y)-1 ) == 0 )
#define CSTRCMP_SKIP( x, y ) ( [&x] () -> bool { if ( CSTRCMP(x,y) ) { x += sizeof(y)-1; return true; } return false; } )()

namespace RobotBellhop
{

class Parser
{
protected:
    //////////////////////////////////////////////////////
    size_t m_line;
    ConfigFile* m_config_file;
    std::list< size_t >      m_errors_line;
    std::list< std::string > m_errors;
    //////////////////////////////////////////////////////
    enum Type
    {
        T_CONFIG,
        T_CONSTS,
        T_START,
        T_END
    };
    //////////////////////////////////////////////////////
    static bool is_line_space(char c)
    {
        return 	 c == ' ' || c == '\t';
    }
    static bool is_space(char c)
    {
        return 	 c == ' ' || c == '\t' || c == '\r' || c == '\n';
    }
    static bool is_start_name(char c)
    {
        return 	 (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
    }
    static bool is_char_name(char c)
    {
        return 	 (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c=='_' ;
    }
    static bool is_uint_number(char c)
    {
        return 	 (c >= '0' && c <= '9');
    }
    static bool is_start_int_number(char c)
    {
        return 	 (c >= '0' && c <= '9')|| c=='-' ;
    }
    static bool is_start_table(char c)
    {
        return (c == '{');
    }
    static bool is_end_table(char c)
    {
        return (c == '}');
    }
    static bool is_start_arg(char c)
    {
        return (c == '(');
    }
    static bool is_end_arg(char c)
    {
        return (c == ')');
    }
    static bool is_start_obj_list(char c)
    {
        return (c == '|');
    }
    static bool is_line_comment(const char* c)
    {
        return (*c)=='/' && (*(c+1))=='/';
    }
    static bool is_start_multy_line_comment(const char* c)
    {
        return (*c)=='/' && (*(c+1))=='*';
    }
    static bool is_end_multy_line_comment(const char* c)
    {
        return (*c)=='*' && (*(c+1))=='/';
    }
    void skeep_line_comment(const char*& inout)
    {
        if(is_line_comment(inout))
        {
            while(*(inout)!=EOF &&
                  *(inout)!='\0'&&
                  *(inout)!='\n')
                 ++(inout);
        }
    }
    void skeep_multy_line_comment(const char*& inout)
    {
        if(is_start_multy_line_comment(inout))
        {
            while(*(inout)!=EOF &&
                  *(inout)!='\0'&&
                  !is_end_multy_line_comment(inout))
            {
                m_line+=(*(inout))=='\n';
                ++(inout);
            }
            if((*(inout))=='*') ++(inout);
        }
    }
    void skeep_space_end_comment(const char*& inout)
    {
        while(is_space(*(inout))||
              is_line_comment(inout)||
              is_start_multy_line_comment(inout))
        {
            skeep_line_comment(inout);
            skeep_multy_line_comment(inout);
            m_line+=(*(inout))=='\n';
            ++(inout);
        }
    }
    void skeep_line_space(const char*& inout)
    {
        while(is_line_space(*inout)) ++(inout);
    }
    static bool is_config_and_skip(const char*& c)
    {
        return CSTRCMP_SKIP(c,"CONFIG");
    }
    static bool is_consts_and_skip(const char*& c)
    {
        return CSTRCMP_SKIP(c,"CONSTS");
    }
    static bool is_start_and_skip(const char*& c)
    {
        return CSTRCMP_SKIP(c,"START");
    }
    static bool is_end_and_skip(const char*& c)
    {
        return CSTRCMP_SKIP(c,"END");
    }
    static bool parsing_uint_number(const char*& c,size_t& i)
    {
        std::string number;
        
        while(is_uint_number(*c)){ number+=(*c); ++c; }
        
        if(number.size())
        {
            std::istringstream ( number ) >> i ;
            return true;
        }
        
        return false;
    }
    static bool parsing_int_number(const char*& c,int& i)
    {
        std::string number;
        
        //minus
        if((*c)=='-')
        {
            number+=(*c);
            ++c;
            if(!is_uint_number(*c)) return false;
        }
        //this part is equals of uint
        while(is_uint_number(*c)){ number+=(*c); ++c; }
        
        if(number.size())
        {
            std::istringstream ( number ) >> i ;
            return true;
        }
        
        return false;
    }
    bool parse_cstring(const char* in,const char** cout,std::string& out)
    {
        const char *tmp=in;
        out="";
        //if(jumpSpace(in,&tmp)==FIND_GOOD){//[  "...."]
        if((*tmp)=='\"')  //["...."]
        {
            ++tmp;  //[...."]
            while((*tmp)!='\"'&&(*tmp)!='\n')
            {
                if((*tmp)=='\\') //[\.]
                {
                    ++tmp;  //[.]
                    switch(*tmp)
                    {
                        case 'n':
                            out+='\n';
                            break;
                        case 't':
                            out+='\t';
                            break;
                        case 'b':
                            out+='\b';
                            break;
                        case 'r':
                            out+='\r';
                            break;
                        case 'f':
                            out+='\f';
                            break;
                        case 'a':
                            out+='\a';
                            break;
                        case '\\':
                            out+='\\';
                            break;
                        case '?':
                            out+='\?';
                            break;
                        case '\'':
                            out+='\'';
                            break;
                        case '\"':
                            out+='\"';
                            break;
                        case '\n': /* jump unix */
                            ++m_line;
                            break;
                        case '\r': /* jump mac */
                            ++m_line;
                            if((*(tmp+1))=='\n') ++tmp; /* jump window (\r\n)*/
                            break;
                        default:
                            return true;
                            break;
                    }
                }
                else
                {
                    if((*tmp)!='\0') out+=(*tmp);
                    else return false;
                }
                ++tmp;//next char
            }
            if((*tmp)=='\n') return false;
            if(cout) (*cout)=tmp+1;
            return true;
        }
        //}
        return false;
    }
    bool parse_name(const char* in,const char** cout,std::string& out)
    {
        if (!is_start_name(*in)) return false;
        out+=*in;
        ++in;
        while(is_char_name(*in))
        {
            out+=*in;
            ++in;
        }
        (*cout)=in;
        return true;
    }

    //////////////////////////////////////////////////////
    void push_error(const std::string& error)
    {
        m_errors.push_front(error);
        m_errors_line.push_front(m_line);
    }
    
    bool config(const char*& ptr)
    {
        //skeep
        skeep_space_end_comment(ptr);
        //
        if(is_start_table(*ptr))
        {
            //skeep '{'
            ++ptr;
            
            while (!is_end_table(*ptr) && *ptr != EOF && *ptr != '\0')
            {
                //skeep
                skeep_space_end_comment(ptr);
                //select command
                if( CSTRCMP_SKIP(ptr, "robot") )
                {
                    //skeep "line" space
                    skeep_line_space(ptr);
                    //get number
                    if(!parsing_uint_number(ptr,m_config_file->m_roobot_init))
                    {
                        push_error("Invalid 'robot' parameter");
                        return false;
                    }
                }
                else if( CSTRCMP_SKIP(ptr, "doors") )
                {
                    //skeep "line" space
                    skeep_line_space(ptr);
                    //is equals or not equals
                    if( CSTRCMP_SKIP(ptr, "equals") )
                        m_config_file->m_doors_equals = true;
                    else
                    {
                        //else not equals
                        bool no_equals= CSTRCMP_SKIP(ptr, "not");
                        skeep_line_space(ptr);
                        no_equals = no_equals && CSTRCMP_SKIP(ptr, "equals");
                        if(no_equals)
                            m_config_file->m_doors_equals = false;
                        //else push error
                        else
                        {
                            push_error("Invalid 'doors' parameter");
                            return false;
                        }
                    }
                }
                else if( CSTRCMP_SKIP(ptr, "dfs") )
                {
                    //skeep "line" space
                    skeep_line_space(ptr);
                    //parser a cstring
                    if(!parse_cstring(ptr,&ptr,m_config_file->m_dfs.m_file))
                    {
                        push_error("Invalid 'dfs' parameter");
                        return false;
                    }
                    m_config_file->m_dfs.m_enable=true;
                }
                else if( CSTRCMP_SKIP(ptr, "bfs") )
                {
                    //skeep "line" space
                    skeep_line_space(ptr);
                    //parser a cstring
                    if(!parse_cstring(ptr,&ptr,m_config_file->m_bfs.m_file))
                    {
                        push_error("Invalid 'bfs' parameter");
                        return false;
                    }
                    m_config_file->m_bfs.m_enable=true;

                }
                else if( CSTRCMP_SKIP(ptr, "uc") )
                {
                    //skeep "line" space
                    skeep_line_space(ptr);
                    //parser a cstring
                    if(!parse_cstring(ptr,&ptr,m_config_file->m_uc.m_file))
                    {
                        push_error("Invalid 'uc' parameter");
                        return false;
                    }
                    m_config_file->m_uc.m_enable=true;
                }
                else if( CSTRCMP_SKIP(ptr, "iddfs") )
                {
                    //skeep "line" space
                    skeep_line_space(ptr);
                    //parser a cstring
                    if(!parse_cstring(ptr,&ptr,m_config_file->m_iddfs.m_file))
                    {
                        push_error("Invalid 'iddfs' parameter");
                        return false;
                    }
                    m_config_file->m_iddfs.m_enable=true;
                }
            }//end while
            if(!is_end_table(*ptr))
            {
                push_error("Not found }");
                return false;
            }
            //skip }
            ++ptr;
            //return
            return true;
        }
        
        push_error("Not found {");
        return false;
    }
    
    bool consts(const char*& ptr)
    {
        //skeep
        skeep_space_end_comment(ptr);
        //
        if(is_start_table(*ptr))
        {
            //skeep '{'
            ++ptr;
            //index consts array
            size_t index=0;
            //skeep
            skeep_space_end_comment(ptr);
            //
            while (!is_end_table(*ptr) && *ptr != EOF && *ptr != '\0')
            {
                //get number
                if(!parsing_int_number(ptr,m_config_file->m_costs[index]))
                {
                    push_error("Invalid parameter");
                    return false;
                }
                //max
                if(m_config_file->m_costs.size() == index)
                {
                    push_error("Number of parameters is not valid");
                    return false;
                }
                //next
                ++index;
                //skeep
                skeep_space_end_comment(ptr);
            }//end while
            //'}' ?
            if(!is_end_table(*ptr))
            {
                push_error("Not found }");
                return false;
            }
            //skip }
            ++ptr;
            //return
            return true;
        }
        
        return false;
    }
    
    bool parsing_a_object(const char*& ptr,std::vector< Object::ptr >& objects)
    {
        //skeep "line" space
        skeep_line_space(ptr);
        //class name
        std::string classname;
        std::string arg;
        //get class name
        if(!parse_name(ptr,&ptr,classname)) { push_error("Class's name is not valid"); return false; }
        //skeep space
        skeep_line_space(ptr);
        //'('
        if(!is_start_arg(*ptr)) { push_error("Not found ("); return false; }
        ++ptr;
        //skeep space
        skeep_line_space(ptr);
        //get arg
        if(!parse_cstring(ptr,&ptr,arg)) { push_error("Argument is not valid"); return false; }
        //')'
        if(!is_end_arg(*ptr)) { push_error("Not found )"); return false; }
        ++ptr;
        //create object
        Object::ptr obj = ObjectsMap::create(classname,arg);
        //not exist this calss?
        if(!obj) { push_error("Class is not valid"); return false; }
        //add object
        objects.push_back(obj);
        return true;
    }
    
    bool parsing_a_room(const char*& ptr,DynamicHouse& h)
    {
        //skeep
        skeep_space_end_comment(ptr);
        //init buffers
        bool                       door = true;
        std::vector< Object::ptr > objects;
        //open or close
        if NOT( CSTRCMP(ptr, "open") || CSTRCMP(ptr, "close") )
        {
            push_error("Room declaration must to be started with 'open' or 'close'");
            return false;
        }
             if( CSTRCMP_SKIP(ptr,"open") )  door = true;
        else if( CSTRCMP_SKIP(ptr,"close") ) door = false;
        //skeep "line" space
        skeep_line_space(ptr);
        // '|'
        if(!is_start_obj_list(*ptr)) { push_error("Not found |"); return false; }
        //skeep '|'
        ++ptr;
        //skeep "line" space
        skeep_line_space(ptr);
        //get all objects
        while(*ptr != '\n' && *ptr != '\r' && *ptr != EOF && *ptr != '\0')
        {
            if(!parsing_a_object(ptr,objects))
            {
                push_error("Invalid object");
                return false;
            }
            //skeep "line" space
            skeep_line_space(ptr);
        }
        //add rooom and door
        h.push(door,objects);
        return true;
    }
    
    bool house(const char*& ptr, DynamicHouse& h)
    {
        //skeep
        skeep_space_end_comment(ptr);
        //
        if(is_start_table(*ptr))
        {
            //skeep '{'
            ++ptr;
            //skeep
            skeep_space_end_comment(ptr);
            //get all
            while (!is_end_table(*ptr) && *ptr != EOF && *ptr != '\0')
            {
                //parsing a room
                if(!parsing_a_room(ptr,h))
                {
                    push_error("Invalid room");
                    return false;
                }
                //skeep
                skeep_space_end_comment(ptr);
            } //end while
            //'}' ?
            if(!is_end_table(*ptr))
            {
                push_error("Not found }");
                return false;
            }
            //skip }
            ++ptr;
            //return
            return true;
        }
        
        push_error("Not found {");
        return false;
    }
    
    
    bool parser(const char*& ptr)
    {
        //skeep line and comments
        skeep_space_end_comment(ptr);
        //get type
        while(*ptr != EOF && *ptr != '\0')
        {
            //parsing a block
                 if(is_config_and_skip(ptr)){ if(!config(ptr)) return false; }
            else if(is_consts_and_skip(ptr)){ if(!consts(ptr)) return false; }
            else if(is_start_and_skip(ptr)) { if(!house(ptr,m_config_file->m_start)) return false; }
            else if(is_end_and_skip(ptr))   { if(!house(ptr,m_config_file->m_end)) return false; }
            else { push_error("Not found a valid command"); return false; }
            //skeep line and comments
            skeep_space_end_comment(ptr);
        }
        return true;
    }
    
public:
    
    bool parsing_a_string(const std::string& str, ConfigFile& config)
    {
        //set ptr to struct
        m_config_file   = & (config);
        //ptr
        const char* cstr= & (str[0]);
        //start parsing
        return parser(cstr);
    }
    
    std::string errors_to_string() const
    {
        std::stringstream sbuffer;
        
        std::list< size_t >::const_iterator       line_it = m_errors_line.begin();
        std::list< std::string >::const_iterator  str_it  = m_errors.begin();
        
        while( line_it != m_errors_line.end()  && str_it != m_errors.end() )
        {
            sbuffer << "Error: " << ((*line_it)+1) << " : " << *str_it << "\n";
            ++line_it;
            ++str_it;
        }
        
        return sbuffer.str();
    }
    
};

};

#endif

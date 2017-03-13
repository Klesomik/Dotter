#ifndef Digraph_hpp

#define Digraph_hpp

#include <cstdio>
#include <iostream>
#include <cstring>
#include <cstdarg>
#include "..//FormatBuffer.hpp"
//#include "..//Tools.hpp"

//TODO: Many setters and no getters, add ios::app with Boost streams
//TODO: we can't do comment with '\n'

void System (const char* cmd, ...);

void System (const char* cmd, ...)
{
    const size_t size = 1000;

    char result[size] = "";

    va_list args;

    va_start (args, cmd);

    vsprintf (result, cmd, args);

    va_end (args);

    system (result);
}

namespace Dotter
{
    class Digraph
    {
        public:
            Digraph ();

            ~Digraph ();

            void open  ();
            void close ();

            void begin ();
            void end   ();

            void set (const std::string& comand, const std::string& param);

            void node (const size_t number, const char* label, ...);
            void link (const size_t from, const size_t to, const char* label, ...);

            void comment (const char* label, ...);

            void render (const bool show = true);

            FormatBuffer& file ();
            std::string& dotter ();
            std::string& text ();
            std::string& photo ();
            size_t& cluster ();
            bool& oriented ();

        private:
            FormatBuffer file_;

            std::string dotter_, // Path to your directory with dotter
                        text_,   // File with information about graph
                        photo_;  // Photo which was built from 'text'

            size_t cluster_;

            bool oriented_;
    };
}

Dotter::Digraph::Digraph ():
    file_    (),
    dotter_  (),
    text_    (),
    photo_   (),
    cluster_ (0),
    oriented_(true)
{
}

Dotter::Digraph::~Digraph ()
{
    close ();
}

void Dotter::Digraph::open ()
{
    file_.print ("digraph Hello\n");
    file_.print ("{\n");
    file_.increase ();

    begin ();
}

void Dotter::Digraph::close ()
{
    end ();
    end ();

    comment (" Build with Dotter ");

    file_.to_file (text_);
}

void Dotter::Digraph::begin ()
{
    file_.print ("subgraph Cluster%u\n", cluster_++);
    file_.print ("{\n");

    file_.increase ();
}

void Dotter::Digraph::end ()
{
    file_.decrease ();

    file_.print ("}\n");
}

void Dotter::Digraph::set (const std::string& comand, const std::string& param)
{
    file_.print ("node [%s=\"%s\"];\n", comand.c_str (), param.c_str ());
}

void Dotter::Digraph::node (const size_t number, const char* label, ...)
{
    file_.print ("Node%u [label=\"", number);

    char buffer[512] = "";

    va_list args;

    va_start (args, label);

    vsprintf (buffer, label, args);

    va_end (args);

    file_.print ("%s\"];\n", buffer);
}

void Dotter::Digraph::link (const size_t from, const size_t to, const char* label, ...)
{
    file_.print ("Node%u %s Node%u [label=\"", from, (oriented_? "->" : "--"), to);

    char buffer[512] = "";

    va_list args;

    va_start (args, label);

    vsprintf (buffer, label, args);

    va_end (args);

    file_.print ("%s\"];\n", buffer);
}

void Dotter::Digraph::comment (const char* label, ...)
{
    file_.print ("/*");

    char buffer[512] = "";

    va_list args;

    va_start (args, label);

    vsprintf (buffer, label, args);

    va_end (args);

    file_.print ("%s*/", buffer);
}

void Dotter::Digraph::render (const bool show /* = true */)
{
    close ();

    std::string type;
    Dotter::Details::Expansion (photo_, type);

    System ("%s -T%s %s -o %s", dotter_.c_str (), type.c_str (), text_.c_str (), photo_.c_str ());

    if (show)
        System ("start %s", photo_.c_str ());
}

FormatBuffer& Dotter::Digraph::file ()
{
    return file_;
}

std::string& Dotter::Digraph::dotter ()
{
    return dotter_;
}

std::string& Dotter::Digraph::text ()
{
    return text_;
}

std::string& Dotter::Digraph::photo ()
{
    return photo_;
}

size_t& Dotter::Digraph::cluster ()
{
    return cluster_;
}

bool& Dotter::Digraph::oriented ()
{
    return oriented_;
}

#endif /* Digraph_hpp */

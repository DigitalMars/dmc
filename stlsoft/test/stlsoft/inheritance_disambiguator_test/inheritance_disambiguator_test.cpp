/* /////////////////////////////////////////////////////////////////////////////
 * File:        inheritance_disambiguator_test.cpp
 *
 * Purpose:
 *
 * Created:
 * Updated:     22nd April 2004
 *
 * Author:      Matthew Wilson
 *
 * Copyright:   Synesis Software Pty Ltd, (c) 2002. All rights reserved.
 *
 * Home:        www.stlsoft.org
 *
 * ////////////////////////////////////////////////////////////////////////// */


// This will cause various compile-time messages to be emitted. When you get
// sick of them just comment out or remove this #define
#define	_STLSOFT_COMPILE_VERBOSE

#include <stlsoft.h>
#include <stlsoft_inheritance_disambiguator.h>

stlsoft_ns_using(inheritance_disambiguator)

////////////////////////////////////////////////////////////////////////////////
// Typedefs

class Vertex
{
public:
    Vertex()
    {
        // No operation, for efficiency
    }

// Members
protected:
    int x;
    int y;
    int z;
};

template <class T>
struct node_link
{
    T   *previous;
    T   *next;

    node_link(T *p = NULL, T *n = NULL)
        : previous(p)
        , next(n)
    {}
};

#if 1

// Uses partial-recursive inheritance via specific class
class VertexNode
    : protected node_link<VertexNode>
    , public Vertex
{
};

#else

// Uses partial-recursive inheritance via template
template <ss_typename_param_k T>
class node
    : protected node_link<node<T> >
    , public T
{
};

typedef node<Vertex>    VertexNode;

#endif /* 0 */


class VertexNode2
	: public Vertex
    , protected inheritance_disambiguator<node_link<VertexNode2>, 0>
    , protected inheritance_disambiguator<node_link<VertexNode2>, 1>
{
public:
    typedef node_link<VertexNode2>									link_type;
    typedef inheritance_disambiguator<node_link<VertexNode2>, 0>	link_0_type;
    typedef inheritance_disambiguator<node_link<VertexNode2>, 1>	link_1_type;

    void MoveIntoList()
    {
#ifdef __BORLANDC__
        link_0_type *l_0    =   this;
        link_1_type *l_1    =   this;
#else
        link_0_type *l_0    =   static_cast<link_0_type*>(this);
        link_1_type *l_1    =   static_cast<link_1_type*>(this);
#endif /* __BORLANDC__ */
        link_type   *l0     =   l_0;
        link_type   *l1     =   l_1;

		STLSOFT_SUPPRESS_UNUSED(l0);
		STLSOFT_SUPPRESS_UNUSED(l1);

// Enable this to demonstrate how the two types are not inter-convertible
//      l_0 = l_1;
    }
};

////////////////////////////////////////////////////////////////////////////////
// main

int main(int /* argc */, char* /* argv */[])
{
    VertexNode  vn;
    VertexNode2 vn2;

    stlsoft_static_assert(sizeof(VertexNode) == sizeof(Vertex) + sizeof(node_link<Vertex>));
    stlsoft_static_assert(sizeof(VertexNode2) == sizeof(Vertex) + 2 * sizeof(node_link<Vertex>));

    vn2.MoveIntoList();

    return 0;
}

////////////////////////////////////////////////////////////////////////////////

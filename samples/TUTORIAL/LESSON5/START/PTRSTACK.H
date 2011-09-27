/// ptrstack.h
//
//   Part of TMLRead, Symantec C++ 7.0 demonstration program
//   Copyright (C) 1994 Symantec Corporation
//
//   last mod:
//     rjw, 11/94 (release)
//
// Contents of this file:
//   CPtrStack class declaration
// Notes:
//   This is a general purpose pointer stack.  One is used by the document
//     to keep a list of files we have visited, so that the 'Previous File'
//     menu command can jump back to previous files.

#ifndef _PTRSTACK_H
#define _PTRSTACK_H

//// CPtrStack
////  General purpose class for a stack of pointers

class CPtrStack {
  public:
    CPtrStack (WORD size=50) : apStack(size) {}
    ~CPtrStack() {}

    // Push() pushes a pointer onto the stack
    void Push(void *ptr) { apStack.AddHead(ptr); }
    // Pop() gets the top pointer off the stack, returns NULL if empty
    void * Pop() { return (IsEmpty() ? NULL : apStack.RemoveHead()); }
    // IsEmpty() returns TRUE if stack is empty
    BOOL IsEmpty() { return apStack.IsEmpty(); }

  private:
    CPtrList apStack;  // list object used as stack
};

#endif
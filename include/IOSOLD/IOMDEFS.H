#ifndef __IOMDEFS_H
#define __IOMDEFS_H

#include <iosold\iostream.h>

#pragma pack(__DEFALIGN)
template <class T> class SMANIP {
public:
    SMANIP(ios &(*f)(ios&, T), T v) : func(f), val(v) {}
    friend istream &operator>>(istream& s, SMANIP<T>& m)
	{ m.func(s, m.val); return s; }
    friend ostream &operator<<(ostream& s, SMANIP<T>& m)
	{ m.func(s, m.val); return s; }
private:
    ios &(*func)(ios&, T);
    T val;
};

template <class T> class IMANIP {
public:
    IMANIP(istream &(*f)(istream&, T), T v) : func(f), val(v) {}
    friend istream &operator>>(istream& s, IMANIP<T>& m)
	{ return m.func(s, m.val); }
private:
    istream &(*func)(istream&, T);
    T val;
};

template <class T> class OMANIP {
public:
    OMANIP(ostream &(*f)(ostream&, T), T v) : func(f), val(v) {}
    friend ostream &operator<<(ostream& s, OMANIP<T>& m)
	{ return m.func(s, m.val); }
private:
    ostream &(*func)(ostream&, T);
    T val;
};

template <class T> class IOMANIP {
public:
    IOMANIP(iostream &(*f)(iostream&, T), T v) : func(f), val(v) {}
    friend istream &operator>>(iostream& s, IOMANIP<T>& m)
	{ return m.func(s, m.val); }
    friend ostream &operator<<(iostream& s, IOMANIP<T>& m)
	{ return m.func(s, m.val); }
private:
    iostream &(*func)(iostream&, T);
    T val;
};

template <class T> class SAPP {
public:
    SAPP(ios &(*f)(ios&, T)) : func(f) {}
    SMANIP<T> operator()(T v) { return SMANIP<T>(func, v); }
private:
    ios &(*func)(ios&, T);
};

template <class T> class IAPP {
public:
    IAPP(istream &(*f)(istream&, T)) : func(f) {}
    IMANIP<T> operator()(T v) { return IMANIP<T>(func, v); }
private:
    istream &(*func)(istream&, T);
};

template <class T> class OAPP {
public:
    OAPP(ostream &(*f)(ostream&, T)) : func(f) {}
    OMANIP<T> operator()(T v) { return OMANIP<T>(func, v); }
private:
    ostream &(*func)(ostream&, T);
};

template <class T> class IOAPP {
public:
    IOAPP(iostream &(*f)(iostream&, T)) : func(f) {}
    IOMANIP<T> operator()(T v) { return IOMANIP<T>(func, v); }
private:
    iostream &(*func)(iostream&, T);
};

#pragma pack()

#endif  // __IOMDEFS_H

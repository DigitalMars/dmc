/*  fstream.h -- class filebuf and fstream declarations

    Copyright 1991, 1993 by TauMetric Corporation      ALL RIGHTS RESERVED

    @(#)fstream.h	1.4  5/28/93 11:52:41
*/
#ifndef _FSTREAM_H_
#define _FSTREAM_H_

#ifndef __cplusplus
#error  Use C++ compiler for fstream.h
#endif

#include <iostream.h>

#pragma pack(__DEFALIGN)
class  filebuf : public streambuf {
public:
static const int openprot;	// default file protection
static const int sh_compat;
static const int sh_none;
static const int sh_read;
static const int sh_write;

// options for setmode member function
static  const int   binary;
static  const int   text;

		// constructors, destructor
		filebuf();	// make a closed filebuf
		filebuf(int);	// make a filebuf attached to fd
		filebuf(int, char*, int); // same, with specified buffer
		~filebuf();

	int	is_open();	// is the file open
	int	fd();		// what is the file descriptor

	// open named file with mode and protection, attach to this filebuf
	filebuf* open(const char*, int, int = filebuf::openprot);

	filebuf* close();	// flush and close file
	int setmode(int = filebuf::text);
	filebuf* attach(int);	// attach this filebuf to opened file descriptor
	int	 detach();	// detach from and return the file descriptor

/*
 * These perform the streambuf functions on a filebuf
 * Get and Put pointers are kept together
 */
virtual int	overflow(int = EOF);
virtual int	underflow();
virtual int	sync();
virtual streampos  seekoff(streamoff, ios::seek_dir, int);
virtual streambuf* setbuf(char*, int);

protected:
	int	xfd;		// the file descriptor, EOF if closed
	int	mode;		// the opened mode
	short	opened;		// non-zero if file is open

	streampos last_seek;	// unused			***
	char*   in_start;	// unused			***

	int	last_op();	// unused			***
	char	lahead[2];	// current input char if unbuffered ***
};
/*
 * The data members marked with *** above are not documented in the AT&T
 * release of streams, so we cannot guarantee compatibility with any
 * other streams release in the use or values of these data members.
 * If you can document any expected behavior of these data members, we
 * will try to adjust our implementation accordingly.
 */
inline int	filebuf::is_open()	{ return opened; }
inline int	filebuf::fd()		{ return xfd; }


class fstreambase : virtual public ios {
public:
		fstreambase();
		fstreambase(const char*, int, int = filebuf::openprot);
		fstreambase(int);
		fstreambase(int _f, char*, int);
		~fstreambase();

	void	open(const char*, int, int = filebuf::openprot);
	void	attach(int);
	void	close();
	void	setbuf(char*, int);
	filebuf* rdbuf();
	int setmode(int mode = filebuf::text) {return rdbuf()->setmode(mode);}

protected:
	void	verify(int);	// unimplemented	***

private:
	filebuf	buf;
};
/*
 * The function member marked with *** above is not documented in the AT&T
 * release of streams, so we cannot guarantee compatibility with any
 * other streams release in its use.
 * If you can document any expected behavior of this function member, we
 * will try to adjust our implementation accordingly.
 */
inline filebuf* fstreambase::rdbuf() { return &buf; }


class ifstream : public fstreambase, public istream {
public:
		ifstream();
		ifstream(const char*, int=ios::in, int=filebuf::openprot);
		ifstream(int);
		ifstream(int, char*, int);
		~ifstream();

	filebuf* rdbuf();
	
	int is_open(){ return rdbuf()->is_open(); }
	void	open(const char*, int=ios::in, int=filebuf::openprot);
    void close();
};
inline filebuf* ifstream::rdbuf() { return fstreambase::rdbuf(); }
inline void	ifstream::open(const char* _n, int _m, int _p)
	{ fstreambase::open(_n, _m | ios::in, _p); }
inline void ifstream::close()
   { fstreambase::close(); }



class ofstream : public fstreambase, public ostream {
public:
		ofstream();
		ofstream(const char*, int=ios::out, int=filebuf::openprot);
		ofstream(int);
		ofstream(int _f, char*, int);
		~ofstream();

	filebuf* rdbuf();
	
	int is_open(){ return rdbuf()->is_open(); }
	void open(const char*, int=ios::out, int=filebuf::openprot);
    void close();
};
inline filebuf*	ofstream::rdbuf() { return fstreambase::rdbuf(); }
inline void	ofstream::open(const char* name, int m, int prot)
	{ fstreambase::open(name, m | ios::out, prot); }
inline void ofstream::close()
  { fstreambase::close(); }



class fstream : public fstreambase, public iostream {
public:
		fstream();
		fstream(const char*, int, int = filebuf::openprot);
		fstream(int);
		fstream(int _f, char*, int);
		~fstream();

	filebuf* rdbuf();
	
	int is_open(){ return rdbuf()->is_open(); }
	void	open(const char *, int, int = filebuf::openprot);
   void close();
};
inline filebuf* fstream::rdbuf() { return fstreambase::rdbuf(); }
inline void	fstream::open(const char* name, int m, int prot)
	{ fstreambase::open(name, m, prot); }
inline void fstream::close()
  { fstreambase::close(); }

inline ios& binary(ios& st) {((filebuf*)st.rdbuf())->setmode(filebuf::binary); return st;}
inline ios& text(ios& st) {((filebuf*)st.rdbuf())->setmode(filebuf::text); return st;}

#pragma pack()

#endif /* _FSTREAM_H_ */

// Copyright (c) 2010-2025, Numerics88 Solutions.
// See LICENSE for details.

#ifndef __pQCTIO_Exception_h
#define __pQCTIO_Exception_h

#include <boost/format.hpp>
#include <string>
#include <exception>

#include "pqctio_export.h"

/** Throws pQCTIOException and sets the file name and line number. */
#if __cplusplus >= 201703L
#define throw_pqctio_exception(x)                                          \
  if(std::uncaught_exceptions() == 0)                                      \
    throw pQCTIO::pQCTIOException (x, __FILE__, __LINE__)
#else
#define throw_pqctio_exception(x)                                          \
  if(!std::uncaught_exception())                                           \
    throw pQCTIO::pQCTIOException (x, __FILE__, __LINE__)
#endif

/** If the argument is false, throws pQCTIOException and sets the file name and line number.
  *
  * Note that this check occurs even in release code.
  */
#define pqctio_assert(x)                                                   \
  if (!(x))                                                               \
    throw pQCTIO::pQCTIOException ("Assertion failure", __FILE__, __LINE__)

/** If the argument is true, throws pQCTIOException and sets the file name and line number.
  *
  * Note that this check occurs even in release code.
  */
#define pqctio_negative_assert(x)                                          \
  if (x)                                                                  \
    throw pQCTIO::pQCTIOException ("Assertion failure", __FILE__, __LINE__)

/** If the argument is false, throws pQCTIOException and sets the file name and line number.
  * Also sets an informative message.
  *
  * Note that this check occurs even in release code.
  */
#define pqctio_verbose_assert(x, msg)                                    \
  if (!(x))                                                             \
    throw pQCTIO::pQCTIOException (                                         \
      std::string("Assertion failure : ") + msg, __FILE__, __LINE__);


namespace pQCTIO
{

/** An exception class for pQCT file. */
class PQCTIO_EXPORT pQCTIOException : public std::exception
{
  public:

    /** Constructor.
      *
      * @param what  Description of the exception.
      */
    explicit pQCTIOException (const std::string& what) throw()
      :
      m_what(what),
      m_file("Unknown"),
      m_line(0)
      {}

    /** Constructor.
      *
      * @param what  Description of the exception.
      */
    explicit pQCTIOException (const boost::format& what) throw()
      :
      m_what(what.str()),
      m_file("Unknown"),
      m_line(0)
      {}

    /** Constructor.
      *
      * @param what  Description of the exception.
      * @param file  The source code file where this occurred.
      * @param file  The line number where this occurred.
      */
    explicit pQCTIOException
      (
      const std::string& what,
      const std::string& file,
      int line
      ) throw()
      :
      m_what(what),
      m_file(file),
      m_line(line)
      {}

    /** Constructor.
      *
      * @param what  Description of the exception.
      * @param file  The source code file where this occurred.
      * @param file  The line number where this occurred.
      */
    explicit pQCTIOException
      (
      const boost::format& what,
      const std::string& file,
      int line
      ) throw()
      :
      m_what(what.str()),
      m_file(file),
      m_line(line)
      {}

    virtual ~pQCTIOException() throw()
      {}

    /** Returns the description of the exception. */
    virtual const char* what() const throw()
      { return m_what.c_str(); }

   /** Returns source code file where the exception occurred. */
    virtual const char* file() const throw()
      { return m_file.c_str(); }

   /** Returns line number where the exception occurred. */
    virtual int line() const throw()
      { return m_line; }

  protected:
    std::string m_what;
    std::string m_file;
    int m_line;

}; /* end class pQCTIOException */

}  /* end namespace pQCTIO */

#endif /* __pQCTIO_Exception_h */

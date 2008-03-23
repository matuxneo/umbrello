/* This file is part of KDevelop
    Copyright (C) 2002,2003 Roberto Raggi <roberto@kdevelop.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef LEXER_H
#define LEXER_H

#include "driver.h"
#include "position.h"

#include <list>
#include <qstring.h>
#include <qmap.h>
#include <q3valuestack.h>
#include <qpair.h>
#include <q3ptrvector.h>
//Added by qt3to4:
#include <Q3MemArray>

#include <assert.h>

typedef boost::spirit::position_iterator<QChar const*> CharIterator;

enum SkipType {
  SkipWord,
  SkipWordAndArguments
};

struct LexerData;

enum Type {
    Token_eof = 0,
    Token_identifier = 1000,
    Token_number_literal,
    Token_char_literal,
    Token_string_literal,
    Token_whitespaces,
    Token_comment,
    Token_preproc,

    Token_assign = 2000,
    Token_ptrmem,
    Token_ellipsis,
    Token_scope,
    Token_shift,
    Token_eq,
    Token_leq,
    Token_geq,
    Token_incr,
    Token_decr,
    Token_arrow,

    Token_concat,

    Token_K_DCOP,
    Token_k_dcop,
    Token_k_dcop_signals,

    Token_Q_OBJECT,
    Token_signals,
    Token_slots,
    Token_emit,

    Token_and,
    Token_and_eq,
    Token_asm,
    Token_auto,
    Token_bitand,
    Token_bitor,
    Token_bool,
    Token_break,
    Token_case,
    Token_catch,
    Token_char,
    Token_class,
    Token_compl,
    Token_const,
    Token_const_cast,
    Token_continue,
    Token_default,
    Token_delete,
    Token_do,
    Token_double,
    Token_dynamic_cast,
    Token_else,
    Token_enum,
    Token_explicit,
    Token_export,
    Token_extern,
    Token_false,
    Token_float,
    Token_for,
    Token_friend,
    Token_goto,
    Token_if,
    Token_inline,
    Token_int,
    Token_long,
    Token_mutable,
    Token_namespace,
    Token_new,
    Token_not,
    Token_not_eq,
    Token_operator,
    Token_or,
    Token_or_eq,
    Token_private,
    Token_protected,
    Token_public,
    Token_register,
    Token_reinterpret_cast,
    Token_return,
    Token_short,
    Token_signed,
    Token_sizeof,
    Token_static,
    Token_static_cast,
    Token_struct,
    Token_switch,
    Token_template,
    Token_this,
    Token_throw,
    Token_true,
    Token_try,
    Token_typedef,
    Token_typeid,
    Token_typename,
    Token_union,
    Token_unsigned,
    Token_using,
    Token_virtual,
    Token_void,
    Token_volatile,
    Token_wchar_t,
    Token_while,
    Token_xor,
    Token_xor_eq
};

class Token {
public:
  Token();
  Token( int type, CharIterator start, CharIterator end);

  operator int () const {return m_type;}

  bool isNull() const {return m_type == Token_eof || m_text.isEmpty();}

  int type() const {return m_type;}

  Position const& getStartPosition() const {return m_start;}
  Position const& getEndPosition() const {return m_end;}
  unsigned int length() const {return m_text.length();}

  QString const& text() const {return m_text;}
private:
  int m_type;
  Position m_start, m_end;
  QString m_text;
};

class Lexer
{
  typedef std::list<Token> TokenList;
public:
  typedef TokenList::const_iterator TokenIterator;

  Lexer( Driver* driver );
  ~Lexer();

  void addSkipWord( const QString& word, SkipType skipType = SkipWord,
		    const QString& str = QString() );
  QString source() const {return m_source.get_source();}
  void setSource( const QString& source );

  void setRecordComments( bool record );
  Position const& currentPosition() const
  {return m_source.get_currentPosition();}
  Token const& lookAhead( TokenIterator p_it, int n ) const {
    std::advance( p_it, n);
    assert( p_it != m_tokens.end());
    return *p_it;
  }
  Position const& getTokenPosition( const Token& token) const;
  TokenIterator tokenBegin() const {return m_tokens.begin();}

private:
  static int toInt( const Token& token );
  void tokenize();
  void nextToken( Token& token, bool stopOnNewline=false );
  void skip( int l, int r );
  void readLineComment();
  void readMultiLineComment();
  bool recordComments() const;
  void reset();

  // preprocessor (based on an article of Al Stevens on Dr.Dobb's journal)
  int testIfLevel();
  int macroDefined();
  QString readArgument();

  int macroPrimary();
  int macroMultiplyDivide();
  int macroAddSubtract();
  int macroRelational();
  int macroEquality();
  int macroBoolAnd();
  int macroBoolXor();
  int macroBoolOr();
  int macroLogicalAnd();
  int macroLogicalOr();
  int macroExpression();

  void handleDirective( const QString& directive );
  void processDefine( Macro& macro );
  void processElse();
  void processElif();
  void processEndif();
  void processIf();
  void processIfdef();
  void processIfndef();
  void processInclude();
  void processUndef();

private:
  LexerData* d;
  Driver* m_driver;
  TokenList m_tokens;
  class Source {
  public:
    Source() {}

    Token createToken( int type, CharIterator start, CharIterator end) const;
    Token createToken( int type, CharIterator start) const {
      return createToken( type, start, m_ptr);
    }
    QChar currentChar() const {
      return m_ptr != m_endPtr ? *m_ptr : QChar::null;
    }
    bool eof() const {return m_ptr >= m_endPtr;}
    int findOperator2() const {
      int n = length();
      if( n>=2 ){
	QChar ch = currentChar(), ch1 = peekChar();
	if( ch == ':' && ch1 == ':' ) return Token_scope;
	else if( ch == '.' && ch1 == '*' ) return Token_ptrmem;
	else if( ch == '+' && ch1 == '=' ) return Token_assign;
	else if( ch == '-' && ch1 == '=' ) return Token_assign;
	else if( ch == '*' && ch1 == '=' ) return Token_assign;
	else if( ch == '/' && ch1 == '=' ) return Token_assign;
	else if( ch == '%' && ch1 == '=' ) return Token_assign;
	else if( ch == '^' && ch1 == '=' ) return Token_assign;
	else if( ch == '&' && ch1 == '=' ) return Token_assign;
	else if( ch == '|' && ch1 == '=' ) return Token_assign;
	else if( ch == '<' && ch1 == '<' ) return Token_shift;
	else if( ch == '>' && ch1 == '>' ) return Token_shift;
	else if( ch == '=' && ch1 == '=' ) return Token_eq;
	else if( ch == '!' && ch1 == '=' ) return Token_eq;
	else if( ch == '<' && ch1 == '=' ) return Token_leq;
	else if( ch == '>' && ch1 == '=' ) return Token_geq;
	else if( ch == '&' && ch1 == '&' ) return Token_and;
	else if( ch == '|' && ch1 == '|' ) return Token_or;
	else if( ch == '+' && ch1 == '+' ) return Token_incr;
	else if( ch == '-' && ch1 == '-' ) return Token_decr;
	else if( ch == '-' && ch1 == '>' ) return Token_arrow;
	else if( ch == '#' && ch1 == '#' ) return Token_concat;
      }
      return -1;
    }
    int findOperator3() const;
    void insert( QString const& p) {
      int l_offset = &*m_ptr - m_source.data();
      m_source.insert( l_offset, p);
      m_ptr = CharIterator( m_source.data() + l_offset,
			    m_source.data() + m_source.length(),
			    m_ptr.get_position());
    }
    int length() const {return m_endPtr - m_ptr;}
    void nextChar() {
      QChar l_current = *m_ptr++;
      switch( l_current.toAscii()) {
      case '\n':
      case '\r':
	m_startLine = true;
	break;
      }
    }
    void nextChar( int n ) {
      std::advance( m_ptr, n);
    }
    QChar peekChar( int n = 1) const {
      CharIterator l_it = m_ptr;
      std::advance( l_it, n);
      return l_it != m_endPtr ? *l_it : QChar::null;
    }
    void readCharLiteral() {
      if( currentChar() == '\'' )
	nextChar(); // skip '
      else if( currentChar() == 'L' && peekChar() == '\'' )
	nextChar( 2 ); // slip L'
      else
	return;

      while( !currentChar().isNull() ){
	int len = length();

	if( len>=2 && (currentChar() == '\\' && peekChar() == '\'') ){
	  nextChar( 2 );
	} else if( len>=2 && (currentChar() == '\\' && peekChar() == '\\') ){
	  nextChar( 2 );
	} else if( currentChar() == '\'' ){
	  nextChar();
	  break;
	} else {
	  nextChar();
	}
      }
    }
    QString readIdentifier() {
      CharIterator start = m_ptr;
      while( currentChar().isLetterOrNumber() || currentChar() == '_' )
	nextChar();
      return substrFrom( start);
    }
    void readNumberLiteral() {
      while( currentChar().isLetterOrNumber() || currentChar() == '.' )
	nextChar();
    }
    void readStringLiteral() {
      if( currentChar() != '"' )
	return;
      nextChar(); // skip "
      while( !currentChar().isNull() ){
	int len = length();
	if( len>=2 && currentChar() == '\\' && peekChar() == '"' ){
	  nextChar( 2 );
	} else if( len>=2 && currentChar() == '\\' && peekChar() == '\\' ){
	  nextChar( 2 );
	} else if( currentChar() == '"' ){
	  nextChar();
	  break;
	} else {
	  nextChar();
	}
      }
    }
    void readWhiteSpaces( bool skipNewLine, bool p_inPreproc);
    void reset() {
      m_source.clear();
      m_ptr = CharIterator();
      m_startLine = false;
    }
    void set_source( const QString& source) {
      m_source = source;
      m_ptr = CharIterator( m_source.data(),
			    m_source.data() + m_source.length());
    }
    QString substrFrom( CharIterator start) const
    {return QString( &*start, &*m_ptr - &*start);}
    bool toLowerIsEqualTo( char const* p_pattern, int p_offset = 0) const {
      CharIterator l_it = m_ptr;
      std::advance( l_it, p_offset);
      int l_len = std::min( size_t( m_source.data() + m_source.length()
				    - &*l_it),
			    strlen( p_pattern));
      return (QString( &*l_it, l_len).toLower() == p_pattern);
    }
    /* getters */
    Position const& get_currentPosition() const {return m_ptr.get_position();}
    CharIterator get_ptr() const {return m_ptr;}
    QString const& get_source() const {return m_source;}
    bool get_startLine() const {return m_startLine;}
    /* setters */
    void set_startLine( bool p) {m_startLine = p;}
    void set_currentPosition( Position const& p) {m_ptr.set_position( p);}
  private:
    QString m_source;
    CharIterator m_ptr;
    const CharIterator m_endPtr;
    bool m_startLine;
  };
  Source m_source;
  bool m_recordComments;
  QMap< QString, QPair<SkipType, QString> > m_words;

  bool m_skipWordsEnabled;

  // preprocessor
  Q3MemArray<bool> m_skipping;
  Q3MemArray<bool> m_trueTest;
  int m_ifLevel;
  bool m_preprocessorEnabled;
  bool m_inPreproc;

  bool m_reportMessages;

private:
  Lexer( const Lexer& source );
  void operator = ( const Lexer& source );
};

inline bool Lexer::recordComments() const
{
    return m_recordComments;
}

inline void Lexer::setRecordComments( bool record )
{
    m_recordComments = record;
}

inline void Lexer::Source::readWhiteSpaces( bool skipNewLine, bool p_inPreproc)
{
  QChar ch;
  while( !(ch = currentChar()).isNull() ){
    if( (ch == '\n' || ch == '\r')
	&& !skipNewLine ){
            break;
        } else if( ch.isSpace() ){
            nextChar();
    } else if( p_inPreproc && currentChar() == '\\' ){
            nextChar();
      readWhiteSpaces( true, p_inPreproc);
        } else {
            break;
        }
    }
}

inline void Lexer::readLineComment()
{
  while( !m_source.currentChar().isNull()
	 && m_source.currentChar() != '\n'
	 && m_source.currentChar() != '\r') {
    if( m_reportMessages && m_source.currentChar() == '@'
	&& m_source.toLowerIsEqualTo( "todo", 1)) {
      m_source.nextChar( 5 );
      QString msg;
      Position l_position = currentPosition();

      while( !m_source.currentChar().isNull() ){
	if( m_source.currentChar() == '*' && m_source.peekChar() == '/' )
	  break;
	else if( m_source.currentChar() == '\n'
		 || m_source.currentChar() == '\r')
	  break;

	msg += m_source.currentChar();
	m_source.nextChar();
      }
      m_driver->addProblem( m_driver->currentFileName(),
			    Problem(msg, l_position, Problem::Level_Todo) );
    } else
      if( m_reportMessages && m_source.toLowerIsEqualTo( "fixme")) {
	m_source.nextChar( 5 );
	QString msg;
	Position l_position = currentPosition();

	while( !m_source.currentChar().isNull() ){
	  if( m_source.currentChar() == '*' && m_source.peekChar() == '/' )
	    break;
	  else if( m_source.currentChar() == '\n'
		   || m_source.currentChar() == '\r')
	    break;

	  msg += m_source.currentChar();
	  m_source.nextChar();
	}
	m_driver->addProblem( m_driver->currentFileName(),
			      Problem(msg, l_position, Problem::Level_Fixme) );
      } else
	m_source.nextChar();
  }
}

inline void Lexer::readMultiLineComment()
{
  while( !m_source.currentChar().isNull() ){
    if( m_source.currentChar() == '*' && m_source.peekChar() == '/' ){
      m_source.nextChar( 2 );
            return;
    } else if( m_reportMessages && m_source.currentChar() == '@'
	       && m_source.toLowerIsEqualTo( "todo", 1)) {
      m_source.nextChar( 5 );
	    QString msg;
      Position l_position = currentPosition();

      while( !m_source.currentChar().isNull() ){
	if( m_source.currentChar() == '*' && m_source.peekChar() == '/' )
		    break;
	else if( m_source.currentChar() == '\n'
		 || m_source.currentChar() == '\r')
	  break;
	msg += m_source.currentChar();
	m_source.nextChar();
	    }
      m_driver->addProblem( m_driver->currentFileName(),
			    Problem(msg, l_position, Problem::Level_Todo) );
	} else
      if( m_reportMessages && m_source.toLowerIsEqualTo( "fixme")) {
	m_source.nextChar( 5 );
            QString msg;
	Position l_position = currentPosition();

	while( !m_source.currentChar().isNull() ){
	  if( m_source.currentChar() == '*' && m_source.peekChar() == '/' )
                break;
	  else if( m_source.currentChar() == '\n'
		   || m_source.currentChar() == '\r')
	    break;

	  msg += m_source.currentChar();
	  m_source.nextChar();
            }
	m_driver->addProblem( m_driver->currentFileName(),
			      Problem(msg, l_position, Problem::Level_Fixme) );
        } else
	m_source.nextChar();
    }
}

#endif

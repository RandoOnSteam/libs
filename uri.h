/**
*	@file uri.h
*
*	#     # ######  ###
*	#     # #     #  #
*	#     # #     #  #
*	#     # ######   #
*	#     # #   #    #
*	#     # #    #   #
*	 #####  #     # ###
*
*	RFC 3986 compliant URI parser that can be compiled on legacy systems.
*
*	###NOTES###
*
*	URI is ridiculously efficient and doesn't do a single allocation except for
*	some combination algorithm functions. It also can't be changed.
*
*	MUTURI allocates 7 member strings, some return values, and in the more
*	complex combination algorithm functions.
*
*	@author Ryan Norton
*	@version 0.1
*	@copyright ALTERNATIVE A - 0BSD (www.opensource.org/license/0BSD)
*
*	Copyright (c) 2024 Ryan Norton
*
*	Permission to use, copy, modify, and/or distribute this software for
*	any purpose with or without fee is hereby granted.
*
*	THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
*	WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES
*	OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE
*	FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY
*	DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
*	AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
*	OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*
*	@copyright ALTERNATIVE B - Public Domain (www.unlicense.org)
*
*	This is free and unencumbered software released into the public domain.
*
*	Anyone is free to copy, modify, publish, use, compile, sell, or distribute
*	this software, either in source code form or as a compiled binary, for any
*	purpose, commercial or non-commercial, and by any means.
*
*	In jurisdictions that recognize copyright laws, the author or authors of
*	this software dedicate any and all copyright interest in the software to
*	the public domain. We make this dedication for the benefit of the public
*	at large and to the detriment of our heirs and successors. We intend this
*	dedication to be an overt act of relinquishment in perpetuity of all
*	present and future rights to this software under copyright law.
*
*	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*	AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
*	ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
*	CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/
#ifndef __URI_H__
#define __URI_H__

#if defined(__cplusplus)
extern "C" {
#endif

#ifndef WIKISERVER_VERSION
#include <stddef.h>
#if !defined(WSSTR_DEFINED)
	typedef struct WSSTR
	{
		char* mData;
		size_t mLength;
	} WSSTR;
	#define WSSTR_DEFINED
#endif
#if !defined(WSBOOL_DEFINED)
	typedef int wsbool;
	#if !defined(TRUE)
		#define TRUE 1
		#define FALSE 0
	#endif
	#define WSBOOL_DEFINED
#endif
#endif

/* Host Type that the server component can be */
/* Host is a normal register name (www.mysite.com etc.) */
#define	URI_REGNAME 0
/* Host is a version 4 ip address (192.168.1.100) */
#define	URI_IPV4ADDRESS 1
/* Host is a version 6 ip address [aa:aa:aa:aa::aa:aa]:5050 */
#define	URI_IPV6ADDRESS 2
/* Host is a future ip address (URI is unsure what kind) */
#define	URI_IPVFUTURE	3
typedef int URIHostType;

/* Component Flags, used as c++ strings can't be NULL */
#define	URI_SCHEME 1
#define	URI_USERINFO 2
#define	URI_HOST 4
#define	URI_PORT 8
#define	URI_PATH 16
#define	URI_QUERY 32
#define	URI_FRAGMENT 64
typedef int URIFieldType;

#define	URI_NONSTRICT 0		/* Enable backward compatability fallbacks */
#define	URI_STRICT 1		/* Follow latest standards */
typedef int URIStrictness;

/**
*	Generic class for parsing URIs. See RFC 3986.
*	Does not allocate in Create() or encode chars there, use MutUri for that.
*	default copy constructor, assignment operator, and destructor are fine.
*/
typedef struct URI
{
	WSSTR mScheme;
	WSSTR mPath;
	WSSTR mQuery;
	WSSTR mFragment;
	WSSTR mUserInfo;
	WSSTR mHost;
	WSSTR mPort;
	size_t mFields;
	URIHostType mHostType;
} URI;

wsbool URI_Create(URI* pThis, const char* urichars);
#define URI_HasScheme(pThis) (((pThis)->mFields & URI_SCHEME) == URI_SCHEME)
#define URI_HasUserInfo(pThis) \
	(((pThis)->mFields & URI_USERINFO) == URI_USERINFO)
#define URI_HasHost(pThis) (((pThis)->mFields & URI_HOST) == URI_HOST)
#define URI_HasPort(pThis) (((pThis)->mFields & URI_PORT) == URI_PORT)
#define URI_HasPath(pThis) (((pThis)->mFields & URI_PATH) == URI_PATH)
#define URI_HasQuery(pThis) (((pThis)->mFields & URI_QUERY) == URI_QUERY)
#define URI_HasFragment(pThis) \
	(((pThis)->mFields & URI_FRAGMENT) == URI_FRAGMENT)
#define URI_GetScheme(pThis) (pThis)->mScheme
#define URI_GetPath(pThis) (pThis)->mPath
#define URI_GetQuery(pThis) (pThis)->mQuery
#define URI_GetFragment(pThis) (pThis)->mFragment
#define URI_GetPort(pThis) (pThis)->mPort
#define URI_GetUserInfo(pThis) (pThis)->mUserInfo
#define URI_GetHost(pThis) (pThis)->mHost
#define URI_GetHostType(pThis) (pThis)->mHostType
void URI_GetUser(const URI* pThis, WSSTR* pOutUser);
void URI_GetPassword(const URI* pThis, WSSTR* pOutPassword);
#define URI_IsRelative(pThis) (!URI_HasScheme(pThis) || !URI_HasHost(pThis))
wsbool URI_IsValidHostname(const URI* pThis, URIStrictness strict);
wsbool URI_IsEqual(const URI* pThis, const URI* pOther);

/* Static methods for determining individual parts */
wsbool URIParseH16		(const char** uri);
wsbool URIParseIPv4address(const char** uri);
wsbool URIParseIPv6address(const char** uri);
wsbool URIParseIPvFuture	(const char** uri);
int URICharToHex(char c);
wsbool URIIsUnreserved (char c);
wsbool URIIsReserved (char c);
wsbool URIIsGenDelim (char c);
wsbool URIIsSubDelim (char c);
wsbool URIIsHex(char c);
wsbool URIIsAlpha(char c);
wsbool URIIsDigit(char c);
wsbool URIIsEndPath(char c);
wsbool URIIsPCharNCNE(char c);
wsbool URIIsPCharNE(char c);
void URIUpTree(const char* uristart, const char** uri);
void URINormalizeEncoded(char** s, char** cp);

/* Escape/Unescape (Unescape is whole string, Escape is single sequence) */
wsbool URIUnescape(const char* uri, size_t urilen, char** out, size_t* outlen);
char* URIEscape(char* s, const char** uri);

/* Specialized cases/C++ wrapping */
void URI_Construct(URI* pThis);
wsbool URI_IsCreated(URI* pThis);
void URI_Destroy(URI* pThis);

/* Recomposing */
typedef wsbool (*URIRECOMPOSEPROC) (const char* uri, size_t urilen,
	char** out, size_t* outlen);
wsbool URIDummyRecomposeProc(const char* uri, size_t urilen,
	char** out, size_t* outlen);
void URI_Recompose(const URI* pThis, URIRECOMPOSEPROC proc,
	char** out, size_t* outlen);
void URI_BuildURI(const URI* pThis, char** out, size_t* outlen);
void URI_BuildUnescapedURI(const URI* pThis, char** out, size_t* outlen);

#if defined(WIKISERVER_VERSION) && defined(WSTESTOPT)
WSTEST_SUITE_DECLARE(WSURI);
#endif

#if defined(__cplusplus)
} /* end extern "C" */
#ifndef WIKISERVER_VERSION
	#include <string>
	#define MutStr std::string
	#define StringReference(x) x // let the STL handle it
	#define string_transfercstring(str, x, y, z) str.assign(x, y)
	#define WSMAXLENGTH std::string::npos
#else
	#define string_transfercstring(str, x, y, z) str.Take(x, y, z)
#endif
#ifdef STRICT
	#undef STRICT
#endif
class Uri : public URI // Generic class for parsing Uris. See RFC 3986.
{ // Does not allocate in Create() or encode chars there, use MutUri for that.
public: // default copy constructor, assignment operator, and destructor are fine.
	typedef URIRECOMPOSEPROC RECOMPOSEPROC;
	enum HostType // Host Type that the server component can be
	{
		REGNAME,		// Host is a normal register name (www.mysite.com etc.)
		IPV4ADDRESS,	// Host is a version 4 ip address (192.168.1.100)
		IPV6ADDRESS,	// Host is a version 6 ip address [aa:aa:aa:aa::aa:aa]:5050
		IPVFUTURE		// Host is a future ip address (URI is unsure what kind)
	};
	enum FieldType // Component Flags, used as c++ strings can't be NULL
	{
		SCHEME = 1,
		USERINFO = 2,
		HOST = 4,
		PORT = 8,
		PATH = 16,
		QUERY = 32,
		FRAGMENT = 64
	};
	enum Strictness
	{
		NONSTRICT,		// Enable backward compatability fallbacks
		STRICT			// Follow latest standards
	};
	Uri() { URI_Construct(this); }
	Uri(const char* urichars) { URI_Create(this, urichars); }
	Uri(const MutStr& uristr) { URI_Create(this, uristr.c_str()); }
	bool Create(const char* urichars)
	{
		if(URI_IsCreated(this))
			URI_Destroy(this);
		return URI_Create(this, urichars) == TRUE;
	}
	bool HasScheme() const		{	return URI_HasScheme(this) == TRUE;		}
	bool HasUserInfo() const	{	return URI_HasUserInfo(this) == TRUE;	}
	bool HasHost() const		{	return (mFields & HOST) == HOST;		}
	bool HasPort() const		{	return (mFields & PORT) == PORT;		}
	bool HasPath() const		{	return (mFields & PATH) == PATH;		}
	bool HasQuery() const		{	return (mFields & QUERY) == QUERY;		}
	bool HasFragment() const	{	return (mFields & FRAGMENT) == FRAGMENT;}
	const WSSTR& GetScheme() const	{	return mScheme;		}
	const WSSTR& GetPath() const		{	return mPath;		}
	const WSSTR& GetQuery() const	{	return mQuery;		}
	const WSSTR& GetFragment() const	{	return mFragment;	}
	const WSSTR& GetPort() const		{	return mPort;		}
	const WSSTR& GetUserInfo() const {	return mUserInfo;	}
	const WSSTR& GetHost() const		{	return mHost;		}
	const URIHostType& GetHostType() const		{	return mHostType;	}
	WSSTR GetUser() const { WSSTR ret; URI_GetUser(this, &ret); return ret; }
	WSSTR GetPassword() const
	{ WSSTR ret; URI_GetPassword(this, &ret); return ret; }
	bool IsRelative() const { return !HasScheme() || !HasHost(); }
	bool IsValidHostname(Strictness strict = STRICT) const
	{ return URI_IsValidHostname(this, strict) == TRUE; }
	MutStr Recompose(RECOMPOSEPROC proc) const
	{
		MutStr ret; char* retc; size_t retclen, retcsize;
		URI_Recompose(this, proc, &retc, &retclen);
		retcsize = retclen + 1;
		string_transfercstring(ret, retc, retclen, retcsize);
		return StringReference(ret);
	}
	MutStr BuildUri()
	{ return StringReference(Recompose(URIDummyRecomposeProc)); }
	MutStr BuildUnescapedUri() const
	{ return StringReference(Recompose(URIUnescape)); }
	static MutStr Unescape (const MutStr& szEscapedUri)
	{
		MutStr ret; char* retc; size_t retclen, retcsize;
		retcsize = URIUnescape(szEscapedUri.c_str(), szEscapedUri.length(),
			&retc, &retclen);
		string_transfercstring(ret, retc, retclen, retcsize);
		return StringReference(ret);
	}
	Uri& operator = (const char* string) { Create(string); return *this; }
	bool operator == (const Uri& uri) const
	{ return URI_IsEqual(this, &uri) == TRUE; }
	static bool ParseH16		(const char*& uri)
	{ return URIParseH16(&uri) == TRUE; }
	static bool ParseIPv4address(const char*& uri)
	{ return URIParseIPv4address(&uri) == TRUE; }
	static bool ParseIPv6address(const char*& uri)
	{ return URIParseIPv6address(&uri) == TRUE; }
	static bool ParseIPvFuture	(const char*& uri)
	{ return URIParseIPvFuture(&uri) == TRUE; }
	static int CharToHex(char c) { return URICharToHex(c); }
	static bool IsUnreserved (char c) { return URIIsUnreserved(c) == TRUE; }
	static bool IsReserved (char c) { return URIIsReserved(c) == TRUE; }
	static bool IsGenDelim (char c) { return URIIsGenDelim(c) == TRUE; }
	static bool IsSubDelim (char c) { return URIIsSubDelim(c) == TRUE; }
	static bool IsHex(char c) { return URIIsHex(c) == TRUE; }
	static bool IsAlpha(char c) { return URIIsAlpha(c) == TRUE; }
	static bool IsDigit(char c) { return URIIsDigit(c) == TRUE; }
	static bool IsEndPath(char c) { return URIIsEndPath(c) == TRUE; }
	static bool IsPCharNCNE(char c) { return URIIsPCharNCNE(c) == TRUE; }
	static bool IsPCharNE(char c) { return URIIsPCharNE(c) == TRUE; }
	static void UpTree(const char* uristart, const char*& uri)
	{ URIUpTree(uristart, &uri); }
	static void Escape(MutStr& s, const char*& sPos)
	{
		size_t prevlen = s.length();
		s.resize(s.length() + 3);
		URIEscape((char*)&s.c_str()[prevlen], &sPos);
	}
	static void NormalizeEncoded(char*& s, char*& cp)
	{ URINormalizeEncoded(&s, &cp); }
};//end of Uri
typedef Uri::HostType UriHostType;
typedef Uri::FieldType UriFieldType;



#if defined(WSMUTURIOPT)
class MutUri // RFC 3986. Escapes characters according to the RFC unlike URI.
{ // default copy constructor, assignment operator, and destructor are fine.
public:
	typedef MutStr (*RECOMPOSEPROC) (const MutStr&);
	MutUri() : mFields(0), mHostType(Uri::REGNAME) { }
	MutUri(const MutStr& uri): mFields(0), mHostType(Uri::REGNAME)
	{ Create(uri); }
	bool Create(const MutStr& uri)
	{
		if (mFields)
			Destroy();
		return Parse(uri.c_str());
	}
	bool HasScheme() const		{	return (mFields & Uri::SCHEME) == Uri::SCHEME;		}
	bool HasUserInfo() const	{	return (mFields & Uri::USERINFO) == Uri::USERINFO;	}
	bool HasHost() const		{	return (mFields & Uri::HOST) == Uri::HOST;			}
	bool HasPort() const		{	return (mFields & Uri::PORT) == Uri::PORT;			}
	bool HasPath() const		{	return (mFields & Uri::PATH) == Uri::PATH;			}
	bool HasQuery() const		{	return (mFields & Uri::QUERY) == Uri::QUERY;		}
	bool HasFragment() const	{	return (mFields & Uri::FRAGMENT) == Uri::FRAGMENT;	}
	const MutStr& GetScheme() const	{	return mScheme;		}
	const MutStr& GetPath() const		{	return mPath;		}
	const MutStr& GetQuery() const	{	return mQuery;		}
	const MutStr& GetFragment() const	{	return mFragment;	}
	const MutStr& GetPort() const		{	return mPort;		}
	const MutStr& GetUserInfo() const {	return mUserInfo;	}
	const MutStr& GetHost() const		{	return mHost;		}
	const Uri::HostType& GetHostType() const	{	return mHostType;	}
	MutStr GetUser() const
	{ // no ':' == whole string, as the password was omitted this is valid
		return mUserInfo.substr(0, mUserInfo.find(':'));
	}
	MutStr GetPassword() const
	{
		size_t dwPasswordPos = mUserInfo.find(':');
		if (dwPasswordPos == WSMAXLENGTH)
			return MutStr();
		return mUserInfo.substr(dwPasswordPos + 1, mUserInfo.length() + 1);
	}
	bool IsRelative() const { return !HasScheme() || !HasHost(); }
	bool IsValidHostname(Uri::Strictness strict = Uri::STRICT) const
	{
		const char* s = mHost.c_str(); const char* labelstart = s;
		if (mHostType != Uri::REGNAME) // can't be an unresolved ip address
			return FALSE;
		for (;*s;++s)
		{
			if (s == labelstart)
			{
				//a-z, A-Z, 0-9
				//Newer RFC 1123 allows leading digit, RFC 952 doesn't
				if (Uri::IsDigit(*s) && strict == Uri::NONSTRICT)
					return FALSE;
				if ((Uri::IsAlpha(*s) || Uri::IsDigit(*s)) == FALSE)
					return FALSE;
			}
			else if (*s == '.')
			{
				if (s == labelstart) // nothing in the label
					return FALSE;
				if (*(s-1) == '-')	// can't end with hyphen
					return FALSE;
				labelstart = s;
				++labelstart;
			}
			else if ((Uri::IsAlpha(*s) || Uri::IsDigit(*s) || '-') == FALSE)
				return FALSE; // not a valid char: a-z, A-Z, 0-9, -
		}
		if (*(s-1) == '.' && (s-1) == labelstart)
			return FALSE; // last label is empty (ends with ..)
		return TRUE;
	}
	void Normalize()
	{ // 6.2.2.	 Syntax-Based Normalization
		// Normalizes directories in-place; I.E. ./ and . are ignored, and relevant
		// ../ and .. are removed along with the directory before them if said directory exists.
		char* s = (char*)mPath.c_str(); char* cp = s; char* bp = s;
		if (!HasPath())
			return;
		while (*cp)
		{
			if (*cp == '.' && (*(cp+1) == '/' || *(cp+1) == '\0')
				&& (cp == mPath.c_str() || !Uri::IsPCharNE(*(cp-1)) || *(cp-1) == '/'))
			{
				//. _or_ ./ - remove (6.2.2.3. Path Segment Normalization)
				if (*(cp+1) == '\0')
					cp += 1;
				else
					cp += 2;
			}
			else if (*cp == '.' && *(cp+1) == '.' && (*(cp+2) == '/' || *(cp+2) == '\0')
					 && (cp == mPath.c_str() || !Uri::IsPCharNE(*(cp-1)) || *(cp-1) == '/'))
			{
				//.. _or_ ../ - remove as well as previous path if it exists (6.2.2.3)
				Uri::UpTree((const char*)bp, (const char*&)s);
				if (*(cp+2) == '\0')
					cp += 2;
				else
					cp += 3;
			}
			else if (*cp == '%' && Uri::CharToHex(*(cp+1)) != -1
								&& Uri::CharToHex(*(cp+2)) != -1)
				Uri::NormalizeEncoded(s, cp);
			else
				*s++ = *cp++;
		}
		mPath.erase(s - mPath.c_str()); // *s = '\0';
	}
	// CodeWarrior requires DummyRecomposeProc be before
	// a declaration of it as a default argument
	static MutStr DummyRecomposeProc(const MutStr& escapedURI)
	{ return escapedURI; }
	MutStr Recompose(RECOMPOSEPROC decodeproc
		= MutUri::DummyRecomposeProc) const
	{
		MutStr ret;
		if (HasScheme())
			ret.append(mScheme).append(":");
		if (HasHost())
		{
			ret.append("//");
			if (HasUserInfo())
				ret.append((*decodeproc)(mUserInfo)).append("@");
			if (mHostType == Uri::REGNAME)
				ret.append((*decodeproc)(mHost));
			else
				ret.append(mHost);
			if (HasPort())
				ret.append(":").append(mPort);
		}
		ret += (*decodeproc)(mPath);
		if (HasQuery())
			ret.append("?").append((*decodeproc)(mQuery));
		if (HasFragment())
			ret.append("#").append((*decodeproc)(mFragment));
		return StringReference(ret);
	}
	MutStr BuildUri(RECOMPOSEPROC proc = MutUri::DummyRecomposeProc)
	{ return StringReference(Recompose(proc)); }
	MutStr BuildUnescapedUri() const
	{ return StringReference(Recompose(Uri::Unescape)); }
	void Resolve(const MutUri& base, Uri::Strictness strict = Uri::STRICT)
	{
		//WSASSERT_MSG(!base.IsRelative(),
		//			"MutUri to inherit from must not be a reference!");
		// If we aren't being strict, enable the older (pre-RFC2396)
		// loophole that allows this uri to inherit other
		// properties from the base uri - even if the scheme
		// is defined
		if (strict == Uri::NONSTRICT &&
			HasScheme() && base.HasScheme() &&
			mScheme == base.mScheme )
			mFields &= ~Uri::SCHEME;

		// Do nothing if this is an absolute MutUri
		//	  if defined(R.scheme) then
		//		 T.scheme	 = R.scheme;
		//		 T.authority = R.authority;
		//		 T.path		 = remove_dot_segments(R.path);
		//		 T.query	 = R.query;
		if (HasScheme())
		{
			Normalize();
			return;
		}
		//No sheme - inherit
		mScheme = base.mScheme;
		mFields |= Uri::SCHEME;
		// All we need to do for relative URIs with an
		// authority component is just inherit the scheme
		//		 if defined(R.authority) then
		//			T.authority = R.authority;
		//			T.path		= remove_dot_segments(R.path);
		//			T.query		= R.query;
		if (HasHost())
		{
			Normalize();
			return;
		}
		//No authority - inherit
		if (base.HasUserInfo())
		{
			mUserInfo = base.mUserInfo;
			mFields |= Uri::USERINFO;
		}
		mHost = base.mHost;
		mHostType = base.mHostType;
		mFields |= Uri::HOST;
		if (base.HasPort())
		{
			mPort = base.mPort;
			mFields |= Uri::PORT;
		}
		// if (R.path == "") then
		//	   T.path = Base.path;
		//	   if defined(R.query) then
		//		   T.query = R.query;
		//	   else
		//		   T.query = Base.query;
		//	   endif;
		if (!HasPath() || mPath.length() == 0)
		{
			mPath = base.mPath;
			mFields |= Uri::PATH;
			if (!HasQuery())
			{
				mQuery = base.mQuery;
				mFields |= Uri::QUERY;
			}
		}
		// else
		//	   if (R.path starts-with "/") then
		//		   T.path = remove_dot_segments(R.path);
		//	   else
		//		   T.path = merge(Base.path, R.path);
		//		   T.path = remove_dot_segments(T.path);
		//	   endif;
		//	   T.query = R.query;
		// endif;
		else if (mPath[0] != '/')
		{
			//Marge paths
			MutUri normalizedbase(base);
			normalizedbase.Normalize();
			const char* bp = normalizedbase.mPath.c_str() + normalizedbase.mPath.length();
			//not a ending directory?  move up
			if (normalizedbase.HasPath() && normalizedbase.mPath[0] && *(bp-1) != '/')
				Uri::UpTree(normalizedbase.mPath.c_str(), bp);
			mPath = normalizedbase.mPath.substr(0, bp - normalizedbase.mPath.c_str()) + mPath;
		}
		//T.fragment = R.fragment;
		Normalize();
	}
	MutUri& operator = (const MutStr& string) { Create(string); return *this; }
	bool operator == (const MutUri& uri) const
	{
		if (HasScheme())
		{
			if (mScheme != uri.mScheme)
				return FALSE;
		}
		else if (uri.HasScheme())
			return FALSE;
		if (HasHost())
		{
			if (HasUserInfo())
			{
				if (mUserInfo != uri.mUserInfo)
					return FALSE;
			}
			else if (uri.HasUserInfo())
				return FALSE;
			if (mHost != uri.mHost || mHostType != uri.mHostType)
				return FALSE;
			if (HasPort())
			{
				if (mPort != uri.mPort)
					return FALSE;
			}
			else if (uri.HasPort())
				return FALSE;
		}
		else if (uri.HasHost())
			return FALSE;
		if (HasPath())
		{
			if (mPath != uri.mPath)
				return FALSE;
		}
		else if (uri.HasPath())
			return FALSE;
		if (HasQuery())
		{
			if (mQuery != uri.mQuery)
				return FALSE;
		}
		else if (uri.HasQuery())
			return FALSE;
		if (HasFragment())
		{
			if (mFragment != uri.mFragment)
				return FALSE;
		}
		else if (uri.HasFragment())
			return FALSE;
		return TRUE;
	}

protected:
	void Destroy()
	{
		mScheme.resize(0); mUserInfo.resize(0); mHost.resize(0);
		mPort.resize(0); mPath.resize(0); mQuery.resize(0);
		mFragment.resize(0); mHostType = Uri::REGNAME;
		mFields = 0;
	}
	bool Parse(const char* uri)
	{
		uri = ParseScheme(uri);
		if (uri)
			uri = ParseAuthority(uri);
		if (uri)
			uri = ParsePath(uri);
		if (uri)
			uri = ParseQuery(uri);
		if (uri)
			uri = ParseFragment(uri);
		return uri && *uri == '\0'; // succeeds only if it parsed the whole string
	}
	const char* ParseScheme(const char* uri)
	{
		const char* const uristart = uri;
		//scheme		= ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
		if (Uri::IsAlpha(*uri))
		{
			mScheme += *uri++;
			while (Uri::IsAlpha(*uri) || Uri::IsDigit(*uri) ||
				   *uri == '+' || *uri == '-' || *uri == '.')
				mScheme += *uri++;
			if (*uri == ':') // valid scheme?
			{
				mFields |= Uri::SCHEME; //mark the scheme as valid
				++uri;
			}
			else // no valid scheme
			{
				uri = uristart; // backtrack
				mScheme.erase();
			}
		} // else no scheme, _possible_ relative MutUri
		return uri;
	}
	const char* ParseAuthority(const char* uri)
	{
		// URI			 = scheme ":" hier-part [ "?" query ] [ "#" fragment ]
		// hier-part	 = "//" authority path-abempty
		bool leadswithdoubleslash = uri[0] == '/' && uri[1] == '/';
		if(leadswithdoubleslash)
		{
			//skip past the two slashes
			uri += 2;
		}

		// ############# DEVIATION FROM RFC #########################
		// Evidence shows us that users expect us to parse the
		// server component in the case of [server][#fragment].
		// Otherwise they don't want us to parse the server at all
		// for file uris. RFC 8089 allows for leading "//" with file.
		if (mScheme.compare("file") == 0)
		{ // look for [server][#fragment] case
			bool serverfragmentfound = FALSE;
			const char* uristart = uri;
			uri = ParseHost(uri);
			if(HasHost())
			{
				uri = ParseFragment(uri);
				serverfragmentfound = HasFragment();
			}
			if(!serverfragmentfound)
			{ // nope... clear fields, return to point after opening slashes
				uri = uristart;
				mHost.erase();
				mFields &= ~Uri::HOST;
			}
		}
		else if(leadswithdoubleslash)
		{ // normal RFC-based authority parsing
			// authority	 = [ userinfo "@" ] host [ ":" port ]
			// file-hier-part = ( "//" auth-path ) / local-path
			const char* uristart = uri;
			uri = ParseUserInfo(uri);
			uri = ParseHost(uri);
			if (!HasHost() && HasUserInfo()) // in practice this case doesn't happen without scheme-based parsing
			{ // userinfo can't exist without host, backtrack and reparse
				mUserInfo.erase();
				mFields &= ~Uri::USERINFO;
				uri = uristart;
				uri = ParseHost(uri);
			}
			uri = ParsePort(uri);
			if (!HasHost()) // in practice this case doesn't happen without scheme-based parsing
			{
				mPort.erase(); // port can't exist without host
				mFields &= ~Uri::PORT;
				uri = uristart - 2; // nothing found, skip to before the leading "//"
			}
		}

		return uri;
	}
	const char* ParseUserInfo(const char* uri)
	{
		const char* const uristart = uri;
		// userinfo		 = *( unreserved / pct-encoded / sub-delims / ":" )
		while (*uri && *uri != '@' && *uri != '/' && *uri != '#' && *uri != '?')
		{
			if (Uri::IsUnreserved(*uri) || Uri::IsSubDelim(*uri) || *uri == ':')
				mUserInfo += *uri++;
			else
				Uri::Escape(mUserInfo, uri);
		}
		if (*uri++ == '@')
			mFields |= Uri::USERINFO; //valid userinfo
		else
		{
			uri = uristart; // backtrack
			mUserInfo.erase();
		}
		return uri;
	}
	const char* ParseHost(const char* uri)
	{
		const char* uristart = uri;
		// host			 = IP-literal / IPv4address / reg-name
		// IP-literal	 = "[" ( IPv6address / IPvFuture  ) "]"
		if (*uri == '[')
		{
			++uri;
			if (Uri::ParseIPv6address(uri) && *uri == ']')
			{
				mHostType = Uri::IPV6ADDRESS;
				mHost.append(uristart + 1, uri - uristart - 1);
				++uri;
			}
			else
			{
				uri = uristart + 1; // skip over '['
				if (Uri::ParseIPvFuture(uri) && *uri == ']')
				{
					mHostType = Uri::IPVFUTURE;
					mHost.append(uristart + 1, uri - uristart - 1);
					++uri;
				}
				else // backtrack due to it not being recognized, IP literal
					uri = uristart;
			}
		}
		else // IPv4address / reg-name
		{
			if (Uri::ParseIPv4address(uri) && (!*uri || *uri == '/' || *uri == ':'))
			{
				mHostType = Uri::IPV4ADDRESS;
				mHost.append(uristart, uri - uristart);
			}
			else // backtrack
				uri = uristart;
		}
		if (uri == uristart) // reg-name
		{
			uri = uristart; // backtrack
			// reg-name		 = *( unreserved / pct-encoded / sub-delims )
			while (*uri && *uri != '/' && *uri != ':' && *uri != '#' && *uri != '?')
			{
				if (Uri::IsUnreserved(*uri) || Uri::IsSubDelim(*uri))
					mHost += *uri++;
				else
					Uri::Escape(mHost, uri);
			}
		}
		mFields |= Uri::HOST; // reg-name can be empty
		return uri;
	}
	const char* ParsePort(const char* uri)
	{
		//[ ":" port ], port = *DIGIT
		if (*uri == ':')
		{
			++uri;
			while (Uri::IsDigit(*uri))
				mPort += *uri++;
			mFields |= Uri::PORT; //mark the port as valid
		}
		return uri;
	}
	const char* ParsePath(const char* uri)
	{
		// hier-part	 = "//" authority path-abempty
		//				 / path-absolute
		//				 / path-rootless
		//				 / path-empty
		// relative-part = "//" authority path-abempty
		//				 / path-absolute
		//				 / path-noscheme
		//				 / path-empty
		// path-abempty	 = *( "/" segment )
		// path-absolute = "/" [ segment-nz *( "/" segment ) ]
		// path-noscheme = segment-nz-nc *( "/" segment )
		// path-rootless = segment-nz *( "/" segment )
		// path-empty	 = 0<pchar>
		// segment		 = *pchar
		// pchar		 = unreserved / pct-encoded / sub-delims / ":" / "@"
		if (Uri::IsEndPath(*uri)
			// When authority is present,
			// the path must either be empty or begin with a slash ("/") character.
			// When authority is not present,
			// the path cannot begin with two slash characters ("//").
			|| (mHost.length() ? (!(!*uri || *uri == '/')) : (*uri == '/' && *(uri+1) == '/')))
			return uri;
		const char* uristart = uri;
		const bool isAbsolute = *uri == '/';
		if (isAbsolute)
		{
			mPath += *uri++;
			// segment-nz	 = 1*pchar
			while (!Uri::IsEndPath(*uri))
			{
				if (Uri::IsPCharNE(*uri) || *uri == '/')
					mPath += *uri++;
				else
					Uri::Escape(mPath, uri);
			}
			mFields |= Uri::PATH; //mark the path as valid
		}
		else if (*uri) //Relative path
		{
			if (!HasScheme())
			{
				// segment-nz-nc = 1*( unreserved / pct-encoded / sub-delims / "@" )
				//				 ; non-zero-length segment without any colon ":"
				while (!Uri::IsEndPath(*uri))
				{
					if (Uri::IsPCharNCNE(*uri) || *uri == '/')
						mPath += *uri++;
					else
						Uri::Escape(mPath, uri);
				}
			}
			else
			{
				// segment-nz	 = 1*pchar
				while (!Uri::IsEndPath(*uri))
				{
					if (Uri::IsPCharNE(*uri) || *uri == '/')
						mPath += *uri++;
					else
						Uri::Escape(mPath, uri);
				}
			}
			if (uri != uristart)
				mFields |= Uri::PATH; // mark the path as valid
		}
		return uri;
	}
	const char* ParseQuery(const char* uri)
	{
		// query		 = *( pchar / "/" / "?" )
		if (*uri == '?')
		{
			++uri;
			while (*uri && *uri != '#')
			{
				if (Uri::IsPCharNE(*uri) || *uri == '/' || *uri == '?')
					  mQuery += *uri++;
				else
					  Uri::Escape(mQuery, uri);
			}
			mFields |= Uri::QUERY; //mark the query as valid
		}
		return uri;
	}
	const char* ParseFragment(const char* uri)
	{
		// fragment		 = *( pchar / "/" / "?" )
		if (*uri == '#')
		{
			++uri;
			while (*uri)
			{
				if (Uri::IsPCharNE(*uri) || *uri == '/' || *uri == '?')
					  mFragment += *uri++;
				else
					  Uri::Escape(mFragment, uri);
			}
			mFields |= Uri::FRAGMENT; //mark the fragment as valid
		}
		return uri;
	}

	MutStr mScheme;
	MutStr mPath;
	MutStr mQuery;
	MutStr mFragment;
	MutStr mUserInfo;
	MutStr mHost;
	MutStr mPort;
	size_t mFields;
	Uri::HostType mHostType;
};//end of MutUri

#if defined(WIKISERVER_VERSION) && defined(WSTESTOPT)
extern "C" {
WSTEST_SUITE_DECLARE(WSMUTURI);
}
#endif
#endif /* WSMUTURIOPT */
#endif /* __cplusplus */
#endif /* __URI_H__ */
/******************************************************************************
	 ##### ###                                                        #####
	 #      #  #    # #####  #      ###### #    # ###### #    # #####     #
	 #      #  ##  ## #    # #      #      ##  ## #      ##   #   #       #
	 #      #  # ## # #    # #      #####  # ## # #####  # #  #   #       #
	 #      #  #    # #####  #      #      #    # #      #  # #   #       #
	 #      #  #    # #      #      #      #    # #      #   ##   #       #
	 ##### ### #    # #      ###### ###### #    # ###### #    #   #   #####
******************************************************************************/
#if defined(URI_IMPLEMENTATION)
/**
*	@file uri.c
*
*	RFC 3986 compliant URI parser implementation.
*
*	Written in ANSI C (C89) for compiler compatability.
*/
/*#include "system.h"*/
#ifdef URISELFCONTAINED
	#define WSMUTURIOPT
#endif

/*#ifndef PRECOMPILEDHEADERS
	#include "uri.h"
#endif*/
#if !defined(__cplusplus) && defined(WSMUTURIOPT)
	#undef WSMUTURIOPT
#endif

#if !defined(WIKISERVER_VERSION)
	#include <string.h> /* memcpy/memchr */
	#include <stdlib.h> /* malloc/realloc/free */
	#define countof(x) (sizeof(x) / sizeof(*x))
	#define strcountof(x) (countof(x) - 1)
	#define WSMemoryFind memchr
	#define WSMemoryCopy memcpy
	#define WSMemoryCompare memcmp
	#define WSALLOC(type, count) (type*)malloc(sizeof(type) * (count))
	#define WSALLOC1(x) (x*)malloc(sizeof(x))
	#define WSREALLOC(old, type, count) \
		(type*)realloc(old, sizeof(type) * (count))
	#define	WSFREE(x) free((void*)(x))
	#define CStrLength(x) strlen(x)
#endif
#if !defined(WSStr_Erase)
	#define WSStr_Erase(pThis, loc) (pThis)->mLength = loc;
#endif

#if defined(__cplusplus)
extern "C" {
#endif

/*=============================================================================
								URI [Private]
=============================================================================*/
wsbool URI_Parse				(URI* pThis, const char* uri);
const char* URI_ParseAuthority	(URI* pThis, const char* uri);
const char* URI_ParseScheme		(URI* pThis, const char* uri);
const char* URI_ParseUserInfo	(URI* pThis, const char* uri);
const char* URI_ParseHost		(URI* pThis, const char* uri);
const char* URI_ParsePort		(URI* pThis, const char* uri);
const char* URI_ParsePath		(URI* pThis, const char* uri);
const char* URI_ParseQuery		(URI* pThis, const char* uri);
const char* URI_ParseFragment	(URI* pThis, const char* uri);

wsbool URIDummyRecomposeProc(const char* uri, size_t urilen,
	char** out, size_t* outlen)
{ *out = (char*)uri; *outlen = urilen; return FALSE; }
void URI_BuildURI(const URI* pThis, char** out, size_t* outlen)
{ URI_Recompose(pThis, URIDummyRecomposeProc, out, outlen); }
void URI_BuildUnescapedURI(const URI* pThis, char** out, size_t* outlen)
{ URI_Recompose(pThis, URIUnescape, out, outlen); }

wsbool URIIsWSSTRNotEqual(const WSSTR* pStr, const WSSTR* pOther)
{
	return pStr->mLength != pOther->mLength
		|| WSMemoryCompare(pStr->mData, pOther->mData,
			pStr->mLength * sizeof(char)) != 0;
}
wsbool UriPartCmpWithLength(const WSSTR* str, const char* s, size_t slen)
{
	return str->mLength == slen
		&& WSMemoryCompare(str->mData, s, slen * sizeof(char)) == 0;
}
wsbool UriPartCmp(const WSSTR* str, const char* s)
 {
	return UriPartCmpWithLength(str, s, CStrLength(s));
}

/*=============================================================================
								URI
=============================================================================*/
void URI_Construct(URI* pThis)
{ /* Nothing needed in current API */
	/*pThis->mScheme.mData = NULL; pThis->mScheme.mLength = 0;
	pThis->mPath.mData = NULL; pThis->mPath.mLength = 0;
	pThis->mQuery.mData = NULL; pThis->mQuery.mLength = 0;
	pThis->mFragment.mData = NULL; pThis->mFragment.mLength = 0;
	pThis->mUserInfo.mData = NULL; pThis->mUserInfo.mLength = 0;
	pThis->mHost.mData = NULL; pThis->mHost.mLength = 0;
	pThis->mPort.mData = NULL; pThis->mPort.mLength = 0;
	pThis->mHostType = URI_REGNAME;
	pThis->mFields = 0;*/
	(void)(pThis);
}

void URI_Destroy(URI* pThis)
{ /* Nothing needed in current API */
	/*pThis->mHostType = URI_REGNAME;
	pThis->mFields = 0;*/
	(void)(pThis);
}
wsbool URI_Create(URI* pThis, const char* uri)
{
	return URI_Parse(pThis, uri);
}
char* URIEscape(char* s, const char** uri)
{ /* pct-encoded	 = "%" HEXDIG HEXDIG */
	if (*(*uri) == '%' && URIIsHex(*((*uri)+1)) && URIIsHex(*((*uri)+2)))
	{
		*s++ = *(*uri)++;
		*s++ = *(*uri)++;
		*s++ = *(*uri)++;
	}
	else /* not a valid escape sequence, encode */
	{	/* 6.2.2.1 dictates uppercase */
		const char* hexdig = "0123456789ABCDEF";
		char c = *(*uri)++;
		*s++ = '%';
		*s++ = hexdig[(c >> 4) & 15];
		*s++ = hexdig[c & 15];
	}
	return s;
}
wsbool URIUnescape(const char* uri, size_t urilen, char** out, size_t* outlen)
{ /* uri == (UTF8 string) uri, unescapedsize <= utf8size */
	/*
		RFC deviation: RFC 3986 dictates that escaped characters be decoded as
		UTF-8, but it is possible in some backward compatability cases where a
		character can't be decoded as UTF-8, which we handle, again, for
		backwards compatability.
	*/
	char* newuri = WSALLOC(char, urilen);
	const char* sEnd;
	char* sNewuri = (char*)newuri;
	if(newuri)
	{
		const char* s;
		sEnd = uri + urilen;
		for (s = uri; s != sEnd; ++s, ++sNewuri)
		{
			char c = *s;
			if (c == '%' && s < sEnd - 2 && URIIsHex(s[1]) && URIIsHex(s[2]))
			{
				c = (char)((URICharToHex(s[1]) << 4) | URICharToHex(s[2]));
				s += 2;
			}
			*sNewuri = c;
		}
		*sNewuri = '\0'; /* newuri.erase(sNewuri - newuri.c_str()); */
		*out = newuri;
	}
	*out = newuri;
	*outlen = sNewuri - newuri;
	return newuri != NULL;
}
void URI_GetUser(const URI* pThis, WSSTR* pOutUser)
{
	/* no ':' == whole string */
	size_t userlength;
	const char* colonPos = (const char*)
		WSMemoryFind(pThis->mUserInfo.mData, ':', pThis->mUserInfo.mLength);
	if (!colonPos)
	{
		userlength = pThis->mUserInfo.mLength;
		if(!userlength)
		{
			pOutUser->mData = NULL;
			pOutUser->mLength = 0;
			return;
		}
	}
	else
		userlength = colonPos - pThis->mUserInfo.mData;
	pOutUser->mData = &pThis->mUserInfo.mData[0];
	pOutUser->mLength = userlength;
}
void URI_GetPassword(const URI* pThis, WSSTR* pOutPassword)
{
	const char* colonPos = (const char*)
		WSMemoryFind(pThis->mUserInfo.mData, ':', pThis->mUserInfo.mLength);
	if (!colonPos)
	{
		pOutPassword->mData = NULL;
		pOutPassword->mLength = 0;
	}
	else
	{
		pOutPassword->mData = (char*)&colonPos[1]; /* From colon on */
		pOutPassword->mLength =
			(pThis->mUserInfo.mData + pThis->mUserInfo.mLength)
				- (pOutPassword->mData);
	}
}
wsbool URI_IsValidHostname(const URI* pThis, URIStrictness strict)
{
	const char* s; const char* labelstart;
	if (pThis->mHostType != URI_REGNAME)
		return FALSE; /* can't be an unresolved ip address */
	s = pThis->mHost.mData; labelstart = s;
	for (;*s;++s)
	{
		if (s == labelstart)
		{
			/* a-z, A-Z, 0-9 */
			/* Newer RFC 1123 allows leading digit, RFC 952 doesn't */
			if (URIIsDigit(*s) && strict == URI_NONSTRICT)
				return FALSE;
			if ((URIIsAlpha(*s) || URIIsDigit(*s)) == FALSE)
				return FALSE;
		}
		else if (*s == '.')
		{
			if (s == labelstart) /* nothing in the label */
				return FALSE;
			if (*(s-1) == '-')	/* can't end with hyphen */
				return FALSE;
			labelstart = s;
			++labelstart;
		}
		else if ((URIIsAlpha(*s) || URIIsDigit(*s) || (*s) == '-') == FALSE)
			return FALSE; /* not a valid char: a-z, A-Z, 0-9, - */
	}
	if (*(s-1) == '.' && (s-1) == labelstart)
		return FALSE; /* last label is empty (ends with ..) */
	return TRUE;
}
void URICStrAppend(char** ret, size_t* retlen, size_t* retsize,
	const char* part, size_t partlen)
{
	if(*retsize - *retlen <= partlen)
	{
		size_t newretsize = partlen + *retsize;
		char* newret = WSREALLOC(*ret, char, *retsize + partlen);
		if(!newret)
			return;
		*ret = newret;
		*retsize = newretsize;
	}
	WSMemoryCopy(*ret + *retlen, part, partlen * sizeof(char));
	*retlen += partlen;
}
void URICStrAppendDecoded(char** ret, size_t* retlen, size_t* retsize,
	const char* part, size_t partlen, URIRECOMPOSEPROC decodeproc)
{
	char* partdecoded;
	size_t partdecodedlen;
	wsbool shouldfree = (*decodeproc)(part, partlen,
		&partdecoded, &partdecodedlen);

	URICStrAppend(ret, retlen, retsize, partdecoded, partdecodedlen);

	if(shouldfree)
		WSFREE(partdecoded);
}


void URI_Recompose(const URI* pThis, URIRECOMPOSEPROC proc,
	char** out, size_t* outlen)
{
	char* ret;
	size_t retlen = 0, retsize =
		pThis->mScheme.mLength + pThis->mHost.mLength
		+ pThis->mPath.mLength + pThis->mQuery.mLength
		+ pThis->mFragment.mLength + pThis->mPort.mLength
		+ pThis->mUserInfo.mLength + 8;
	ret = WSALLOC(char, retsize);
	if(ret)
	{
		if (URI_HasScheme(pThis))
		{
			URICStrAppend(&ret, &retlen, &retsize,
				pThis->mScheme.mData, pThis->mScheme.mLength);
			URICStrAppend(&ret, &retlen, &retsize,
				":", strcountof(":"));
		}
		if (URI_HasHost(pThis))
		{
			URICStrAppend(&ret, &retlen, &retsize,
				"//", strcountof("//"));
			if (URI_HasUserInfo(pThis))
			{
				URICStrAppendDecoded(&ret, &retlen, &retsize,
					pThis->mUserInfo.mData, pThis->mUserInfo.mLength, proc);
				URICStrAppend(&ret, &retlen, &retsize,
					 "@", strcountof("@"));
			}
			if (pThis->mHostType == URI_REGNAME)
				URICStrAppendDecoded(&ret, &retlen, &retsize,
					pThis->mHost.mData, pThis->mHost.mLength, proc);
			else
				URICStrAppend(&ret, &retlen, &retsize,
					pThis->mHost.mData, pThis->mHost.mLength);
			if (URI_HasPort(pThis))
			{
				URICStrAppend(&ret, &retlen, &retsize,
					 ":", strcountof(":"));
				URICStrAppendDecoded(&ret, &retlen, &retsize,
					pThis->mPort.mData, pThis->mPort.mLength, proc);
			}
		}
		URICStrAppendDecoded(&ret, &retlen, &retsize,
			pThis->mPath.mData, pThis->mPath.mLength, proc);
		if (URI_HasQuery(pThis))
		{
			URICStrAppend(&ret, &retlen, &retsize,
					"?", strcountof("?"));
			URICStrAppendDecoded(&ret, &retlen, &retsize,
				pThis->mQuery.mData, pThis->mQuery.mLength, proc);
		}
		if (URI_HasFragment(pThis))
		{
			URICStrAppend(&ret, &retlen, &retsize,
					"#", strcountof("#"));
			URICStrAppendDecoded(&ret, &retlen, &retsize,
				pThis->mFragment.mData, pThis->mFragment.mLength, proc);
		}
		ret[retlen] = '\0';
	}
	*out = ret;
	*outlen = retlen;
}
wsbool URI_IsEqual(const URI* pThis, const URI* uri)
{
	if (URI_HasScheme(pThis))
	{
		if (URIIsWSSTRNotEqual(&pThis->mScheme, &uri->mScheme))
			return FALSE;
	}
	else if (URI_HasScheme(uri))
		return FALSE;
	if (URI_HasHost(pThis))
	{
		if (URI_HasUserInfo(pThis))
		{
			if (URIIsWSSTRNotEqual(&pThis->mUserInfo, &uri->mUserInfo))
				return FALSE;
		}
		else if (URI_HasUserInfo(uri))
			return FALSE;
		if (URIIsWSSTRNotEqual(&pThis->mHost, &uri->mHost)
				|| pThis->mHostType != uri->mHostType)
			return FALSE;
		if (URI_HasPort(pThis))
		{
			if (URIIsWSSTRNotEqual(&pThis->mPort, &uri->mPort))
				return FALSE;
		}
		else if (URI_HasPort(uri))
			return FALSE;
	}
	else if (URI_HasHost(uri))
		return FALSE;
	if (URI_HasPath(pThis))
	{
		if (URIIsWSSTRNotEqual(&pThis->mPath, &uri->mPath))
			return FALSE;
	}
	else if (URI_HasPath(uri))
		return FALSE;
	if (URI_HasQuery(pThis))
	{
		if (URIIsWSSTRNotEqual(&pThis->mQuery, &uri->mQuery))
			return FALSE;
	}
	else if (URI_HasQuery(uri))
		return FALSE;
	if (URI_HasFragment(pThis))
	{
		if (URIIsWSSTRNotEqual(&pThis->mFragment, &uri->mFragment))
			return FALSE;
	}
	else if (URI_HasFragment(uri))
		return FALSE;
	return TRUE;
}
wsbool URI_Parse(URI* pThis, const char* uri)
{ /* succeeds (TRUE return) only if it parsed the whole string */
	pThis->mFields = 0;
	uri = URI_ParseScheme(pThis, uri);
	/*if (uri)*/
		uri = URI_ParseAuthority(pThis, uri);
	/*if (uri)*/
		uri = URI_ParsePath(pThis, uri);
	/*if (uri)*/
		uri = URI_ParseQuery(pThis, uri);
	/*if (uri)*/
		uri = URI_ParseFragment(pThis, uri);
	return uri && *uri == '\0';
}
const char* URI_ParseScheme(URI* pThis, const char* uri)
{
	const char* const uristart = uri;
	/*scheme		= ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )*/
	if (URIIsAlpha(*uri))
	{
		uri++;
		while (URIIsAlpha(*uri) || URIIsDigit(*uri) ||
			   *uri == '+' || *uri == '-' || *uri == '.')
			uri++;
		if (*uri == ':') /* valid scheme?*/
		{
			pThis->mFields |= URI_SCHEME; /*mark the scheme as valid*/
			pThis->mScheme.mData = (char*)uristart;
			pThis->mScheme.mLength = uri - uristart;
			++uri;
		}
		else /* no valid scheme*/
		{
			WSStr_Erase(&pThis->mScheme, 0);
			uri = uristart; /* backtrack*/
		}
	} /* else no scheme, _possible_ relative URI*/
	else
		WSStr_Erase(&pThis->mScheme, 0);
	return uri;
}
const char* URI_ParseAuthority(URI* pThis, const char* uri)
{
	/* URI			 = scheme ":" hier-part [ "?" query ] [ "#" fragment ]*/
	/* hier-part	 = "//" authority path-abempty*/
	wsbool leadswithdoubleslash = uri[0] == '/' && uri[1] == '/';
	if(leadswithdoubleslash)
	{
		/*skip past the two slashes*/
		uri += 2;
	}

	/*
		############# DEVIATION FROM RFC #########################
		Evidence shows us that users expect us to parse the
		server component in the case of [server][#fragment].
		Otherwise they don't want us to parse the server at all
		for file uris. RFC 8089 allows for leading "//" with file.
	*/
	if (UriPartCmpWithLength(&pThis->mScheme, "file", strcountof("file")))
	{ /* look for [server][#fragment] case */
		wsbool serverfragmentfound = FALSE;
		const char* uristart = uri;
		uri = URI_ParseHost(pThis, uri);
		if(URI_HasHost(pThis))
		{
			uri = URI_ParseFragment(pThis, uri);
			serverfragmentfound = URI_HasFragment(pThis);
		}
		if(!serverfragmentfound)
		{ /* nope... clear fields, return to point after opening slashes*/
			uri = uristart;
			WSStr_Erase(&pThis->mHost, 0);
			pThis->mFields &= ~URI_HOST;
		}
		WSStr_Erase(&pThis->mPort, 0);
		WSStr_Erase(&pThis->mUserInfo, 0);
	}
	else if(leadswithdoubleslash)
	{ /* normal RFC-based authority parsing*/
		/* authority	 = [ userinfo "@" ] host [ ":" port ]*/
		/* file-hier-part = ( "//" auth-path ) / local-path*/
		const char* uristart = uri;
		uri = URI_ParseUserInfo(pThis, uri);
		uri = URI_ParseHost(pThis, uri);
		if (!URI_HasHost(pThis) && URI_HasUserInfo(pThis)) /* in practice this case doesn't happen without scheme-based parsing*/
		{ /* userinfo can't exist without host, backtrack and reparse*/
			WSStr_Erase(&pThis->mUserInfo, 0);
			pThis->mFields &= ~URI_USERINFO;
			uri = uristart;
			uri = URI_ParseHost(pThis, uri);
		}
		uri = URI_ParsePort(pThis, uri);
		if (!URI_HasHost(pThis)) /* in practice this case doesn't happen without scheme-based parsing*/
		{
			WSStr_Erase(&pThis->mPort, 0); /* port can't exist without host*/
			pThis->mFields &= ~URI_PORT;
			uri = uristart - 2; /* nothing found, skip to before the leading "//"*/
		}
	}
	else
	{
		WSStr_Erase(&pThis->mPort, 0);
		WSStr_Erase(&pThis->mHost, 0);
		WSStr_Erase(&pThis->mUserInfo, 0);
	}

	return uri;
}
const char* URI_ParseUserInfo(URI* pThis, const char* uri)
{
	const char* const uristart = uri;
	/* userinfo		 = *( unreserved / pct-encoded / sub-delims / ":" )*/
	while (*uri && *uri != '@' && *uri != '/' && *uri != '#' && *uri != '?')
		uri++;
	if (*uri == '@')
	{
		pThis->mUserInfo.mData = (char*)uristart;
		pThis->mUserInfo.mLength = uri - uristart;
		pThis->mFields |= URI_USERINFO; /*valid userinfo*/
		++uri; /* move past '@'*/
	}
	else
	{
		WSStr_Erase(&pThis->mUserInfo, 0);
		uri = uristart; /* backtrack*/
	}

	return uri;
}
const char* URI_ParseHost(URI* pThis, const char* uri)
{
	const char* uristart = uri;
	/* host			 = IP-literal / IPv4address / reg-name*/
	/* IP-literal	 = "[" ( IPv6address / IPvFuture  ) "]"*/
	if (*uri == '[')
	{
		++uri;
		if (URIParseIPv6address(&uri) && *uri == ']')
		{
			pThis->mHostType = URI_IPV6ADDRESS;
			pThis->mHost.mData = (char*)uristart + 1;
			pThis->mHost.mLength = uri - uristart - 1;
			++uri;
		}
		else
		{
			uri = uristart + 1; /* skip over ']'*/
			if (URIParseIPvFuture(&uri) && *uri == ']')
			{
				pThis->mHostType = URI_IPVFUTURE;
				pThis->mHost.mData = (char*)uristart + 1;
				pThis->mHost.mLength = uri - uristart - 1;
				++uri;
			}
			else /* backtrack due to it not being recognized, IP literal*/
				uri = uristart;
		}
	}
	else /* IPv4address / reg-name*/
	{
		if (URIParseIPv4address(&uri) && (!*uri || *uri == '/' || *uri == ':'))
		{
			pThis->mHostType = URI_IPV4ADDRESS;
			pThis->mHost.mData = (char*)uristart;
			pThis->mHost.mLength = uri - uristart;
		}
		else /* backtrack*/
			uri = uristart;
	}
	if (uri == uristart) /* reg-name*/
	{
		uri = uristart; /* backtrack*/
		/* reg-name		 = *( unreserved / pct-encoded / sub-delims )*/
		while (*uri && *uri != '/' && *uri != ':' && *uri != '#' && *uri != '?')
			uri++;
		pThis->mHost.mData = (char*)uristart;
		pThis->mHost.mLength = uri - uristart;
		pThis->mHostType = URI_REGNAME;
	}
	pThis->mFields |= URI_HOST; /* reg-name can be empty*/
	return uri;
}
const char* URI_ParsePort(URI* pThis, const char* uri)
{
	const char* uristart = uri;
	/*[ ":" port ], port = *DIGIT*/
	if (*uri == ':')
	{
		++uri;
		while (URIIsDigit(*uri))
			uri++;
		pThis->mPort.mData = (char*)uristart + 1;
		pThis->mPort.mLength = uri - (uristart + 1);
		pThis->mFields |= URI_PORT; /*mark the port as valid*/
	}
	else
		WSStr_Erase(&pThis->mPort, 0);
	return uri;
}
void URINormalizeEncoded(char** s, char** cp)
{
	char pctchar;
	/*6.2.2.2.	Percent-Encoding Normalization*/
	/*1) Make encoded alpha uppercase (6.2.2.1)*/
	if (*((*cp)+1) >= 'a' && *((*cp)+1) <= 'z')
		*((*cp)+1) -= 32;
	if (*((*cp)+2) >= 'a' && *((*cp)+2) <= 'z')
		*((*cp)+2) -= 32;
	/*2) Decode unreserved chars (6.2.2.2)*/
	if (*((*cp)+1) >= 'A' && *((*cp)+1) <= 'Z')
		pctchar = (*((*cp)+1) - 'A' + 10) * 16;
	else
		pctchar = (*((*cp)+1) - '0') * 16;
	if (*((*cp)+2) >= 'A' && *((*cp)+2) <= 'Z')
		pctchar += (*((*cp)+2) - 'A' + 10);
	else
		pctchar += (*((*cp)+2) - '0');
	if (URIIsUnreserved(pctchar))
	{
		*(*s)++ = pctchar;
		(*cp) += 3;
	}
	else
	{
		*(*s)++ = *(*cp)++;
		*(*s)++ = *(*cp)++;
		*(*s)++ = *(*cp)++;
	}
}
void URI_Normalize(URI* pThis)
{ /* 6.2.2.	 Syntax-Based Normalization */
	/*
		Normalizes directories in-place;
		I.E. ./ and . are ignored, and relevant
		../ and .. are removed along with the directory
		before them if said directory exists.
	*/
	char* s = (char*)pThis->mPath.mData; char* cp = s; char* bp = s;
	if (!URI_HasPath(pThis))
		return;
	while (*cp)
	{
		if (*cp == '.' && (*(cp+1) == '/' || *(cp+1) == '\0')
			&& (cp == pThis->mPath.mData || !URIIsPCharNE(*(cp-1)) || *(cp-1) == '/'))
		{
			/*.
				_or_ ./
				remove (6.2.2.3. Path Segment Normalization)
			*/
			if (*(cp+1) == '\0')
				cp += 1;
			else
				cp += 2;
		}
		else if (*cp == '.' && *(cp+1) == '.' && (*(cp+2) == '/' || *(cp+2) == '\0')
				 && (cp == pThis->mPath.mData || !URIIsPCharNE(*(cp-1)) || *(cp-1) == '/'))
		{
			/*
				.. _or_ ../
				remove as well as previous path if it exists (6.2.2.3)
			*/
			URIUpTree((const char*)bp, (const char**)&s);
			if (*(cp+2) == '\0')
				cp += 2;
			else
				cp += 3;
		}
		else if (*cp == '%' && URICharToHex(*(cp+1)) != -1
				&& URICharToHex(*(cp+2)) != -1)
		{
			cp += 3;
			s = cp;
		}
		else
		{
			cp++;
			s++;
		}
	}
	WSStr_Erase(&pThis->mPath, s - pThis->mPath.mData); /* *s = '\0'; */
}
/* Moves a URI path up a directory*/
void URIUpTree(const char* uristart, const char** uri)
{
	while ((*uri) != uristart && *((*uri)-1) == '/')
		--(*uri);
	while ((*uri) != uristart && *--(*uri) != '/') {}
	if (*(*uri) == '/')
		++(*uri);
}
const char* URI_ParsePath(URI* pThis, const char* uri)
{
	const char* uristart;
	wsbool isAbsolute;
	/*
		hier-part	 = "//" authority path-abempty
					 / path-absolute
					 / path-rootless
					 / path-empty
		relative-part = "//" authority path-abempty
					 / path-absolute
					 / path-noscheme
					 / path-empty
		path-abempty	 = *( "/" segment )
		path-absolute = "/" [ segment-nz *( "/" segment ) ]
		path-noscheme = segment-nz-nc *( "/" segment )
		path-rootless = segment-nz *( "/" segment )
		path-empty	 = 0<pchar>
		segment		 = *pchar
		pchar		 = unreserved / pct-encoded / sub-delims / ":" / "@"
	*/
	if (URIIsEndPath(*uri)
		/* When authority is present,*/
		/* the path must either be empty or begin with a slash ("/") character.*/
		/* When authority is not present,*/
		/* the path cannot begin with two slash characters ("//").*/
		|| (pThis->mHost.mLength ? (!(!*uri || *uri == '/')) :
							 (*uri == '/' && *(uri+1) == '/')))
	{
		WSStr_Erase(&pThis->mPath, 0);
		return uri;
	}
	uristart = uri;
	isAbsolute = *uri == '/';
	if (isAbsolute)
		uri++;
	/* segment-nz	 = 1*pchar*/
	while (!URIIsEndPath(*uri))
		uri++;
	pThis->mPath.mData = (char*)uristart;
	pThis->mPath.mLength = uri - uristart;
	pThis->mFields |= URI_PATH; /*mark the path as valid*/
	return uri;
}
const char* URI_ParseQuery(URI* pThis, const char* uri)
{
	const char* uristart;
	/* query		 = *( pchar / "/" / "?" )*/
	if (*uri == '?')
	{
		++uri;
		uristart = uri;
		while (*uri && *uri != '#')
			uri++;
		pThis->mQuery.mData = (char*)uristart;
		pThis->mQuery.mLength = uri - uristart;
		pThis->mFields |= URI_QUERY; /*mark the query as valid*/
	}
	else
		WSStr_Erase(&pThis->mQuery, 0);
	return uri;
}
const char* URI_ParseFragment(URI* pThis, const char* uri)
{
	const char* uristart;
	/* fragment		 = *( pchar / "/" / "?" )*/
	if (*uri == '#')
	{
		++uri;
		uristart = uri;
		while (*uri)
			uri++;
		pThis->mFragment.mData = (char*)uristart;
		pThis->mFragment.mLength = uri - uristart;
		pThis->mFields |= URI_FRAGMENT; /*mark the fragment as valid*/
	}
	else
		WSStr_Erase(&pThis->mFragment, 0);
	return uri;
}
wsbool URIParseH16(const char** uri)
{
	/* h16			 = 1*4HEXDIG*/
	if (!URIIsHex(*(*uri)))
		return FALSE;
	if (URIIsHex(*++(*uri)) && URIIsHex(*++(*uri)) && URIIsHex(*++(*uri)))
		++(*uri);
	return TRUE;
}
wsbool URIParseIPv4address(const char** uri)
{
	/*
		IPv4address	= dec-octet "." dec-octet "." dec-octet "." dec-octet

		dec-octet	=	   DIGIT					; 0-9
					  / %x31-39 DIGIT				; 10-99
					  / "1" 2DIGIT					; 100-199
					  / "2" %x30-34 DIGIT			; 200-249
					  / "25" %x30-35				; 250-255
	*/
	size_t iIPv4 = 0;
	const char* uriOrig = (*uri);
	if (URIIsDigit(*(*uri)))
	{
		++iIPv4;
		/* each ip part must be between 0-255 (dupe of version in for loop)*/
		if ((URIIsDigit(*++(*uri)) && URIIsDigit(*++(*uri)) &&
		   /* 100 or less  (note !)*/
		   !( (*((*uri)-2) == '1') ||
		   /* 240 or less*/
			 (*((*uri)-2) == '2' &&
			   (*((*uri)-1) < '5' || (*((*uri)-1) == '5' && *(*uri) <= '5'))
			 )
			)
		   ) || (((*uri) - uriOrig >= 2) && (*uriOrig == '0')) /* leading 0*/
		  ) { return FALSE; }
		if (URIIsDigit(*(*uri)))
			++(*uri);
		for (; iIPv4 < 4; ++iIPv4) /*compilers should unroll this loop*/
		{
			if (*(*uri) != '.' || !URIIsDigit(*++(*uri)))
				break;
			/*each ip part must be between 0-255*/
			uriOrig = (*uri);
			if ((URIIsDigit(*++(*uri)) && URIIsDigit(*++(*uri)) &&
			   /* 100 or less  (note !)*/
			   !( (*((*uri)-2) == '1') ||
			   /* 240 or less*/
				 (*((*uri)-2) == '2' &&
				   (*((*uri)-1) < '5'
						|| (*((*uri)-1) == '5' && *(*uri) <= '5'))
				 )
				)
			   ) || (((*uri) - uriOrig >= 2) && (*uriOrig == '0')) /* leading 0*/
			  ) { return FALSE; }
			if (URIIsDigit(*(*uri)))
				++(*uri);
		}
	}
	return iIPv4 == 4;
}
wsbool URIParseIPv6address(const char** uri)
{
	/*
		IPv6address	 =							  6( h16 ":" ) ls32
					 /						 "::" 5( h16 ":" ) ls32
					 / [			   h16 ] "::" 4( h16 ":" ) ls32
					 / [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32
					 / [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32
					 / [ *3( h16 ":" ) h16 ] "::"	 h16 ":"   ls32
					 / [ *4( h16 ":" ) h16 ] "::"			   ls32
					 / [ *5( h16 ":" ) h16 ] "::"			   h16
					 / [ *6( h16 ":" ) h16 ] "::"
	*/
	size_t leftHexpairs = 0, rightHexpairs, maxRightHexpairs;
	const char* uristart;
	wsbool doublecolon = FALSE;
	if (*(*uri) == ':' && *((*uri)+1) == ':')
		doublecolon = TRUE;
	else
	{
		uristart = (*uri);
		if (URIParseH16(&(*uri)))
		{
			++leftHexpairs;
			if (*(*uri) == ':' && *((*uri)+1) == ':')
				doublecolon = TRUE;
			else
			{
				for (;leftHexpairs < 7;)
				{ /* skip up to 6 leading [":" h16] pairs*/
					if (*(*uri) == ':')
					{
						++(*uri); /* skip over single colon*/
						uristart = (*uri);
						if (!URIParseH16(&(*uri))
							|| (*(*uri) != ':' && *(*uri)))
						{
							(*uri) = uristart;
							break;
						}
						++leftHexpairs;
					}
					else
						break;
					if (*(*uri) == ':' && *((*uri)+1) == ':')
					{
						doublecolon = TRUE;
						break;
					}
				} /* [":" h16] skipping loop*/
				if (!doublecolon && leftHexpairs == 7 && *(*uri) == ':')
					++(*uri); /* skip over single colon*/
			}/*leading h16 :: check*/
		}/*h16*/
		else
			(*uri) = uristart;
	}
	if (doublecolon)
	{
		(*uri) += 2;
		if (leftHexpairs < 5)
			maxRightHexpairs = 5 - leftHexpairs;
		else
			maxRightHexpairs = 0;
	}
	else
	{
		if (leftHexpairs < 6)
			maxRightHexpairs = 6 - leftHexpairs;
		else
			maxRightHexpairs = 0;
	}
	for (rightHexpairs = 0; rightHexpairs < maxRightHexpairs; ++rightHexpairs)
	{ /* skip up to 6 trailing [h16 ":"] pairs*/
		uristart = (*uri);
		if (!URIParseH16(&(*uri)) || *(*uri) != ':')
		{
			(*uri) = uristart;
			break;
		}
		++(*uri);
	}
	if (!doublecolon)
	{
		if (leftHexpairs < 6)
			return FALSE;
		rightHexpairs = leftHexpairs;
		leftHexpairs = 0;
	}
	uristart = (*uri);
	/* ls32 = ( h16 ":" h16 ) / IPv4address*/
	if (leftHexpairs < 6 && rightHexpairs < 7)
	{
		if (URIParseH16(&(*uri)) && *(*uri)++ == ':'
			&& URIParseH16(&(*uri)) && *(*uri) == ']')
			return TRUE;
		(*uri) = uristart;
		if (URIParseIPv4address(&(*uri)) && *(*uri) == ']')
			return TRUE;
		(*uri) = uristart;
	}
	/* final single h16 case or tail end of ending ls32*/
	if (leftHexpairs < 7 && (doublecolon || rightHexpairs == 7)
	   && URIParseH16(&(*uri)) && *(*uri) == ']')
		return TRUE;
	/* can only be empty if a "::" was detected*/
	return doublecolon && *(*uri) == ']';
}
wsbool URIParseIPvFuture(const char** uri)
{
	/* IPvFuture	 = "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" )*/
	if (*++(*uri) != 'v' || !URIIsHex(*++(*uri)))
		return FALSE;
	while (URIIsHex(*++(*uri))) {}
	if (*(*uri) != '.' || !(URIIsUnreserved(*++(*uri))
		|| URIIsSubDelim(*(*uri)) || *(*uri) == ':'))
		return FALSE;
	while (URIIsUnreserved(*++(*uri)) || URIIsSubDelim(*(*uri))
		|| *(*uri) == ':') {}
	return TRUE;
}
int URICharToHex(char c)
{
	if ((c >= 'A') && (c <= 'F'))
		return c - 'A' + 10;
	if ((c >= 'a') && (c <= 'f'))
		return c - 'a' + 10;
	if ((c >= '0') && (c <= '9'))
		return c - '0';
	return -1;
}
wsbool URIIsUnreserved (char c)
{ /* unreserved	  = ALPHA / DIGIT / "-" / "." / "_" / "~"*/
	return URIIsAlpha(c) || URIIsDigit(c) ||
		   c == '-' || c == '.' ||
		   c == '_' || c == '~';
}
wsbool URIIsReserved (char c)
{ return URIIsGenDelim(c) || URIIsSubDelim(c); }
/* gen-delims	  = ":" / "/" / "?" / "#" / "[" / "]" / "@"*/
wsbool URIIsGenDelim (char c)
{
	return c == ':' || c == '/' || c == '?' || c == '#' ||
		   c == '[' || c == ']' || c == '@';
}
/*! sub-delims	  = "!" / "$" / "&" / "'" / "(" / ")"*/
/*!				  / "*" / "+" / "," / ";" / "="*/
wsbool URIIsSubDelim (char c)
{
	return c == '!' || c == '$' || c == '&' || c == '\'' ||
		   c == '(' || c == ')' || c == '*' || c == '+' ||
		   c == ',' || c == ';' || c == '=';
}
wsbool URIIsHex(char c)
{ return URIIsDigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }
wsbool URIIsAlpha(char c)
{ return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');	}
wsbool URIIsDigit(char c)
{ return c >= '0' && c <= '9'; }
wsbool URIIsEndPath(char c)
{ return c == '\0' || c == '#' || c == '?'; }
/* pchar		 = unreserved / pct-encoded / sub-delims / ":" / "@"*/
/* pct-encoded handled outside, NC is for no colon for relative paths*/
wsbool URIIsPCharNCNE(char c)
{ return URIIsUnreserved(c) || URIIsSubDelim(c) || c == '@' || c == '/'; }
wsbool URIIsPCharNE(char c)
{ return URIIsPCharNCNE(c) || c == ':'; }
#if defined(__cplusplus)
}
#endif
/*============================================================================
								MutUri
=============================================================================*/
#if defined(WSMUTURIOPT) && 0
MutUri::MutUri() : mFields(0), mHostType(Uri::REGNAME) { }
MutUri::MutUri(const MutStr& uri) : mFields(0), mHostType(Uri::REGNAME)
{ Create(uri); }
void MutUri::Destroy()
{
	mScheme.resize(0); mUserInfo.resize(0); mHost.resize(0); mPort.resize(0);
	mPath.resize(0); mQuery.resize(0); mFragment.resize(0);
	mHostType = Uri::REGNAME;
	mFields = 0;
}
bool MutUri::Create(const MutStr& uri)
{
	if (mFields)
		Destroy();
	return Parse(uri.c_str());
}
MutStr MutUri::GetUser() const
{
	// no ':' == whole string, as the password was omitted this is valid
	return mUserInfo.substr(0, mUserInfo.find(':'));
}
MutStr MutUri::GetPassword() const
{
	size_t dwPasswordPos = mUserInfo.find(':');
	if (dwPasswordPos == WSMAXLENGTH)
		return MutStr();
	return mUserInfo.substr(dwPasswordPos + 1, mUserInfo.length() + 1);
}
bool MutUri::IsValidHostname(Uri::Strictness strict) const
{
	const char* s = mHost.c_str(); const char* labelstart = s;
	if (mHostType != Uri::REGNAME) // can't be an unresolved ip address
		return FALSE;
	for (;*s;++s)
	{
		if (s == labelstart)
		{
			//a-z, A-Z, 0-9
			//Newer RFC 1123 allows leading digit, RFC 952 doesn't
			if (Uri::IsDigit(*s) && strict == Uri::NONSTRICT)
				return FALSE;
			if ((Uri::IsAlpha(*s) || Uri::IsDigit(*s)) == FALSE)
				return FALSE;
		}
		else if (*s == '.')
		{
			if (s == labelstart) // nothing in the label
				return FALSE;
			if (*(s-1) == '-')	// can't end with hyphen
				return FALSE;
			labelstart = s;
			++labelstart;
		}
		else if ((Uri::IsAlpha(*s) || Uri::IsDigit(*s) || '-') == FALSE)
			return FALSE; // not a valid char: a-z, A-Z, 0-9, -
	}
	if (*(s-1) == '.' && (s-1) == labelstart)
		return FALSE; // last label is empty (ends with ..)
	return TRUE;
}
MutStr MutUri::Recompose(RECOMPOSEPROC decodeproc) const
{
	MutStr ret;
	if (HasScheme())
		ret << mScheme << ":";
	if (HasHost())
	{
		ret << "//";
		if (HasUserInfo())
			ret << (*decodeproc)(mUserInfo) << "@";
		if (mHostType == Uri::REGNAME)
			ret << (*decodeproc)(mHost);
		else
			ret << mHost;
		if (HasPort())
			ret << ":" << mPort;
	}
	ret += (*decodeproc)(mPath);
	if (HasQuery())
		ret << "?" << (*decodeproc)(mQuery);
	if (HasFragment())
		ret << "#" << (*decodeproc)(mFragment);
	return StringReference(ret);
}
bool MutUri::operator == (const MutUri& uri) const
{
	if (HasScheme())
	{
		if (mScheme != uri.mScheme)
			return FALSE;
	}
	else if (uri.HasScheme())
		return FALSE;
	if (HasHost())
	{
		if (HasUserInfo())
		{
			if (mUserInfo != uri.mUserInfo)
				return FALSE;
		}
		else if (uri.HasUserInfo())
			return FALSE;
		if (mHost != uri.mHost || mHostType != uri.mHostType)
			return FALSE;
		if (HasPort())
		{
			if (mPort != uri.mPort)
				return FALSE;
		}
		else if (uri.HasPort())
			return FALSE;
	}
	else if (uri.HasHost())
		return FALSE;
	if (HasPath())
	{
		if (mPath != uri.mPath)
			return FALSE;
	}
	else if (uri.HasPath())
		return FALSE;
	if (HasQuery())
	{
		if (mQuery != uri.mQuery)
			return FALSE;
	}
	else if (uri.HasQuery())
		return FALSE;
	if (HasFragment())
	{
		if (mFragment != uri.mFragment)
			return FALSE;
	}
	else if (uri.HasFragment())
		return FALSE;
	return TRUE;
}
bool MutUri::Parse(const char* uri)
{
	uri = ParseScheme(uri);
	if (uri)
		uri = ParseAuthority(uri);
	if (uri)
		uri = ParsePath(uri);
	if (uri)
		uri = ParseQuery(uri);
	if (uri)
		uri = ParseFragment(uri);
	return uri && *uri == '\0'; // succeeds only if it parsed the whole string
}
const char* MutUri::ParseScheme(const char* uri)
{
	const char* const uristart = uri;
	//scheme		= ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
	if (Uri::IsAlpha(*uri))
	{
		mScheme += *uri++;
		while (Uri::IsAlpha(*uri) || Uri::IsDigit(*uri) ||
			   *uri == '+' || *uri == '-' || *uri == '.')
			mScheme += *uri++;
		if (*uri == ':') // valid scheme?
		{
			mFields |= Uri::SCHEME; //mark the scheme as valid
			++uri;
		}
		else // no valid scheme
		{
			uri = uristart; // backtrack
			mScheme.erase();
		}
	} // else no scheme, _possible_ relative MutUri
	return uri;
}
const char* MutUri::ParseAuthority(const char* uri)
{
	// URI			 = scheme ":" hier-part [ "?" query ] [ "#" fragment ]
	// hier-part	 = "//" authority path-abempty
	bool leadswithdoubleslash = uri[0] == '/' && uri[1] == '/';
	if(leadswithdoubleslash)
	{
		//skip past the two slashes
		uri += 2;
	}

	// ############# DEVIATION FROM RFC #########################
	// Evidence shows us that users expect us to parse the
	// server component in the case of [server][#fragment].
	// Otherwise they don't want us to parse the server at all
	// for file uris. RFC 8089 allows for leading "//" with file.
	if (AnsiiCompareICase(mScheme.c_str(), "file",
		mScheme.length(), strcountof("file")) == 0)
	{ // look for [server][#fragment] case
		bool serverfragmentfound = FALSE;
		const char* uristart = uri;
		uri = ParseHost(uri);
		if(HasHost())
		{
			uri = ParseFragment(uri);
			serverfragmentfound = HasFragment();
		}
		if(!serverfragmentfound)
		{ // nope... clear fields, return to point after opening slashes
			uri = uristart;
			mHost.erase();
			mFields &= ~Uri::HOST;
		}
	}
	else if(leadswithdoubleslash)
	{ // normal RFC-based authority parsing
		// authority	 = [ userinfo "@" ] host [ ":" port ]
		// file-hier-part = ( "//" auth-path ) / local-path
		const char* uristart = uri;
		uri = ParseUserInfo(uri);
		uri = ParseHost(uri);
		if (!HasHost() && HasUserInfo()) // in practice this case doesn't happen without scheme-based parsing
		{ // userinfo can't exist without host, backtrack and reparse
			mUserInfo.erase();
			mFields &= ~Uri::USERINFO;
			uri = uristart;
			uri = ParseHost(uri);
		}
		uri = ParsePort(uri);
		if (!HasHost()) // in practice this case doesn't happen without scheme-based parsing
		{
			mPort.erase(); // port can't exist without host
			mFields &= ~Uri::PORT;
			uri = uristart - 2; // nothing found, skip to before the leading "//"
		}
	}

	return uri;
}
const char* MutUri::ParseUserInfo(const char* uri)
{
	const char* const uristart = uri;
	// userinfo		 = *( unreserved / pct-encoded / sub-delims / ":" )
	while (*uri && *uri != '@' && *uri != '/' && *uri != '#' && *uri != '?')
	{
		if (Uri::IsUnreserved(*uri) || Uri::IsSubDelim(*uri) || *uri == ':')
			mUserInfo += *uri++;
		else
			Uri::Escape(mUserInfo, uri);
	}
	if (*uri++ == '@')
		mFields |= Uri::USERINFO; //valid userinfo
	else
	{
		uri = uristart; // backtrack
		mUserInfo.erase();
	}
	return uri;
}
const char* MutUri::ParseHost(const char* uri)
{
	const char* uristart = uri;
	// host			 = IP-literal / IPv4address / reg-name
	// IP-literal	 = "[" ( IPv6address / IPvFuture  ) "]"
	if (*uri == '[')
	{
		++uri;
		if (Uri::ParseIPv6address(uri) && *uri == ']')
		{
			mHostType = Uri::IPV6ADDRESS;
			mHost.append(uristart + 1, uri - uristart - 1);
			++uri;
		}
		else
		{
			uri = uristart + 1; // skip over '['
			if (Uri::ParseIPvFuture(uri) && *uri == ']')
			{
				mHostType = Uri::IPVFUTURE;
				mHost.append(uristart + 1, uri - uristart - 1);
				++uri;
			}
			else // backtrack due to it not being recognized, IP literal
				uri = uristart;
		}
	}
	else // IPv4address / reg-name
	{
		if (Uri::ParseIPv4address(uri) && (!*uri || *uri == '/' || *uri == ':'))
		{
			mHostType = Uri::IPV4ADDRESS;
			mHost.append(uristart, uri - uristart);
		}
		else // backtrack
			uri = uristart;
	}
	if (uri == uristart) // reg-name
	{
		uri = uristart; // backtrack
		// reg-name		 = *( unreserved / pct-encoded / sub-delims )
		while (*uri && *uri != '/' && *uri != ':' && *uri != '#' && *uri != '?')
		{
			if (Uri::IsUnreserved(*uri) || Uri::IsSubDelim(*uri))
				mHost += *uri++;
			else
				Uri::Escape(mHost, uri);
		}
	}
	mFields |= Uri::HOST; // reg-name can be empty
	return uri;
}
const char* MutUri::ParsePort(const char* uri)
{
	//[ ":" port ], port = *DIGIT
	if (*uri == ':')
	{
		++uri;
		while (Uri::IsDigit(*uri))
			mPort += *uri++;
		mFields |= Uri::PORT; //mark the port as valid
	}
	return uri;
}
void MutUri::Normalize() // 6.2.2.	 Syntax-Based Normalization
{
	// Normalizes directories in-place; I.E. ./ and . are ignored, and relevant
	// ../ and .. are removed along with the directory before them if said directory exists.
	char* s = (char*)mPath.c_str(); char* cp = s; char* bp = s;
	if (!HasPath())
		return;
	while (*cp)
	{
		if (*cp == '.' && (*(cp+1) == '/' || *(cp+1) == '\0')
			&& (cp == mPath.c_str() || !Uri::IsPCharNE(*(cp-1)) || *(cp-1) == '/'))
		{
			//. _or_ ./ - remove (6.2.2.3. Path Segment Normalization)
			if (*(cp+1) == '\0')
				cp += 1;
			else
				cp += 2;
		}
		else if (*cp == '.' && *(cp+1) == '.' && (*(cp+2) == '/' || *(cp+2) == '\0')
				 && (cp == mPath.c_str() || !Uri::IsPCharNE(*(cp-1)) || *(cp-1) == '/'))
		{
			//.. _or_ ../ - remove as well as previous path if it exists (6.2.2.3)
			Uri::UpTree((const char*)bp, (const char*&)s);
			if (*(cp+2) == '\0')
				cp += 2;
			else
				cp += 3;
		}
		else if (*cp == '%' && Uri::CharToHex(*(cp+1)) != -1
							&& Uri::CharToHex(*(cp+2)) != -1)
			Uri::NormalizeEncoded(s, cp);
		else
			*s++ = *cp++;
	}
	mPath.erase(s - mPath.c_str()); // *s = '\0';
}
const char* MutUri::ParsePath(const char* uri)
{
	// hier-part	 = "//" authority path-abempty
	//				 / path-absolute
	//				 / path-rootless
	//				 / path-empty
	// relative-part = "//" authority path-abempty
	//				 / path-absolute
	//				 / path-noscheme
	//				 / path-empty
	// path-abempty	 = *( "/" segment )
	// path-absolute = "/" [ segment-nz *( "/" segment ) ]
	// path-noscheme = segment-nz-nc *( "/" segment )
	// path-rootless = segment-nz *( "/" segment )
	// path-empty	 = 0<pchar>
	// segment		 = *pchar
	// pchar		 = unreserved / pct-encoded / sub-delims / ":" / "@"
	if (Uri::IsEndPath(*uri)
		// When authority is present,
		// the path must either be empty or begin with a slash ("/") character.
		// When authority is not present,
		// the path cannot begin with two slash characters ("//").
		|| (mHost.length() ? (!(!*uri || *uri == '/')) : (*uri == '/' && *(uri+1) == '/')))
		return uri;
	const char* uristart = uri;
	const bool isAbsolute = *uri == '/';
	if (isAbsolute)
	{
		mPath += *uri++;
		// segment-nz	 = 1*pchar
		while (!Uri::IsEndPath(*uri))
		{
			if (Uri::IsPCharNE(*uri) || *uri == '/')
				mPath += *uri++;
			else
				Uri::Escape(mPath, uri);
		}
		mFields |= Uri::PATH; //mark the path as valid
	}
	else if (*uri) //Relative path
	{
		if (!HasScheme())
		{
			// segment-nz-nc = 1*( unreserved / pct-encoded / sub-delims / "@" )
			//				 ; non-zero-length segment without any colon ":"
			while (!Uri::IsEndPath(*uri))
			{
				if (Uri::IsPCharNCNE(*uri) || *uri == '/')
					mPath += *uri++;
				else
					Uri::Escape(mPath, uri);
			}
		}
		else
		{
			// segment-nz	 = 1*pchar
			while (!Uri::IsEndPath(*uri))
			{
				if (Uri::IsPCharNE(*uri) || *uri == '/')
					mPath += *uri++;
				else
					Uri::Escape(mPath, uri);
			}
		}
		if (uri != uristart)
			mFields |= Uri::PATH; // mark the path as valid
	}
	return uri;
}
const char* MutUri::ParseQuery(const char* uri)
{
	// query		 = *( pchar / "/" / "?" )
	if (*uri == '?')
	{
		++uri;
		while (*uri && *uri != '#')
		{
			if (Uri::IsPCharNE(*uri) || *uri == '/' || *uri == '?')
				  mQuery += *uri++;
			else
				  Uri::Escape(mQuery, uri);
		}
		mFields |= Uri::QUERY; //mark the query as valid
	}
	return uri;
}
const char* MutUri::ParseFragment(const char* uri)
{
	// fragment		 = *( pchar / "/" / "?" )
	if (*uri == '#')
	{
		++uri;
		while (*uri)
		{
			if (Uri::IsPCharNE(*uri) || *uri == '/' || *uri == '?')
				  mFragment += *uri++;
			else
				  Uri::Escape(mFragment, uri);
		}
		mFields |= Uri::FRAGMENT; //mark the fragment as valid
	}
	return uri;
}
void MutUri::Resolve(const MutUri& base, Uri::Strictness strict)
{
	WSASSERT_MSG(!base.IsRelative(),
				"MutUri to inherit from must not be a reference!");
	// If we aren't being strict, enable the older (pre-RFC2396)
	// loophole that allows this uri to inherit other
	// properties from the base uri - even if the scheme
	// is defined
	if (strict == Uri::NONSTRICT &&
		HasScheme() && base.HasScheme() &&
		mScheme == base.mScheme )
		mFields &= ~Uri::SCHEME;

	// Do nothing if this is an absolute MutUri
	//	  if defined(R.scheme) then
	//		 T.scheme	 = R.scheme;
	//		 T.authority = R.authority;
	//		 T.path		 = remove_dot_segments(R.path);
	//		 T.query	 = R.query;
	if (HasScheme())
	{
		Normalize();
		return;
	}
	//No sheme - inherit
	mScheme = base.mScheme;
	mFields |= Uri::SCHEME;
	// All we need to do for relative URIs with an
	// authority component is just inherit the scheme
	//		 if defined(R.authority) then
	//			T.authority = R.authority;
	//			T.path		= remove_dot_segments(R.path);
	//			T.query		= R.query;
	if (HasHost())
	{
		Normalize();
		return;
	}
	//No authority - inherit
	if (base.HasUserInfo())
	{
		mUserInfo = base.mUserInfo;
		mFields |= Uri::USERINFO;
	}
	mHost = base.mHost;
	mHostType = base.mHostType;
	mFields |= Uri::HOST;
	if (base.HasPort())
	{
		mPort = base.mPort;
		mFields |= Uri::PORT;
	}
	// if (R.path == "") then
	//	   T.path = Base.path;
	//	   if defined(R.query) then
	//		   T.query = R.query;
	//	   else
	//		   T.query = Base.query;
	//	   endif;
	if (!HasPath() || mPath.length() == 0)
	{
		mPath = base.mPath;
		mFields |= Uri::PATH;
		if (!HasQuery())
		{
			mQuery = base.mQuery;
			mFields |= Uri::QUERY;
		}
	}
	// else
	//	   if (R.path starts-with "/") then
	//		   T.path = remove_dot_segments(R.path);
	//	   else
	//		   T.path = merge(Base.path, R.path);
	//		   T.path = remove_dot_segments(T.path);
	//	   endif;
	//	   T.query = R.query;
	// endif;
	else if (mPath[0] != '/')
	{
#if 1
		//Marge paths
		MutUri normalizedbase(base);
		normalizedbase.Normalize();
		const char* bp = normalizedbase.mPath.c_str() + normalizedbase.mPath.length();
		//not a ending directory?  move up
		if (normalizedbase.HasPath() && normalizedbase.mPath[0] && *(bp-1) != '/')
			Uri::UpTree(normalizedbase.mPath.c_str(), bp);
		mPath = normalizedbase.mPath.substr(0, bp - normalizedbase.mPath.c_str()) + mPath;
#else
		//Marge paths
		const char* bp = base.mPath.c_str() + base.mPath.length();
		//not a ending directory?  move up
		if (base.mPath[0] && *(bp-1) != '/')
			Uri::UpTree(base.mPath, bp);
		//normalize directories
		while (*op == '.' && *(op+1) == '.' && (*(op+2) == '\0' || *(op+2) == '/') )
		{
			Uri::UpTree(base.mPath, bp);
			if (*(op+2) == '\0')
				op += 2;
			else
				op += 3;
		}
		mPath = base.mPath.substr(0, bp - base.mPath.c_str()) +
				mPath.substr((op - mPath.c_str()), mPath.length() - (op - mPath.c_str()));
#endif
	}
	//T.fragment = R.fragment;
	Normalize();
}
#endif // WSMUTURIOPT

/*-----------------------------------------------------------------------------
								[Unit Tests]
-----------------------------------------------------------------------------*/
#if defined(WSTESTOPT)
#include "test.h"
#if defined(__cplusplus)
extern "C" {
#endif
wsbool Uri_HostCmp(const URI* pThis, const char* s)
 { return UriPartCmp(&URI_GetHost(pThis), s); }
#define WSTEST_ASSERT_EQUAL(x,y) WSTEST_ASSERT((x)==(y))

/* apply the given accessor to the URI, check that the result is as expected*/
#define URI_ASSERT_PART_EQUAL(uri, expected, accessor) \
	{	URI htest; WSSTR* part; URI_Create(&htest, uri); \
		part = &URI_##accessor(&htest); \
		WSTEST_ASSERT(UriPartCmp(part, expected)); }
#define URI_ASSERT_BUILDURI_EQUAL(uri, expected) \
	{	URI htest; char* part; size_t partlen, exlen; \
		exlen = CStrLength(expected); URI_Create(&htest, uri); \
		URI_BuildURI(&htest, &part, &partlen); \
		WSTEST_ASSERT(partlen == exlen \
			&& WSMemoryCompare(part, (expected), \
							  partlen * sizeof(char)) == 0); \
		WSFREE(part); \
	}
#define URI_ASSERT_HOST_TEST(uri, expectedhost, expectedtype) \
	{	URI htest; URI_Create(&htest, uri); \
		WSTEST_ASSERT(((Uri_HostCmp(&htest, (expectedhost))) \
			&& (URI_GetHostType(&htest) == (expectedtype)))); }
#define URI_ASSERT_HOST_TESTBAD(uri, ne) \
	{	URI htest; URI_Create(&htest, uri); \
		WSTEST_ASSERT(URI_GetHostType(&htest) != (ne)); }
#define URI_ASSERT_HOSTTYPE_EQUAL(uri, expected) \
	{	URI htest; URI_Create(&htest, uri); \
		WSTEST_ASSERT(URI_GetHostType(&htest) == (expected)); }
#define URI_ASSERT_HOST_EQUAL(uri, expected) \
	URI_ASSERT_PART_EQUAL((uri), (expected), GetHost)
#define URI_ASSERT_PATH_EQUAL(uri, expected) \
	URI_ASSERT_PART_EQUAL((uri), (expected), GetPath)
#define URI_ASSERT_USER_EQUAL(uri, expected) \
	{	URI htest; WSSTR part; URI_Create(&htest, uri); \
		URI_GetUser(&htest, &part); \
		WSTEST_ASSERT(UriPartCmp(&part, expected)); }
#define URI_ASSERT_BADPATH(uri) { URI huri; URI_Create(&huri, uri); \
	WSTEST_ASSERT(!URI_HasPath(&huri)); }
#define URI_ASSERT_IPV4_TEST(ip, expected) \
	URI_ASSERT_HOST_TEST("http://user:password@" ip ":5050/path", expected, URI_IPV4ADDRESS)
#define URI_ASSERT_IPV4_TESTBAD(ip) \
	URI_ASSERT_HOST_TESTBAD("http://user:password@" ip ":5050/path", URI_IPV4ADDRESS)

WSTEST_FUNCTION(IPv4)
{
	URI_ASSERT_IPV4_TEST("192.168.1.100", "192.168.1.100");
	URI_ASSERT_IPV4_TEST("192.255.1.100", "192.255.1.100");
	URI_ASSERT_IPV4_TEST("192.0.2.16", "192.0.2.16");
	URI_ASSERT_IPV4_TEST("255.0.0.0", "255.0.0.0");
	URI_ASSERT_IPV4_TEST("0.0.0.0", "0.0.0.0");
	URI_ASSERT_IPV4_TEST("1.0.0.0", "1.0.0.0");
	URI_ASSERT_IPV4_TEST("2.0.0.0", "2.0.0.0");
	URI_ASSERT_IPV4_TEST("3.0.0.0", "3.0.0.0");
	URI_ASSERT_IPV4_TEST("30.0.0.0", "30.0.0.0");
	URI_ASSERT_IPV4_TESTBAD("192.256.1.100");
	URI_ASSERT_IPV4_TESTBAD("01.0.0.0");
	URI_ASSERT_IPV4_TESTBAD("001.0.0.0");
	URI_ASSERT_IPV4_TESTBAD("00.0.0.0");
	URI_ASSERT_IPV4_TESTBAD("000.0.0.0");
	URI_ASSERT_IPV4_TESTBAD("256.0.0.0");
	URI_ASSERT_IPV4_TESTBAD("300.0.0.0");
	URI_ASSERT_IPV4_TESTBAD("1111.0.0.0");
	URI_ASSERT_IPV4_TESTBAD("-1.0.0.0");
	URI_ASSERT_IPV4_TESTBAD("0.0.0");
	URI_ASSERT_IPV4_TESTBAD("0.0.0.");
	URI_ASSERT_IPV4_TESTBAD("0.0.0.0.");
	URI_ASSERT_IPV4_TESTBAD("0.0.0.0.0");
	URI_ASSERT_IPV4_TESTBAD("0.0..0");
	URI_ASSERT_IPV4_TESTBAD(".0.0.0");
	URI_ASSERT_HOSTTYPE_EQUAL("http://user:password@192.168.1.100:5050/path",
							URI_IPV4ADDRESS);
	URI_ASSERT_HOSTTYPE_EQUAL("http://user:password@192.255.1.100:5050/path",
							URI_IPV4ADDRESS);
}
#define URI_ASSERT_IPV6_TEST(ip, expected) \
	URI_ASSERT_HOST_TEST("http://user:password@" ip ":5050/path", expected, URI_IPV6ADDRESS)
#define URI_ASSERT_IPV6_TESTBAD(ip) \
	URI_ASSERT_HOST_TESTBAD("http://user:password@" ip ":5050/path", URI_IPV6ADDRESS)
WSTEST_FUNCTION(IPv6)
{
	URI_ASSERT_HOSTTYPE_EQUAL
	(
		"http://user:password@[aa:aa:aa:aa:aa:aa:192.168.1.100]:5050/path",
		URI_IPV6ADDRESS
	);

	URI_ASSERT_HOSTTYPE_EQUAL
	(
		"http://user:password@[aa:aa:aa:aa:aa:aa:aa:aa]:5050/path",
		URI_IPV6ADDRESS
	);

	URI_ASSERT_HOSTTYPE_EQUAL
	(
		"http://user:password@[aa:aa:aa:aa::192.168.1.100]:5050/path",
		URI_IPV6ADDRESS
	);

	URI_ASSERT_HOSTTYPE_EQUAL
	(
		"http://user:password@[aa:aa:aa:aa::aa:aa]:5050/path",
		URI_IPV6ADDRESS
	);
	URI_ASSERT_IPV6_TEST("[aa:aa:aa:aa:aa:aa:192.168.1.100]", "aa:aa:aa:aa:aa:aa:192.168.1.100");
	URI_ASSERT_IPV6_TEST("[aa:aa:aa:aa:aa:aa:aa:aa]", "aa:aa:aa:aa:aa:aa:aa:aa");
	URI_ASSERT_IPV6_TEST("[aa:aa:aa:aa::192.168.1.100]", "aa:aa:aa:aa::192.168.1.100");
	URI_ASSERT_IPV6_TEST("[aa:aa:aa:aa::aa:aa]", "aa:aa:aa:aa::aa:aa");
	URI_ASSERT_IPV6_TEST("[FEDC:BA98:7654:3210:FEDC:BA98:7654:3210]", "FEDC:BA98:7654:3210:FEDC:BA98:7654:3210");
	URI_ASSERT_IPV6_TEST("[1080:0:0:0:8:800:200C:417A]", "1080:0:0:0:8:800:200C:417A");
	URI_ASSERT_IPV6_TEST("[3ffe:2a00:100:7031::1]", "3ffe:2a00:100:7031::1");
	URI_ASSERT_IPV6_TEST("[1080::8:800:200C:417A]", "1080::8:800:200C:417A");
	URI_ASSERT_IPV6_TEST("[::192.9.5.5]", "::192.9.5.5");
	URI_ASSERT_IPV6_TEST("[::FFFF:129.144.52.38]", "::FFFF:129.144.52.38");
	URI_ASSERT_IPV6_TEST("[2010:836B:4179::836B:4179]", "2010:836B:4179::836B:4179");
	URI_ASSERT_IPV6_TEST("[abcd::]", "abcd::");
	URI_ASSERT_IPV6_TEST("[abcd::1]", "abcd::1");
	URI_ASSERT_IPV6_TEST("[abcd::12]", "abcd::12");
	URI_ASSERT_IPV6_TEST("[abcd::123]", "abcd::123");
	URI_ASSERT_IPV6_TEST("[abcd::1234]", "abcd::1234");
	URI_ASSERT_IPV6_TEST("[2001:0db8:0100:f101:0210:a4ff:fee3:9566]", "2001:0db8:0100:f101:0210:a4ff:fee3:9566");
	URI_ASSERT_IPV6_TEST("[2001:0DB8:0100:F101:0210:A4FF:FEE3:9566]", "2001:0DB8:0100:F101:0210:A4FF:FEE3:9566");
	URI_ASSERT_IPV6_TEST("[2001:db8:100:f101:210:a4ff:fee3:9566]", "2001:db8:100:f101:210:a4ff:fee3:9566");
	URI_ASSERT_IPV6_TEST("[2001:0db8:100:f101:0:0:0:1]","2001:0db8:100:f101:0:0:0:1");
	URI_ASSERT_IPV6_TEST("[1:2:3:4:5:6:255.255.255.255]","1:2:3:4:5:6:255.255.255.255");
	URI_ASSERT_IPV6_TEST("[::1.2.3.4]", "::1.2.3.4");
	URI_ASSERT_IPV6_TEST("[3:4::5:1.2.3.4]", "3:4::5:1.2.3.4");
	URI_ASSERT_IPV6_TEST("[::ffff:1.2.3.4]", "::ffff:1.2.3.4");
	URI_ASSERT_IPV6_TEST("[::0.0.0.0]", "::0.0.0.0");
	URI_ASSERT_IPV6_TEST("[::255.255.255.255]", "::255.255.255.255");
	URI_ASSERT_IPV6_TEST("[::1:2:3:4:5:6:7]", "::1:2:3:4:5:6:7");
	URI_ASSERT_IPV6_TEST("[1::1:2:3:4:5:6]", "1::1:2:3:4:5:6");
	URI_ASSERT_IPV6_TEST("[1:2::1:2:3:4:5]", "1:2::1:2:3:4:5");
	URI_ASSERT_IPV6_TEST("[1:2:3::1:2:3:4]", "1:2:3::1:2:3:4");
	URI_ASSERT_IPV6_TEST("[1:2:3:4::1:2:3]", "1:2:3:4::1:2:3");
	URI_ASSERT_IPV6_TEST("[1:2:3:4:5::1:2]", "1:2:3:4:5::1:2");
	URI_ASSERT_IPV6_TEST("[1:2:3:4:5:6::1]", "1:2:3:4:5:6::1");
	URI_ASSERT_IPV6_TEST("[1:2:3:4:5:6:7::]", "1:2:3:4:5:6:7::");
	URI_ASSERT_IPV6_TEST("[1:1:1::1:1:1:1]", "1:1:1::1:1:1:1");
	URI_ASSERT_IPV6_TEST("[1:1:1::1:1:1]", "1:1:1::1:1:1");
	URI_ASSERT_IPV6_TEST("[1:1:1::1:1]", "1:1:1::1:1");
	URI_ASSERT_IPV6_TEST("[1:1::1:1]", "1:1::1:1");
	URI_ASSERT_IPV6_TEST("[1:1::1]", "1:1::1");
	URI_ASSERT_IPV6_TEST("[1::1]", "1::1");
	URI_ASSERT_IPV6_TEST("[::1]", "::1");
	URI_ASSERT_IPV6_TEST("[::]", "::");
	URI_ASSERT_IPV6_TEST("[21ff:abcd::1]", "21ff:abcd::1");
	URI_ASSERT_IPV6_TEST("[2001:db8:100:f101::1]", "2001:db8:100:f101::1");
	URI_ASSERT_IPV6_TEST("[a:b:c::12:1]", "a:b:c::12:1");
	URI_ASSERT_IPV6_TEST("[a:b::0:1:2:3]", "a:b::0:1:2:3");
	URI_ASSERT_IPV6_TESTBAD("[::12345]");
	URI_ASSERT_IPV6_TESTBAD("[abcd::abcd::abcd]");
	URI_ASSERT_IPV6_TESTBAD("[:::1234]");
	URI_ASSERT_IPV6_TESTBAD("[1234:::1234:1234]");
	URI_ASSERT_IPV6_TESTBAD("[1234:1234:::1234]");
	URI_ASSERT_IPV6_TESTBAD("[1234:::]");
	URI_ASSERT_IPV6_TESTBAD("[1.2.3.4]");
	URI_ASSERT_IPV6_TESTBAD("[0001.0002.0003.0004]");
	URI_ASSERT_IPV6_TESTBAD("[0000:0000:0000:0000:0000:1.2.3.4]");
	URI_ASSERT_IPV6_TESTBAD("[0:0:0:0:0:0:0]");
	URI_ASSERT_IPV6_TESTBAD("[0:0:0:0:0:0:0:]");
	URI_ASSERT_IPV6_TESTBAD("[0:0:0:0:0:0:0:1.2.3.4]");
	URI_ASSERT_IPV6_TESTBAD("[0:0:0:0:0:0:0:0:0]");
	URI_ASSERT_IPV6_TESTBAD("[::ffff:001.02.03.004]");
	URI_ASSERT_IPV6_TESTBAD("[::ffff:1.2.3.1111]");
	URI_ASSERT_IPV6_TESTBAD("[::ffff:1.2.3.256]");
	URI_ASSERT_IPV6_TESTBAD("[::ffff:311.2.3.4]");
	URI_ASSERT_IPV6_TESTBAD("[::ffff:1.2.3:4]");
	URI_ASSERT_IPV6_TESTBAD("[::ffff:1.2.3]");
	URI_ASSERT_IPV6_TESTBAD("[::ffff:1.2.3.]");
	URI_ASSERT_IPV6_TESTBAD("[::ffff:1.2.3a.4]");
	URI_ASSERT_IPV6_TESTBAD("[::ffff:1.2.3.4:123]");
	URI_ASSERT_IPV6_TESTBAD("[g:0:0:0:0:0:0]");
}
WSTEST_FUNCTION(Host)
{
	URI_ASSERT_HOST_EQUAL("", "");
	URI_ASSERT_HOST_EQUAL("http://foo/", "foo");
	URI_ASSERT_HOST_EQUAL("http://foo-bar/", "foo-bar");
	URI_ASSERT_HOST_EQUAL("http://foo/bar/", "foo");
	URI_ASSERT_HOST_EQUAL("http://192.168.1.0/", "192.168.1.0");
	URI_ASSERT_HOST_EQUAL("http://192.168.1.17/", "192.168.1.17");
	URI_ASSERT_HOST_EQUAL("http://192.168.1.255/", "192.168.1.255");
	URI_ASSERT_HOST_EQUAL("http://192.168.1.1/index.html", "192.168.1.1");
	URI_ASSERT_HOST_EQUAL("http://[aa:aa:aa:aa::aa:aa]/foo", "aa:aa:aa:aa::aa:aa");
	URI_ASSERT_USER_EQUAL("http://user:pass@host/path/", "user");
	URI_ASSERT_USER_EQUAL("http://user@host/path/", "user");
	URI_ASSERT_USER_EQUAL("http://host/path/", "");
}
WSTEST_FUNCTION(Paths)
{
	URI test;
	URI_Create(&test, ("http://user:password@192.256.1.100:5050/../path"));
	URI_ASSERT_PATH_EQUAL("http://user:password@192.256.1.100:5050/../path", "/../path");
	URI_ASSERT_PATH_EQUAL("http://user:password@192.256.1.100:5050/path/../", "/path/../");
	URI_ASSERT_PATH_EQUAL("http://user:password@192.256.1.100:5050/path/.", "/path/.");
	URI_ASSERT_PATH_EQUAL("http://user:password@192.256.1.100:5050/path/./", "/path/./");
	{	URI htest; char* part; size_t partlen, exlen;
		exlen = CStrLength("path/john/../../../joe");
		URI_Create(&htest, "path/john/../../../joe");
		URI_BuildURI(&htest, &part, &partlen);
		WSTEST_ASSERT(partlen == exlen
			&& WSMemoryCompare(part, ("path/john/../../../joe"),
							  partlen * sizeof(char)) == 0);
		WSFREE(part);
	}
	URI_ASSERT_BUILDURI_EQUAL("path/john/../../../joe", "path/john/../../../joe");
	/*When authority is present, the path must either be empty or begin with a slash ("/") character.*/
	URI_ASSERT_BADPATH("http://good.com:8042BADPATH");
	URI_ASSERT_PATH_EQUAL("http://good.com:8042/GOODPATH", "/GOODPATH");
	/*When authority is not present, the path cannot begin with two slash characters ("//").*/
	URI_ASSERT_BADPATH("http:////BADPATH");
}
#if defined(WSMUTURIOPT)
#if 0
#define URI_TEST_RESOLVE_IMPL(string, eq, strict) \
	{ \
		MutUri uri(string); \
		uri.Resolve(masteruri, strict); \
		WSTEST_ASSERT_EQUAL(uri.BuildUri(), eq); \
	}
#else
void URI_TEST_RESOLVE_DMPL(const MutUri& masteruri, const char* string,
	const char* eq, Uri::Strictness strict)
	{
		MutUri uri(string);
		MutStr sb = uri.BuildUri();
		uri.Resolve(masteruri, strict);
		sb = uri.BuildUri();
		(void)(eq);
	}
#define URI_TEST_RESOLVE_IMPL(string, eq, strict) \
	{\
		URI_TEST_RESOLVE_DMPL(masteruri, string, eq, strict);\
		MutUri uri(string); \
		uri.Resolve(masteruri, strict); \
		WSTEST_ASSERT_EQUAL(uri.BuildUri(), eq); \
	}
#endif
#define URI_TEST_RESOLVE(string, eq) \
		URI_TEST_RESOLVE_IMPL(string, eq, Uri::STRICT);
#define URI_TEST_RESOLVE_LAX(string, eq) \
		URI_TEST_RESOLVE_IMPL(string, eq, Uri::NONSTRICT);
WSTEST_FUNCTION(NormalResolving)
{
	MutUri masteruri("http://a/b/c/d;p?q");
	URI_TEST_RESOLVE("g:h"	,"g:h")
	URI_TEST_RESOLVE("g"	,"http://a/b/c/g")
	URI_TEST_RESOLVE("./g"	,"http://a/b/c/g")
	URI_TEST_RESOLVE("g/"	,"http://a/b/c/g/")
	URI_TEST_RESOLVE("/g"	,"http://a/g")
	URI_TEST_RESOLVE("//g"	,"http://g")
	URI_TEST_RESOLVE("?y"	,"http://a/b/c/d;p?y")
	URI_TEST_RESOLVE("g?y"	,"http://a/b/c/g?y")
	URI_TEST_RESOLVE("#s"	,"http://a/b/c/d;p?q#s")
	URI_TEST_RESOLVE("g#s"	,"http://a/b/c/g#s")
	URI_TEST_RESOLVE("g?y#s","http://a/b/c/g?y#s")
	URI_TEST_RESOLVE(";x"	,"http://a/b/c/;x")
	URI_TEST_RESOLVE("g;x"	,"http://a/b/c/g;x")
	URI_TEST_RESOLVE("g;x?y#s","http://a/b/c/g;x?y#s")
	URI_TEST_RESOLVE(""		,"http://a/b/c/d;p?q")
	URI_TEST_RESOLVE("."	,"http://a/b/c/")
	URI_TEST_RESOLVE("./"	,"http://a/b/c/")
	URI_TEST_RESOLVE(".."	,"http://a/b/")
	URI_TEST_RESOLVE("../"	,"http://a/b/")
	URI_TEST_RESOLVE("../g" ,"http://a/b/g")
	URI_TEST_RESOLVE("../..","http://a/")
	URI_TEST_RESOLVE("../../"		 ,	"http://a/")
	URI_TEST_RESOLVE("../../g"		 ,	"http://a/g")
}
WSTEST_FUNCTION(ComplexResolving)
{
	MutUri masteruri("http://a/b/c/d;p?q");
	URI_TEST_RESOLVE("../../../g"	, "http://a/g")
	URI_TEST_RESOLVE("../../../../g", "http://a/g")
	URI_TEST_RESOLVE("/./g"	  ,"http://a/g")
	URI_TEST_RESOLVE("/../g"  ,"http://a/g")
	URI_TEST_RESOLVE("g."	  ,"http://a/b/c/g.")
	URI_TEST_RESOLVE(".g"	  ,"http://a/b/c/.g")
	URI_TEST_RESOLVE("g.."	  ,"http://a/b/c/g..")
	URI_TEST_RESOLVE("..g"	  ,"http://a/b/c/..g")

	masteruri = "file:doc.chm#xchm:/d/e";
	URI_TEST_RESOLVE("/a/b/c.jpg"	  ,"file://doc.chm/a/b/c.jpg")
}
WSTEST_FUNCTION(ReallyComplexResolving)
{
	MutUri masteruri("http://a/b/c/d;p?q");
	URI_TEST_RESOLVE("./../g"		,"http://a/b/g")
	URI_TEST_RESOLVE("./g/."		,"http://a/b/c/g/")
	URI_TEST_RESOLVE("g/./h"		,"http://a/b/c/g/h")
	URI_TEST_RESOLVE("g/../h"		,"http://a/b/c/h")
	URI_TEST_RESOLVE("g;x=1/./y"	,"http://a/b/c/g;x=1/y")
	URI_TEST_RESOLVE("g;x=1/../y"	,"http://a/b/c/y")
}
WSTEST_FUNCTION(QueryFragmentResolving)
{
	MutUri masteruri("http://a/b/c/d;p?q"); //query/fragment ambigiousness
	URI_TEST_RESOLVE("g?y/./x",		"http://a/b/c/g?y/./x")
	URI_TEST_RESOLVE("g?y/../x",	"http://a/b/c/g?y/../x")
	URI_TEST_RESOLVE("g#s/./x",		"http://a/b/c/g#s/./x")
	URI_TEST_RESOLVE("g#s/../x",	"http://a/b/c/g#s/../x")
}
WSTEST_FUNCTION(BackwardsResolving)
{
	MutUri masteruri("http://a/b/c/d;p?q");
	URI_TEST_RESOLVE("http:g" ,	 "http:g")		   //strict
	URI_TEST_RESOLVE_LAX("http:g", "http://a/b/c/g"); //non-strict, bw compat
}
#endif /* WSMUTURIOPT */
WSTEST_FUNCTION(Assignment)
{
	URI uri1, uri2;
	char* uri1s; char* uri2s;
	size_t uri1slen, uri2slen;
	URI_Create(&uri1, ("http://mysite.com"));
	URI_Create(&uri2, ("http://mysite2.com"));
	uri2 = uri1;
	URI_BuildURI(&uri1, &uri1s, &uri1slen);
	URI_BuildURI(&uri2, &uri2s, &uri2slen);
	WSTEST_ASSERT(uri1slen == uri2slen
		&& WSMemoryCompare(uri1s, uri2s, uri1slen * sizeof(char)) == 0);
	WSFREE(uri1s);
	WSFREE(uri2s);
}
WSTEST_FUNCTION(Comparison)
{
	URI uri1, uri2;
	URI_Create(&uri1, ("http://mysite.com"));
	URI_Create(&uri2, ("http://mysite.com"));
	WSTEST_ASSERT(URI_IsEqual(&uri1, &uri2));
	URI_Create(&uri1, ("http://mysite2.com"));
	URI_Create(&uri2, ("http://mysite3.com"));
	WSTEST_ASSERT(URI_IsEqual(&uri1, &uri2) == FALSE);
	URI_Create(&uri1, ("http://mysite4.com"));
	URI_Create(&uri2, ("http://mysite4.com"));
	WSTEST_ASSERT(URI_IsEqual(&uri1, &uri2));
}
#if defined(WSMUTURIOPT)
WSTEST_FUNCTION(Unescaping)
{
	MutStr escaped, unescaped;
	escaped = "http://test.com/of/file%3A%2F%2FC%3A%5Curi%5C"
			  "escaping%5Cthat%5Cseems%5Cbroken%5Csadly%5B1%5D.rss";
	//URI uritest = URI(escaped);
	unescaped = Uri(escaped).BuildUnescapedUri();
	MutStr unescaped2 = Uri::Unescape(escaped);
	//bool trueee = "http://test.com/of/file://C:\\uri\\escaping\\that\\seems\\broken\\sadly[1].rss" == unescaped;
	WSTEST_ASSERT_EQUAL( unescaped,
							"http://test.com/of/file://C:\\uri\\"
						  "escaping\\that\\seems\\broken\\sadly[1].rss");
	WSTEST_ASSERT_EQUAL( unescaped, Uri::Unescape(escaped) );
	escaped = "http://ru.wikipedia.org/wiki/"
			  "%D0%A6%D0%B5%D0%BB%D0%BE%D0%B5_%D1%87%D0%B8%D1%81%D0%BB%D0%BE";
	unescaped = Uri::Unescape(escaped);
	WSTEST_ASSERT_EQUAL(unescaped,
						"http://ru.wikipedia.org/wiki/"
						 "\xD0\xA6\xD0\xB5\xD0\xBB\xD0\xBE\xD0\xB5_"
						 "\xD1\x87\xD0\xB8\xD1\x81\xD0\xBB\xD0\xBE");
	escaped = "%2FH%C3%A4ll%C3%B6%5C";
	unescaped = Uri(escaped).BuildUnescapedUri();
	WSTEST_ASSERT_EQUAL(unescaped, "\x2FH\xC3\xA4ll\xC3\xB6\x5C");

	Uri testuu("scheme://///////////");
	MutStr uu = testuu.BuildUri();

	Uri testuu2("scheme://example.com//a?valid=yes");
	uu = testuu2.BuildUri();
	Uri testuu3("scheme:////?invalid");
	uu = testuu3.BuildUri();
	Uri testuu4("scheme:////SERVERA/////////////////////////?abnf=ok");
	uu = testuu4.BuildUri();
}
#endif /*WSMUTURIOPT*/
WSTEST_FUNCTION(FileScheme)
{
	/*file:// variety (NOT CONFORMING TO THE RFC)*/
	URI_ASSERT_PATH_EQUAL( "file://e:/wscode/script1.xml",
					"e:/wscode/script1.xml" );
	/*file:/// variety*/
	URI_ASSERT_PATH_EQUAL( "file:///e:/wscode/script1.xml",
					"/e:/wscode/script1.xml" );
	/*file:/ variety*/
	URI_ASSERT_PATH_EQUAL( "file:/e:/wscode/script1.xml",
					"/e:/wscode/script1.xml" );
	/*file: variety*/
	URI_ASSERT_PATH_EQUAL( "file:e:/wscode/script1.xml",
					"e:/wscode/script1.xml" );
}
WSTEST_FUNCTION(URLCompat)
{
	URI test;
	URI_Create(&test, ("file:\"myf\"ile.txt"));
#if defined(WSMUTURIOPT)
	MutUri muttest("file:\"myf\"ile.txt");

	WSTEST_ASSERT_EQUAL(muttest.BuildUri(), "file:%22myf%22ile.txt");
	WSTEST_ASSERT_EQUAL(muttest.GetPath(), "%22myf%22ile.txt");
#endif
	WSTEST_ASSERT(UriPartCmp(&URI_GetScheme(&test), "file"));
}

/* Normalization*/
#if defined(WSMUTURIOPT)
#if 0
#define URI_ASSERT_NORMALIZEDENCODEDPATH_EQUAL(uri, expected) \
	{ MutUri nuri(uri); nuri.Resolve(MutUri("http://a/"));\
	  WSTEST_ASSERT_EQUAL(nuri.GetPath(), expected); }
#define URI_ASSERT_NORMALIZEDPATH_EQUAL(uri, expected) \
	{ URI_ASSERT_NORMALIZEDENCODEDPATH_EQUAL(uri, expected); }

#else
#define URI_ASSERT_NORMALIZEDENCODEDPATH_EQUAL(uri, expected) \
	{	MutUri nuri(uri); nuri.Normalize(); \
		WSTEST_ASSERT_EQUAL(nuri.GetPath(), expected); }
#define URI_ASSERT_NORMALIZEDPATH_EQUAL(uri, expected) \
	{ URI_ASSERT_NORMALIZEDENCODEDPATH_EQUAL(uri, expected); }
#endif
WSTEST_FUNCTION(Normalizing)
{
	//5.2.4 #2 remove dot segments
	URI_ASSERT_NORMALIZEDPATH_EQUAL("./", "");			//A
	URI_ASSERT_NORMALIZEDPATH_EQUAL("/./", "/");		//B
	URI_ASSERT_NORMALIZEDPATH_EQUAL("/.", "/");			//B2
	URI_ASSERT_NORMALIZEDPATH_EQUAL("/../", "/");		//C
	URI_ASSERT_NORMALIZEDPATH_EQUAL("/..", "/");		//C2
	URI_ASSERT_NORMALIZEDPATH_EQUAL(".", "");			//D
	URI_ASSERT_NORMALIZEDPATH_EQUAL("../", "");			//A2
	URI_ASSERT_NORMALIZEDPATH_EQUAL("..", "");			//D2
	URI_ASSERT_NORMALIZEDPATH_EQUAL("../../../", "");	//A2 complex
	URI_ASSERT_NORMALIZEDPATH_EQUAL("../..", "");		//A2+D2 complex
	//5.2.4 in practice
	URI_ASSERT_NORMALIZEDPATH_EQUAL("path/john/../../../joe", "joe");
	URI_ASSERT_NORMALIZEDPATH_EQUAL("http://user:password@192.256.1.100:5050/../path", "/path");
	URI_ASSERT_NORMALIZEDPATH_EQUAL("http://user:password@192.256.1.100:5050/path/../", "/");
	URI_ASSERT_NORMALIZEDPATH_EQUAL("http://user:password@192.256.1.100:5050/path/.", "/path/");
	URI_ASSERT_NORMALIZEDPATH_EQUAL("http://user:password@192.256.1.100:5050/path/./", "/path/");
	// hexdigit normalizing
	URI_ASSERT_NORMALIZEDENCODEDPATH_EQUAL("%aA", "%AA");
	URI_ASSERT_NORMALIZEDENCODEDPATH_EQUAL("%Aa", "%AA");
	URI_ASSERT_NORMALIZEDENCODEDPATH_EQUAL("%aa", "%AA");
	URI_ASSERT_NORMALIZEDENCODEDPATH_EQUAL("%AA", "%AA");
	URI_ASSERT_NORMALIZEDENCODEDPATH_EQUAL("%Af", "%AF");
	//Alpha/Digit/'-'/'.'/'_'/'~'
	URI_ASSERT_NORMALIZEDENCODEDPATH_EQUAL("%42", "B");
	URI_ASSERT_NORMALIZEDENCODEDPATH_EQUAL("%30", "0");
	URI_ASSERT_NORMALIZEDENCODEDPATH_EQUAL("%2D", "-");
	URI_ASSERT_NORMALIZEDENCODEDPATH_EQUAL("%2E", ".");
	URI_ASSERT_NORMALIZEDENCODEDPATH_EQUAL("%5F", "_");
	URI_ASSERT_NORMALIZEDENCODEDPATH_EQUAL("%7E", "~");
	URI_ASSERT_NORMALIZEDENCODEDPATH_EQUAL("%42%30%2D%2E%5F%7E", "B0-._~");
	URI_ASSERT_NORMALIZEDENCODEDPATH_EQUAL("%F1%42%30%2D%Fa%2E%5F%7E%F1", "%F1B0-%FA._~%F1");
}
#endif
/*****************************************************************************/
WSTEST_SUITE_BEGIN(WSURI)
	WSTEST_GROUP_BEGIN(Host)
		WSTEST_CASE(IPv4)
		WSTEST_CASE(IPv6)
		WSTEST_CASE(Host)
	WSTEST_GROUP_END(Host)
	WSTEST_GROUP_BEGIN(Path)
		WSTEST_CASE(Paths)
	WSTEST_GROUP_END(Path)
WSTEST_SUITE_END(WSURI)
#if defined(WSMUTURIOPT)
WSTEST_SUITE_BEGIN(WSMUTURI)
	WSTEST_GROUP_BEGIN(Resolving)
		WSTEST_CASE(NormalResolving)
		WSTEST_CASE(ComplexResolving)
		WSTEST_CASE(ReallyComplexResolving)
		WSTEST_CASE(QueryFragmentResolving)
		WSTEST_CASE(BackwardsResolving)
	WSTEST_GROUP_END(Resolving)
	WSTEST_GROUP_BEGIN(Compatability)
		WSTEST_CASE(FileScheme)
		WSTEST_CASE(URLCompat)
	WSTEST_GROUP_END(Compatability)
	WSTEST_GROUP_BEGIN(InternalFeatures)
		WSTEST_CASE(Normalizing)
	WSTEST_GROUP_END(InternalFeatures)
WSTEST_SUITE_END(WSMUTURI)
#endif /*WSMUTURIOPT */
#if defined(__cplusplus)
}
#endif

/*****************************************************************************/
#ifdef URISELFCONTAINED
int main()
{
	WSTEST_SUITE_RUN(WSURI);
#if defined(WSMUTURIOPT)
	WSTEST_SUITE_RUN(WSMUTURI);
#endif
	return 0;
}
#endif
#endif /* defined(WSTESTOPT)*/
#endif /* URI_IMPLEMENTATION */

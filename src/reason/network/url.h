
/**
 * Reason
 * Copyright (C) 2009  Emerson Clarke
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifndef NETWORK_URL_H
#define NETWORK_URL_H

#include "reason/system/string.h"
#include "reason/system/path.h"
#include "reason/system/encoding/mime.h"
#include "reason/system/encoding/transcoder.h"
#include "reason/system/object.h"

#include "reason/structure/set.h"
#include "reason/structure/list.h"
#include "reason/system/properties.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using namespace Reason::System;
using namespace Reason::System::Encoding;

using namespace Reason::Structure;

using namespace Reason::System;

namespace Reason { namespace System {
	class Properties;
}}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Reason { namespace Network {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Url : public Path
{
public:
	static Identity Instance;

	virtual Identity& Identify()
	{
		return Instance;
	}

	virtual bool InstanceOf(Identity &identity)
	{
		return Instance == identity || Path::InstanceOf(identity);
	}

public:

	enum UrlType
	{
		URL_TYPE_NONE		=(0),	
		URL_TYPE_HTTP		=(1),	
		URL_TYPE_HTTPS		=(2),	
		URL_TYPE_FTP		=(3),	
		URL_TYPE_GOPHER		=(4),	
		URL_TYPE_MAILTO		=(5),	
		URL_TYPE_NEWS		=(6),	
		URL_TYPE_NNTP		=(7),	
		URL_TYPE_TELNET		=(8),	
		URL_TYPE_WAIS		=(9),	
		URL_TYPE_FILE		=(10),	

		URL_TYPE_UNKNOWN	=(11),	
	};

	Bitmask16	Type;

	virtual int Hash();
	virtual int Compare(Reason::System::Object * object, int comparitor=COMPARE_GENERAL);
	virtual bool Equals(Reason::System::Object * object, int comparitor=COMPARE_GENERAL);

	Reason::System::Path	*Scheme;		
	Reason::System::Path	*Username;		
	Reason::System::Path	*Password;		
	Reason::System::Path	*Host;			
	Reason::System::Path	*Path;			
	Reason::System::Path	*Port;			

	Reason::System::Path	*Query;		
	Reason::System::Path	*Fragment;	

	Url(const Url &url)						{Initialise();Construct(url);}
	Url(const Sequence & sequence)			{Initialise();Construct(sequence);}
	Url(const char *data)					{Initialise();Construct(data);}
	Url(char *data, int size)				{Initialise();Construct(data, size);}
	Url()									{Initialise();}
	~Url()									{}

	Url & operator = (const Url & url)	{Construct(url);return *this;}

	void Release();
	void Destroy();

	bool Construct(const Url &url);
	bool Construct(const Sequence & sequence)			{return Construct(sequence.Data,sequence.Size);};
	bool Construct(const char *url)						{return Construct((char*)url,String::Length(url));};
	bool Construct(char *url, int size);
	bool Construct();

	void Parameterise();
	void Parameters(Reason::System::Properties &properties);

	void Filename(Reason::System::Substring &string);
	Reason::System::Substring Filename();

	bool Download(String &string, bool textOnly = false);	

	void Normalise();
	static void Normalise(String & path);

	void Absolute(const Url &parent);
	void Relative(const Url &parent);

	bool IsAlias(const Url &url);

	bool IsValid();
	bool IsUnknown();
	bool IsRelative();
	bool IsAbsolute();
	bool IsHttp();
	bool IsFtp();
	bool IsFile();
	bool IsMailto();

	static void Encode(String & string) {Transcoder(string).EncodeUrl();}
	static void Decode(String & string) {Transcoder(string).DecodeUrl();}

protected:

	void Initialise();
	void WhitespaceEncode();
	void WhitespaceDecode();
};

class UrlLibrary
{
public:
	static int ResolvePort(const Url & url);
	static const char * ResolveProtocol(const Url & url);
	static const char * ResolveScheme(const Url & url);

	static bool IsProtocol(const Url & url);
	static bool IsApplication(const Url & url);

	struct UrlProtocol
	{
		const char * Scheme;
		const char * Protocol;
		const int Port;
	};

	static const int ProtocolCount;
	static const UrlProtocol Protocol[];

	class UrlAllocator
	{

	};
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif


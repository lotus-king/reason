
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
#ifndef MESSAGING_CALLCHAIN_H
#define MESSAGING_CALLCHAIN_H

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "reason/system/output.h"
#include "reason/generic/generic.h"
#include "reason/generic/traits.h"
#include "reason/structure/array.h"
#include "reason/messaging/callback.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using namespace Reason::Generic;
using namespace Reason::System;
using namespace Reason::Structure;

namespace Reason { namespace Messaging {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename _Return_>
class Transaction
{
public:

	Reason::Messaging::Callback<_Return_> Begin;
	Reason::Messaging::Callback<_Return_> End;

	Transaction(Reason::Messaging::Callback<_Return_> & begin, Reason::Messaging::Callback<_Return_> & end)
	{
		Begin = begin;
		Begin = end;

		Begin();
	}

	template<typename _That_>
	Transaction(_That_ * that, Reason::Messaging::Callback<_Return_> & begin, Reason::Messaging::Callback<_Return_> & end)
	{

		Begin = begin;
		End = end;

		Begin.Call.That = that;
		End.Call.That = that;

		Begin();
	}

	~Transaction()
	{

		End();
	}

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename _Return_, typename _Arg1_=None, typename _Arg2_=None, typename _Arg3_=None, typename _Arg4_=None, typename _Arg5_=None>
class Adapter
{
public:

};

template <typename _Return_, typename _Arg1_=None, typename _Arg2_=None, typename _Arg3_=None, typename _Arg4_=None, typename _Arg5_=None>
class Callchain
{
public:

	Reason::Structure::Array< Reason::Messaging::Callback<_Return_,_Arg1_,_Arg2_,_Arg3_,_Arg4_,_Arg5_> > Incoming;
	Reason::Structure::Array< Reason::Messaging::Callback<_Return_,_Arg1_,_Arg2_,_Arg3_,_Arg4_,_Arg5_> > Outgoing;

	Callchain & operator << (const Reason::Messaging::Callback<_Return_,_Arg1_,_Arg2_,_Arg3_,_Arg4_,_Arg5_> & callback)
	{
		Incoming.Insert(callback);
	}

	Callchain & operator >> (const Reason::Messaging::Callback<_Return_,_Arg1_,_Arg2_,_Arg3_,_Arg4_,_Arg5_> & callback)
	{
		Outgoing.Insert(callback);
	}

};

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


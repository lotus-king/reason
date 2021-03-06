
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
#ifndef PLATFORM_ASYNC_H
#define PLATFORM_ASYNC_H

#include "reason/generic/generic.h"
#include "reason/messaging/callback.h"
#include "reason/platform/thread.h"

using namespace Reason::Generic;
using namespace Reason::Messaging;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Reason { namespace Platform {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename _Callback_>
class Async : public Thread
{
public:

	_Callback_ Callback;
};

template<typename _Return_, typename _Arg1_,typename _Arg2_,typename _Arg3_,typename _Arg4_,typename _Arg5_>
class Async< Reason::Messaging::Callback<_Return_,_Arg1_,_Arg2_,_Arg3_,_Arg4_,_Arg5_> > : public Threaded
{
public:

	Reason::Messaging::Callback<_Return_,_Arg1_,_Arg2_,_Arg3_,_Arg4_,_Arg5_> Callback;
	Reason::Messaging::Callback<void,_Return_> Overlap;
	_Return_ Return;
	Locked Complete;

	struct Arguments
	{
		_Arg1_ Arg1;
		_Arg2_ Arg2;
		_Arg3_ Arg3;
		_Arg4_ Arg4;
		_Arg5_ Arg5;

		Arguments():
			Arg1(Null<_Arg1_>::Value()),
			Arg2(Null<_Arg1_>::Value()),
			Arg3(Null<_Arg1_>::Value()),
			Arg4(Null<_Arg1_>::Value()),
			Arg5(Null<_Arg1_>::Value())
		{
		}
	};

	Arguments Args;

	Async():Return(Null<_Return_>::Value())
	{

		Critical.Enter();
		Complete.Lock();
	}

	void Run(void * thread)
	{
		while (this->Threads.Count > 0 )
		{

			Critical.Wait();
			if (this->Threads.Count > 1)
			{

				Overlap(Callback(Args.Arg1,Args.Arg2,Args.Arg3,Args.Arg4,Args.Arg5));
			}
			else
			{
				Return = Callback(Args.Arg1,Args.Arg2,Args.Arg3,Args.Arg4,Args.Arg5);
				Complete.Unlock();
			}
		}
	}

	bool operator () (Arguments & args)
	{

		Complete.Lock();

		if (Critical.Owned() || Critical.Enter())
		{
			Args = args;
			Critical.Leave();
		}
		else
		{
			return false;
		}
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Future
{
public:

};

class Promise
{
public:

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


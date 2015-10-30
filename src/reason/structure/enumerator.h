
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

#ifndef STRUCTURE_ENUMERATOR_H
#define STRUCTURE_ENUMERATOR_H

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "reason/system/interface.h"
#include "reason/generic/generic.h"

#include "reason/structure/iterator.h"
#include "reason/structure/disposable.h"
#include "reason/structure/array.h"
#include "reason/structure/policy.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Reason { namespace Structure { namespace Abstract {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}}}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Reason { namespace Structure { 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename _Kind_>
class Generator 
{
public:

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename _Kind_, typename _Iterable_=Structure::List<_Kind_> >
class Enumerator : public _Iterable_
{
public:

	Structure::Iterator<_Kind_> EnumerationIterator;

	bool Has() {return EnumerationIterator.Has();}

	Reason::Structure::Iterand<_Kind_> & Move() {return EnumerationIterator.Move();}
	Reason::Structure::Iterand<_Kind_> & Move(int amount) {return EnumerationIterator.Move(amount);}
	Reason::Structure::Iterand<_Kind_> & Forward() 
	{
		if (!EnumerationIterator.Iterand)
			EnumerationIterator.Iterand = this->Iterate();
		return EnumerationIterator.Forward();
	}

	Reason::Structure::Iterand<_Kind_> & Reverse() 
	{	
		if (!EnumerationIterator.Iterand)
			EnumerationIterator.Iterand = this->Iterate();	
		return EnumerationIterator.Reverse();
	}

	int Index() {return EnumerationIterator.Index();}

	typename Type<_Kind_>::Pointer Pointer() {return EnumerationIterator.Pointer();}
	typename Type<_Kind_>::Reference Reference() {return EnumerationIterator.Reference();}
	typename Template<_Kind_>::Reference operator () (void) {return EnumerationIterator();}

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename _Kind_>
class Enumerable : public Structure::Iterable<_Kind_> 
{
public:

	Structure::Iterator<_Kind_> EnumerationIterator;

	Enumerable()
	{

	}

	bool Has() {return EnumerationIterator.Has();}

	Reason::Structure::Iterand<_Kind_> & Move() {return EnumerationIterator.Move();}
	Reason::Structure::Iterand<_Kind_> & Move(int amount) {return EnumerationIterator.Move(amount);}
	Reason::Structure::Iterand<_Kind_> & Forward() 
	{
		if (!EnumerationIterator.Iterand)
			EnumerationIterator.Iterand = this->Iterate();
		return EnumerationIterator.Forward();
	}

	Reason::Structure::Iterand<_Kind_> & Reverse() 
	{	
		if (!EnumerationIterator.Iterand)
			EnumerationIterator.Iterand = this->Iterate();	
		return EnumerationIterator.Reverse();
	}

	int Index() {return EnumerationIterator.Index();}

	typename Type<_Kind_>::Pointer Pointer() {return EnumerationIterator.Pointer();}
	typename Type<_Kind_>::Reference Reference() {return EnumerationIterator.Reference();}
	typename Template<_Kind_>::Reference operator () (void) {return EnumerationIterator();}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename _Kind_>
class Enumeration : public Enumerable<_Kind_>
{
public:

	static Identity	Instance;
	virtual Identity & Identify() {return Instance;};

	_Kind_ * Generate;

	Callback<_Kind_&,int,void*> Generator;

	Enumeration():Generate(0)
	{

	}

	class EnumerationIteration : public Structure::Iteration< _Kind_ >
	{
	public:

		int Index;

		EnumerationIteration():Index(0) {}
		EnumerationIteration(Enumeration<_Kind_> * enumeration):
			Structure::Iteration<_Kind_>(enumeration),Index(0) 
		{

		}

		void Forward(Structure::Iterand<_Kind_> & iterand)
		{
			iterand.Forward();

			Enumeration<_Kind_> * enumeration = (Enumeration<_Kind_> *)this->Iterable();
			Index = 0;
			iterand.Kind(enumeration->Generator(Index,enumeration->Generate));
		}

		void Reverse(Structure::Iterand<_Kind_> & iterand)
		{
			iterand.Reverse();
			Enumeration<_Kind_> * enumeration = (Enumeration<_Kind_> *)this->Iterable();

			Index = -1;
			iterand.Kind(enumeration->Generator(Index,enumeration->Generate));
		}

		void Move(Structure::Iterand<_Kind_> & iterand, int amount)
		{
			amount *= iterand.Move();
			Enumeration<_Kind_> * enumeration = (Enumeration<_Kind_> *)this->Iterable();
			Index += amount;
			iterand.Kind(enumeration->Generator(Index,enumeration->Generate));
		}
	};

	Structure::Iterator<_Kind_> Iterate()
	{
		return Structure::Iterator<_Kind_>(new EnumerationIteration(this));
	}
};

template<typename _Kind_>
Identity Enumeration<_Kind_>::Instance;

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


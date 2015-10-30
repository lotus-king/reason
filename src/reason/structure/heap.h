
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
#ifndef STRUCTURE_HEAP_H
#define STRUCTURE_HEAP_H

#include "reason/structure/array.h"
#include "reason/structure/tree.h"

using namespace Reason::System;

namespace Reason { namespace Structure {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Abstract {

template<typename _Kind_>
class TreeHeap : public UnorderedTree<_Kind_>
{

};

template<typename _Kind_>
class ArrayHeap : public UnorderedArray<_Kind_>
{

};

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename _Kind_>
class Heap : public Abstract::ArrayHeap<_Kind_>
{
public:

	int Comparison;

	Heap(int comparitor=Reason::System::Comparable::COMPARE_GENERAL):Comparison(comparitor)
	{

	}

	void Insert(typename Template<_Kind_>::ConstantReference type)
	{

		int index = this->Size;
		Append(type);
		int parent = Parent(index);

		while (index > 0 && Reason::Structure::Comparer<_Kind_>::Compare(this->Data[index],this->Data[parent],Comparison) <= 0)
		{
			Swap(index,parent);
			index = parent;
			parent = Parent(parent);
		}

		return true;
	}

	void Delete(int index=0)
	{

		int child = this->Size-1;
		this->Data[index] = this->Data[child];
		DeleteAt(child);

		while (true)
		{
			int left = Left(index);
			int right = Right(index);

			if (left >= this->Size)
			{
				if (right >= this->Size) 
					break;

				child = right;
			}
			else
			if (right >= this->Size)
			{
				child = left;
			}
			else
			{
				if (Reason::Structure::Comparer<_Kind_>::Compare(this->Data[left],this->Data[right],Comparison) <= 0)
					child = left;
				else
					child = right;

				if (Reason::Structure::Comparer<_Kind_>::Compare(this->Data[index],this->Data[child],Comparison) <= 0)
					break;
			}

			Swap(index,child);
			index = child;
		}
	}

	typename Template<_Kind_>::Reference Select(int index=0)
	{
		OutputAssert(this->Size > 0);
		return this->Data[index];
	}

private:

	int Left(int index) {return 2*index+1;}
	int Right(int index) {return 2*index+2;}
	int Parent(int index) {return (int)floor((double)(index-1)/2);}
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

}}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif

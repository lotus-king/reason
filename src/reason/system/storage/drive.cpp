
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


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "reason/system/storage/drive.h"
#include "reason/system/string.h"
#include "reason/system/number.h"
#include "reason/system/output.h"
#include "reason/platform/platform.h"

#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#ifdef REASON_PLATFORM_WINDOWS
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <direct.h>
#endif

#ifdef REASON_PLATFORM_POSIX
#include <sys/types.h>
#include <sys/stat.h>

#ifndef REASON_PLATFORM_CYGWIN
#include <sys/dir.h>
#endif

#include <unistd.h>
#include <dirent.h>
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using namespace Reason::Structure;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Reason { namespace System { namespace Storage {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const Drive::FileExtensionMap Drive::FileExtension[Drive::FileExtensionCount] = {
{"txt",	TEXT_PLAIN},
{"html",TEXT_HTML},
{"htm",	TEXT_HTML},
{"asp",	TEXT_HTML},	
{"xml",	TEXT_XML},
{"js",	TEXT_SCRIPT},
{"css",	TEXT_CSS},
{"exe",	BINARY_EXEC_EXE},
{"vbs",	BINARY_EXEC_SCRIPT},
{"js",	BINARY_EXEC_SCRIPT},
{"bat",	BINARY_EXEC_BAT},
{"com",	BINARY_EXEC_COM},
{"dll",	BINARY_EXEC_DLL},
{"zip",	BINARY_ARCH_ZIP},
{"arj",	BINARY_ARCH_ARJ},
{"rar",	BINARY_ARCH_RAR},
{"gz",	BINARY_ARCH_GZ},
{"tar",	BINARY_ARCH_TAR},
{"wav",	BINARY_MEDIA_WAV},
{"mp3",	BINARY_MEDIA_MP3},
{"rm",	BINARY_MEDIA_RM},
{"gif",	BINARY_MEDIA_GIF},
{"jpg",	BINARY_MEDIA_JPG},
{"png",	BINARY_MEDIA_PNG},
{"bmp",	BINARY_MEDIA_BMP},
{"tif",	BINARY_MEDIA_TIF},
{"psd",	BINARY_MEDIA_PSD}};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Drive::Drive(const Drive & filesystem)
{
	Construct(filesystem);
}

Drive::Drive(const Sequence & sequence)
{
	Construct(sequence);
}

Drive::Drive(const char * path)
{
	Construct(path);
}

Drive::Drive(char * path, int size)
{
	Construct(path,size);
}

Drive::Drive()
{
}

Drive::~Drive()
{

}

Drive & Drive::operator = (const Drive & filesystem)
{
	Construct(filesystem);
	return *this;
}

bool Drive::Current(const Path & path)
{

	static const int maxPath = 260;

	char buffer[maxPath];
	if( getcwd( buffer, maxPath ) != 0 )
	{
		((Path&)path).Construct(buffer);
		return true;
	}

	return false;
}

bool Drive::Status(const Path & path, class Stat & stat)
{
	stat = Reason::System::Stat(path.Data,path.Size);
	return true;
}

void Drive::Release()
{
	Path::Release();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Identity DriveFile::Instance;

DriveFile::DriveFile():
	Handle(0),Options(0)
{
}

DriveFile::~DriveFile()
{
}

void DriveFile::Construct(const Path & path, char *data, int size)
{
	Construct(path);
}

void DriveFile::Construct(const Path & path)
{
	Close(path);
}

bool DriveFile::Create(const Path & path, int access)
{
	access = ( (access & File::ACCESS_READ) && !(access & File::ACCESS_WRITE) ) ? S_IREAD:S_IREAD|S_IWRITE;

	if (close(creat(((Path&)path).Print(),access)) == -1)
	{
		if (((Path&)path).Contains("/") || ((Path&)path).Contains("\\"))
		{

			FilePath file(path);

			DriveFolder folder;
			folder.Create(*file.Path);
			return close(creat(((Path&)path).Print(),access)) != -1;
		}

	}

	return true;
}

bool DriveFile::Open(const Path & path, int options)
{
	return Reason::System::FileStorage::Open(path,options);
}

bool DriveFile::Open(const Path & path, int mode,int type, int access, int usage)
{
	if (((Path&)path).IsEmpty())
	{
		OutputError("DriveFile::Open - Could not open file with no name. \"%s\".\n",((Path&)path).Print());
		return false;
	}

	if (!mode) mode = File::MODE_OPEN;
	Construct(path);

	if (Handle) 
	{
		Close(path);	
	}

	Options = mode|type|access|usage;

	String key;
	key.Allocate(4);

	switch (mode)
	{
	case File::MODE_APPEND:
		{
			switch (access)
			{
			case File::ACCESS_MUTABLE:case File::ACCESS_READ: key.Append("a+"); break;
			case File::ACCESS_WRITE:default: key.Append("a"); break;
			}
		}
		break;
	case File::MODE_OPEN:
		{
			switch (access)
			{
			case File::ACCESS_MUTABLE:case File::ACCESS_WRITE: key.Append("r+"); break;
			case File::ACCESS_READ:default: key.Append("r"); break;
			}
		}
		break;
	case File::MODE_CREATE:
		{
			switch (access)
			{
			case File::ACCESS_MUTABLE:case File::ACCESS_READ: key.Append("w+"); break;
			case File::ACCESS_WRITE:default: key.Append("w"); break;
			}
		}
		break;
	}

	switch(type)
	{
	case File::TYPE_BINARY: key.Append("b"); break;
	case File::TYPE_TEXT:default: key.Append("t"); break;
	}

	switch(usage)
	{
	case File::USAGE_SEQUENTIAL: key.Append("S"); break;
	case File::USAGE_RANDOM: key.Append("R"); break;
	case File::USAGE_SCRATCH: key.Append("T"); break;
	case File::USAGE_TEMPORARY: key.Append("D"); break;
	}

	if ((Handle = fopen(((Path&)path).Print(),key.Data)) == 0 && (mode&File::MODE_CREATE))
	{

		if (!((Path&)path).IsEmpty())
		{

			FilePath file(path);			
			DriveFolder folder;

			folder.Create(*file.Path);
			Handle = fopen(((Path&)path).Print(),key.Data);
		}
	}

	if (Handle == 0)
	{
		OutputError("DriveFile::Open - Could not open file. \"%s\".\n",((Path&)path).Print());
		return false;
	}

	return true;
}

bool DriveFile::Close(const Path & path)
{
	if (Handle != 0) 
	{
		if (fclose(Handle) == -1)
		{
			OutputError("DriveFile::Close - Could not close file. \"%s\"\n",((Path&)path).Print());
			return false;
		}
		else
		{
			Handle = 0;
			return true;
		}
	}

	return true;
}

Stat DriveFile::Status(const Path & path)
{
	struct stat st;
	memset(&st,0,sizeof(struct stat));

	if ( !Handle && !Open(path)) 
	{
		OutputError("DriveFile::Status - The file does not exist or could not be opened for status.\n");	
		return Stat();
	}

	#ifdef REASON_PLATFORM_WINDOWS	
	if (fstat(_fileno(Handle),&st) == 0)
	#else 
	if (fstat(fileno(Handle),&st) == 0)
	#endif
	{

		return Stat(st,true);
	}

	return Stat();
}

int DriveFile::Seek(const Path & path, int position, int origin)	
{
	if ( !Handle && !Exists(path)) 
	{
		OutputError("DriveFile::Seek - The file does not exist or could not be opened for seeking.\n");	
		return 0;
	}

	if (position == 0 && origin == -1)
	{

		rewind(Handle);
		return 0;
	}
	else
	if (fseek(Handle,position,((origin<0)?0:((origin>0)?2:1))) != 0)
	{
		OutputError("DriveFile::Seek - Could not seek with position %d, origin %d.\n",position,origin);
	}

	return ftell(Handle);
}

bool DriveFile::Rename(const Path & path, char * name, int size)
{

	String alias;

	alias.Append(name,size);

	if (rename(((Path&)path).Print(),alias.Print()) == -1)
	{
		OutputError("DriveFile::Rename - Could not rename file. \"%s\" to \"%s\"\n",((Path&)path).Print(), alias.Print());
		return false;
	}

	return true;
}

bool DriveFile::Truncate(const Path & path)
{
	#ifdef REASON_PLATFORM_WINDOWS

	return _chsize(_fileno(Handle),Seek(0,0)) == 0;
	#endif

	#ifdef REASON_PLATFORM_POSIX
	return ftruncate((long)Handle,Seek(0,0)) == 0;	
	#endif

}

bool DriveFile::Delete(const Path & path)
{
	if (Exists(path) && remove(((Path&)path).Print()) == -1)
	{
		OutputError("DriveFile::Delete - Could not delete file. \"%s\"\n",((Path&)path).Print());
		return false;
	}

	return true;
}

int DriveFile::Read(const Path & path, char * data, int size)
{
	#ifdef REASON_PLATFORM_WINDOWS

		if ( Handle && (Handle->_flag & _IORW) && (Handle->_flag & (_IOREAD | _IOWRT)) == _IOWRT )
		{
			Flush(path);
			Handle->_flag |= _IOREAD;
		}

	#endif

	if (data == 0 || !(size > 0))
	{
		OutputError("DriveFile::Read - The destination is null or has zero size.\n");	
		return 0;
	}

	if (!Handle && !OpenReadable(path)) 
		return 0;

	int read = fread(data,sizeof(char),size,Handle);
	return (read<0)?0:read;
}

int DriveFile::ReadLine(const Path & path, char * data, int size)
{
	#ifdef REASON_PLATFORM_WINDOWS

		if ( Handle && (Handle->_flag & _IORW) && (Handle->_flag & (_IOREAD | _IOWRT)) == _IOWRT )
		{
			Flush(path);
			Handle->_flag |= _IOREAD;
		}

	#endif

	if (data == 0 || !(size > 0))
	{
		OutputError("DriveFile::ReadLine - The destination is null or has zero size.\n");	
		return 0;
	}

	if (!Handle && !OpenReadable(path)) 
		return 0;

	if (fgets(data,size,Handle) != 0)
	{

		int size = strlen(data);
		while (size > 0 && (data[size-1] == '\n' || data[size-1] == '\r'))
			data[--size]=0;

		return size;
	}
	else
	{
		return 0;
	}

}

int DriveFile::ReadLine(const Path & path, String & string)
{
	#ifdef REASON_PLATFORM_WINDOWS

		if ( Handle && (Handle->_flag & _IORW) && (Handle->_flag & (_IOREAD | _IOWRT)) == _IOWRT )
		{
			Flush(path);
			Handle->_flag |= _IOREAD;
		}

	#endif

	if (!Handle && !OpenReadable(path)) 
		return 0;

	int amount = 0;

	if (string.Allocated == 0)
		string.Allocate(0);

	int offset = 0;
	string.Size = 0;

	amount = string.Allocated-1;

	OutputAssert(amount > 0);

	char * data = 0;
	int size = 0;

	while(amount > 0 && (data=fgets(string.Data+offset,amount,Handle)) != 0)
	{
		string.Size += amount;

		size = strlen(data);	
		if (data[size-1] == '\n' || data[size-1] == '\r')
		{
			while (size > 0 && (data[size-1] == '\n' || data[size-1] == '\r'))
			{
				data[--size]=0;
				string.Size--;
			}

			break;
		}

		if (string.Size == (string.Allocated-1))
		{
			if (IsReadable(path))
			{
				string.Allocate(0);
			}
		}

		offset += amount;
		amount = (string.Allocated-1)-string.Size;		
	}

	string.Terminate();
	return string.Size;
}

int DriveFile::Write(const Path & path, char * data, int size)
{
	#ifdef REASON_PLATFORM_WINDOWS

		if ( Handle && (Handle->_flag & _IORW) && (Handle->_flag & (_IOREAD | _IOWRT)) == _IOREAD )
		{
			Flush(path);
			Handle->_flag |= _IOWRT;
		}
	#endif

	if (data == 0 || !(size > 0))
	{
		OutputError("DriveFile::Write - The source is null or has zero size.\n");	
		return 0;
	}

	if (!Handle && !OpenWriteable(path))
		return 0;

	int write = fwrite(data,sizeof(char),size,Handle);
	return (write<0)?0:write;;
}

int DriveFile::WriteLine(const Path & path, char * data, int size)
{
	int write = Write(path,data,size);
	if (write == size && Write(path,"\n",1)) 
		++write;

	return write;	
}

bool DriveFile::Flush(const Path & path)
{

	if (fflush(Handle) == 0)
	{

		return true;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Identity DriveFolder::Instance;

DriveFolder::DriveFolder()
{

}

DriveFolder::~DriveFolder()
{
}

bool DriveFolder::Exists(const Path & path)
{
	return Stat(path.Data,path.Size).Exists;
}

bool DriveFolder::Create(const Path & path)
{
	if (! Stat(path.Data,path.Size).Exists)
	{
		#ifdef REASON_PLATFORM_WINDOWS
		if (_mkdir(((Path&)path).Print()) == 0)
		{
			return true;
		}
		else
		{
			Reason::System::Path split(path);
			split.Split("/",true);
			String folder;
			Path::Enumerator iterator(split);
			for (iterator.Forward();iterator.Has();iterator.Move())
			{
				folder.Append(iterator.Reference());
				Stat stat(folder);
				if (!stat.Exists)
					if (!(_mkdir(folder.Print())==0))
						return false;

			}
		}
		#endif

		#ifdef REASON_PLATFORM_POSIX

		if (mkdir(((Path&)path).Print(),S_IRWXU|S_IRWXG|S_IRWXO) == 0)
		{
			return true;
		}
		else
		{
			Reason::System::Path split(path);
			split.Split("/",true);
			String folder;
			Path::Enumerator iterator(split);
			for (iterator.Forward();iterator.Has();iterator.Move())
			{
				folder.Append(iterator.Reference());
				Stat stat(folder);
				if (!stat.Exists)
					if (!(mkdir(folder.Print(),S_IRWXU|S_IRWXG|S_IRWXO)==0))
						return false;

			}
		}
		#endif
	}

	return false;
}

bool DriveFolder::Delete(const Path & path,  Reason::Structure::List<Reason::System::File> & files, Reason::Structure::List<Reason::System::Folder> & folders, bool recursive)
{

	if (Exists(path))
	{

		if (recursive)
		{
			List(path,files,folders,recursive);

			Iterator<Reason::System::File> file = files.Iterate();
			for (file.Forward();file.Has();file.Move())
				file().Delete();

			Iterator<Reason::System::Folder> folder = folders.Iterate();
			for (folder.Forward();folder.Has();folder.Move())
				folder().Delete(recursive);
		}

		if (rmdir(((Path&)path).Print()) == 0)
			return true;
	}

	return false;
}

bool DriveFolder::Rename(const Path & path, char * name, int size)
{
	String newname;

	newname.Append(name,size);

	if (rename(((Path&)path).Print(),newname.Print()) == -1)
	{
		OutputError("FilesystemFolder::Rename - Could not rename file. [%s] to [%s]\n",((Path&)path).Print(), newname.Print());
		return false;
	}

	return true;
}

bool DriveFolder::CreateFile(const Path & path, char *name)
{
	if (name == 0) 
		return false;

	String string;
	string << path << "/" << name;

	DriveFile file;

	if (! file.Exists(string))
	{
		return file.Create(string);
	}

	return false;
}

bool DriveFolder::DeleteFile(const Path & path, char *name)
{
	if (name == 0) 
		return false;

	String string;
	string << path.Data << "/" << name;

	DriveFile file;

	if (file.Exists(string))
	{
		return file.Delete(string);
	}

	return false;
}

bool DriveFolder::CreateFolder(const Path & path, char *name)
{
	if (name == 0) 
		return false;

	String string;
	string << path << "/" << name;

	#ifdef REASON_PLATFORM_WINDOWS
	if (_mkdir(string.Print()) == 0)
		return true;
	#endif
	#ifdef REASON_PLATFORM_POSIX
	if (mkdir(string.Print(),S_IRWXU|S_IRWXG|S_IRWXO) == 0)
		return true;
	#endif

	return false;
}

bool DriveFolder::DeleteFolder(const Path & path, char *name)
{

	String string;
	string << path << "/" << name;

	#ifdef REASON_PLATFORM_WINDOWS
	if (_rmdir(string.Print()) == 0)
		return true;
	#endif
	#ifdef REASON_PLATFORM_POSIX
	if (rmdir(string.Print()) == 0)
		return true;
	#endif

	return false;
}

bool DriveFolder::List(const Path & path, Reason::Structure::List<Reason::System::File> & files, Reason::Structure::List<Reason::System::Folder> & folders, bool recursive)
{
	folders.Destroy();
	files.Destroy();

	#ifdef REASON_PLATFORM_WINDOWS

	String find;
	struct _finddata_t fileInfo;

	if (!((Path&)path).IsEmpty())
	{
		if (!((Path&)path).Is("/")) find.Append(path);
		find.Append("/*"); 
	}

	long handle = _findfirst(find.Print(),&fileInfo);

	if (handle != -1)
	{
		do 
		{

			if ((strcmp(fileInfo.name,"..") == 0 || strcmp(fileInfo.name,".") == 0))
				continue;

			find.Release();
			if (!((Path&)path).Is("/")) find.Append(path);
			find << "/" << fileInfo.name;

			if ( ! (fileInfo.attrib & _A_SUBDIR) )
			{
				files.Append(find);			
			}
			else
			{	
				folders.Append(find);
			}
		}
		while( _findnext( handle, &fileInfo ) == 0 );

		_findclose(handle);			
	}
	else
	{
		return false;
	}

	#endif

	#ifdef REASON_PLATFORM_POSIX

	String find;
	DIR * dir = opendir(((Path&)path).Print());
	struct dirent * dirEntry=0;
	struct stat dirStat;

	if (dir)
	{
		while (true)  
		{
			dirEntry = readdir(dir);
			if (dirEntry == 0) break;

			if (strcmp(dirEntry->d_name,".") == 0 || strcmp(dirEntry->d_name,"..") == 0)
				continue;

			find.Release();
			if (!((Path&)path).Is("/")) find.Append(path);
			find << "/" << dirEntry->d_name;

			if (stat(find.Print(),&dirStat)==0)
			{
				if (!S_ISDIR(dirStat.st_mode))
				{
					files.Append(find);
				}
				else
				{	
					folders.Append(find);
				}
			}	
		}
	}
	else
	{
		return false;
	}

	#endif

	if (recursive)
	{
		Iterator<Reason::System::Folder> folder = folders.Iterate();
		for (folder.Forward();folder.Has();folder.Move())
		{
			folder().List(recursive);
		}
	}

	return true;
}

Stat DriveFolder::Status(const Path & path)
{
	return Reason::System::Stat(path.Data,path.Size);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}}}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//
// C++ Implementation: sound_lib
//
// Description:
//
//
// Author: Cavalaria Christophe <chris.cavalaria@free.fr>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#pragma warning(disable:4786)

#include "sound_lib.h"

SoundLib::SoundLib()
{
	// nothing to do yet
}

SoundLib::~SoundLib()
{
	for ( std::map<SoundIdType, SoundDesc>::iterator cur = sound_list.begin(); cur != sound_list.end(); ++cur )
	{
		Mix_FreeChunk ( cur->second.data );
	}
	sound_list.clear();
}

SoundDesc* SoundLib::GetSoundById ( const SoundIdType id ) const
{
	const SoundDesc& sound = sound_list.find ( id )->second;
	return const_cast<SoundDesc*> ( &sound );
}

void SoundLib::AddSound ( const SoundIdType id, std::string& filename_prefix )
{
	std::string path = "client/data/sounds/"+filename_prefix+".wav";
	SoundDesc s;
	s.data = Mix_LoadWAV ( path.c_str() );
	if ( s.data == NULL )
	{
		SoundLoadError ( ( "Unable to load WAV file " + path + " : " + Mix_GetError() ).c_str() );
	}
	sound_list[id] = s;
}

void SoundDesc::Play ( double volume )
{
	/* volume between 0 (silent) and 1 (loud) */

	Mix_Volume ( -1,int ( MIX_MAX_VOLUME * volume ) );
	Mix_PlayChannel ( -1, data, 0 );
}

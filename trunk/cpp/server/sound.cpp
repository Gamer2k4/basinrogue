//
// C++ Implementation: sound
//
// Description:
//
//
// Author: Cavalaria Christophe <chris.cavalaria@free.fr>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <string>
#include <sstream>

#pragma warning(disable:4786)

#include "sound.h"


Sound::Sound ( const std::string name ) : name ( name ), sound_id ( -1 )
{}


Sound::~Sound()
{}

int Sound::GetSoundId() const
{
	return sound_id;
}

SoundLib::~SoundLib()
{
	for ( unsigned ii=0; ii < sound_list.size(); ++ii )
		delete sound_list[ii];
	sound_list.clear();
}

Sound& SoundLib::AddSound ( const std::string name, const std::string filename_prefix )
{
	Sound* sound = new Sound ( name );
	int new_id = sound_list.size();
	sound_list.push_back ( sound );
	sound->sound_id = new_id;
	sound->filename_prefix = filename_prefix;
	return *sound;
}

void SoundLib::SendSoundLib ( NetworkCommandBuffer* buffer ) const
{
	for ( unsigned ii=0; ii < sound_list.size(); ++ii )
	{
		buffer->SendChar ( MSG_SENDSOUND );
		buffer->SendInt ( sound_list[ii]->GetSoundId() );
		buffer->SendString ( sound_list[ii]->filename_prefix );
	}
}

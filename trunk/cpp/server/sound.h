//
// C++ Interface: sound
//
// Description:
//
//
// Author: Cavalaria Christophe <chris.cavalaria@free.fr>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SOUND_H
#define SOUND_H

#include <string>
#include <vector>

#include "networkcommandbuffer.h"

class SoundLib;

/**
    @author Cavalaria Christophe <chris.cavalaria@free.fr>
*/
class Sound
{
		friend class SoundLib;
	private:
		const std::string name;
		int sound_id;
		std::string filename_prefix;
	public:
		Sound ( const std::string name );
		~Sound();

		int GetSoundId() const;
};

class SoundLib
{
	private:
		std::vector<Sound*> sound_list;
	public:
		~SoundLib();

		Sound& AddSound ( const std::string name, const std::string filename_prefix );
		void SendSoundLib ( NetworkCommandBuffer* buffer ) const;
};

#endif

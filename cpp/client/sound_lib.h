//
// C++ Interface: sound_lib
//
// Description:
//
//
// Author: Cavalaria Christophe <chris.cavalaria@free.fr>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef SOUND_LIB_H
#define SOUND_LIB_H

#include <string>
#include <map>
#include "SDL.h"
#include "SDL_mixer.h"

typedef unsigned long SoundIdType;

class SoundDesc
{
    public:
        Mix_Chunk* data;
        void Play(double volume); /* volume between 0 (silent) and 1 (loud) */
};

class SoundLib
{
    private:
        std::map<SoundIdType, SoundDesc> sound_list;
    public:
        SoundLib();
        ~SoundLib();

        SoundDesc* GetSoundById(const SoundIdType id) const;
        void AddSound(const SoundIdType id, std::string& filename_prefix);
};

class SoundLoadError
{
    public:
        const char* error;
        SoundLoadError(const char* arg) : error(arg) { }
};

#endif

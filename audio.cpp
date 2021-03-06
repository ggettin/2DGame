/*
 * Simple-SDL2-Audio
 *
 * Copyright 2016 Jake Besworth
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "audio.h"



/*
 * Add a sound to the end of the queue
 *
 * @param root      Root of queue
 * @param new       New Sound to add
 *
 */
static void addSound(Sound * root, Sound * newSound);

/*
 * Frees as many chained Sounds as given
 *
 * @param sound     Chain of sounds to free
 *
 */
static void freeSound(Sound * sound);

/*
 * Create a Sound object
 *
 * @param filename      Filename for the WAVE file to load
 * @param loop          Loop 0, ends after playing, 1 refreshes
 * @param volume        Volume, read playSound()
 *
 * @return returns a new Sound or NULL on failure
 *
 */
static Sound * createSound(const char * filename, uint8_t loop, int volume);

/*
 * Audio callback function for OpenAudioDevice
 *
 * @param userdata      Points to linked list of sounds to play, first being a placeholder
 * @param stream        Stream to mix sound into
 * @param len           Length of sound to play
 *
 */
static inline void audioCallback(void * userdata, uint8_t * stream, int len);

void audio::playSound(const char * filename, int volume)
{
    Sound * newSound;

    if(!gDevice->audioEnabled)
    {
        return;
    }

    newSound = createSound(filename, 0, volume);

    SDL_LockAudioDevice(gDevice->device);
    addSound((Sound *) (gDevice->want).userdata, newSound);

    SDL_UnlockAudioDevice(gDevice->device);
}

void audio::playMusic(const char * filename, int volume)
{
    Sound * global;
    Sound * newSound;
    uint8_t music;

    if(!gDevice->audioEnabled)
    {
        return;
    }

    music = 0;

    /* Create new music sound with loop */
    newSound = createSound(filename, 1, volume);

    /* Lock callback function */
    SDL_LockAudioDevice(gDevice->device);
    global = ((Sound *) (gDevice->want).userdata)->next;

    /* Find any existing musics, 0, 1 or 2 */
    while(global != NULL)
    {
        /* Phase out any current music */
        if(global->loop == 1 && global->fade == 0)
        {
            if(music)
            {
                global->length = 0;
                global->volume = 0;
            }

            global->fade = 1;
        }
        /* Set flag to remove any queued up music in favour of new music */
        else if(global->loop == 1 && global->fade == 1)
        {
            music = 1;
        }

        global = global->next;
    }

    addSound((Sound *) (gDevice->want).userdata, newSound);

    SDL_UnlockAudioDevice(gDevice->device);
}

audio::audio() : gDevice()
{
    Sound * global;
    gDevice = (PrivateAudioDevice *) calloc(1, sizeof(PrivateAudioDevice));

    if(!(SDL_WasInit(SDL_INIT_AUDIO) & SDL_INIT_AUDIO))
    {
        fprintf(stderr, "[%s: %d]Error: SDL_INIT_AUDIO not initialized\n", __FILE__, __LINE__);
        gDevice->audioEnabled = 0;
        return;
    }
    else
    {
        gDevice->audioEnabled = 1;
    }

    if(gDevice == NULL)
    {
        fprintf(stderr, "[%s: %d]Fatal Error: Memory c-allocation error\n", __FILE__, __LINE__);
        return;
    }

    SDL_memset(&(gDevice->want), 0, sizeof(gDevice->want));

    (gDevice->want).freq = AUDIO_FREQUENCY;
    (gDevice->want).format = AUDIO_FORMAT;
    (gDevice->want).channels = AUDIO_CHANNELS;
    (gDevice->want).samples = AUDIO_SAMPLES;
    (gDevice->want).callback = audioCallback;
    (gDevice->want).userdata = calloc(1, sizeof(Sound));

    global = (Sound *)(gDevice->want).userdata;

    if(global == NULL)
    {
        fprintf(stderr, "[%s: %d]Error: Memory allocation error\n", __FILE__, __LINE__);
        return;
    }

    global->buffer = NULL;
    global->next = NULL;

    /* want.userdata = new; */
    if((gDevice->device = SDL_OpenAudioDevice(NULL, 0, &(gDevice->want), NULL, SDL_AUDIO_ALLOW_ANY_CHANGE)) == 0)
    {
        fprintf(stderr, "[%s: %d]Warning: failed to open audio device: %s\n", __FILE__, __LINE__, SDL_GetError());
    }
    else
    {
        /* Unpause active audio stream */
        SDL_PauseAudioDevice(gDevice->device, 0);
    }
}

audio::~audio()
{
    if(gDevice->audioEnabled)
    {
        SDL_PauseAudioDevice(gDevice->device, 1);

        freeSound((Sound *) (gDevice->want).userdata);

        /* Close down audio */
        SDL_CloseAudioDevice(gDevice->device);
    }

    free(gDevice);
}

static Sound * createSound(const char * filename, uint8_t loop, int volume)
{
    Sound * newSound = (Sound *) calloc(1, sizeof(Sound));

    if(newSound == NULL)
    {
        fprintf(stderr, "[%s: %d]Error: Memory allocation error\n", __FILE__, __LINE__);
        return NULL;
    }

    newSound->next = NULL;
    newSound->loop = loop;
    newSound->fade = 0;
    newSound->volume = volume;

    if(SDL_LoadWAV(filename, &(newSound->audio), &(newSound->bufferTrue), &(newSound->lengthTrue)) == NULL)
    {
        fprintf(stderr, "[%s: %d]Warning: failed to open wave file: %s error: %s\n", __FILE__, __LINE__, filename, SDL_GetError());
        free(newSound);
        return NULL;
    }

    newSound->buffer = newSound->bufferTrue;
    newSound->length = newSound->lengthTrue;
    (newSound->audio).callback = NULL;
    (newSound->audio).userdata = NULL;

    return newSound;
}

static inline void audioCallback(void * userdata, uint8_t * stream, int len)
{
    Sound * sound = (Sound *) userdata;
    Sound * previous = sound;
    int tempLength;
    uint8_t music = 0;

    /* Silence the main buffer */
    SDL_memset(stream, 0, len);

    /* First one is place holder */
    sound = sound->next;

    while(sound != NULL)
    {
        if(sound->length > 0)
        {
            if(sound->fade == 1 && sound->loop == 1)
            {
                music = 1;
                sound->volume--;

                if(sound->volume == 0)
                {
                    sound->length = 0;
                }
            }

            if(music && sound->loop == 1 && sound->fade == 0)
            {
                tempLength = 0;
            }
            else
            {
                tempLength = ((uint32_t) len > sound->length) ? sound->length : (uint32_t) len;
            }

            SDL_MixAudioFormat(stream, sound->buffer, AUDIO_FORMAT, tempLength, sound->volume);

            sound->buffer += tempLength;
            sound->length -= tempLength;

            previous = sound;
            sound = sound->next;
        }
        else if(sound->loop == 1 && sound->fade == 0)
        {
            sound->buffer = sound->bufferTrue;
            sound->length = sound->lengthTrue;
        }
        else
        {
            previous->next = sound->next;
            SDL_FreeWAV(sound->bufferTrue);
            free(sound);

            sound = previous->next;
        }
    }
}

static void addSound(Sound * root, Sound * newSound)
{
    if(root == NULL)
    {
        return;
    }

    while(root->next != NULL)
    {
        root = root->next;
    }

    root->next = newSound;
}

static void freeSound(Sound * sound)
{
    Sound * temp;

    while(sound != NULL)
    {
        SDL_FreeWAV(sound->bufferTrue);

        temp = sound;
        sound = sound->next;

        free(temp);
    }
}

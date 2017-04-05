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

/*
 * audio.h
 *
 * All audio related functions go here
 *
 */
#ifndef SIMPLE_AUDIO_
#define SIMPLE_AUDIO_

#ifdef __cplusplus
extern "C"
{
#endif



/*
 * Native WAVE format
 *
 * On some GNU/Linux you can identify a files properties using:
 *      mplayer -identify music.wav
 *
 * On some GNU/Linux to convert any music to this or another specified format use:
 *      ffmpeg -i in.mp3 -acodec pcm_s16le -ac 2 -ar 48000 out.wav
 */
/* SDL_AudioFormat of files, such as s16 little endian */
#define AUDIO_FORMAT AUDIO_S16LSB

/* Frequency of the file */
#define AUDIO_FREQUENCY 48000

/* 1 mono, 2 stereo, 4 quad, 6 (5.1) */
#define AUDIO_CHANNELS 2

/* Specifies a unit of audio data to be used at a time. Must be a power of 2 */
#define AUDIO_SAMPLES 4096

/*
 * Queue structure for all loaded sounds
 *
 */
typedef struct sound
{
    uint32_t length;
    uint32_t lengthTrue;
    uint8_t * bufferTrue;
    uint8_t * buffer;
    uint8_t loop;
    uint8_t fade;
    uint8_t volume;

    SDL_AudioSpec audio;

    struct sound * next;
} Sound;

/*
 * Definition for the game global sound device
 *
 */
typedef struct privateAudioDevice
{
    SDL_AudioDeviceID device;
    SDL_AudioSpec want;
    uint8_t audioEnabled;
} PrivateAudioDevice;


class audio{
public:

		audio();
		~audio();

			/*
	 * Play a wave file currently must be S16LE format 2 channel stereo
	 *
	 * @param filename      Filename to open, use getAbsolutePath
	 * @param volume        Volume 0 - 128. SDL_MIX_MAXVOLUME constant for max volume
	 *
	 */
	void playSound(const char * filename, int volume);

	/*
	 * Plays a new music, only 1 at a time plays
	 *
	 * @param filename      Filename of the WAVE file to load
	 * @param volume        Volume read playSound for moree
	 *
	 */
	void playMusic(const char * filename, int volume);

private:
	PrivateAudioDevice *gDevice;



};

#ifdef __cplusplus
}
#endif

#endif

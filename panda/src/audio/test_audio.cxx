// Filename: test_audio.cxx
// Created by:  cary (24Sep00)
// 
////////////////////////////////////////////////////////////////////

#include <pandabase.h>
#include "audio.h"

#include "config_audio.h"
#include <ipc_traits.h>

int
main(int argc, char* argv[]) {
  for (int i=1; i<argc; ++i)
    if (! AudioPool::verify_sound(argv[i])) {
      audio_cat->fatal() << "could not locate '" << argv[i] << "'" << endl;
      exit(-1);
    }

  AudioManager::spawn_update();
  {
    {
      PT(AudioSound) tester = AudioPool::load_sound(argv[1]);
      AudioManager::play(tester);
      AudioPool::release_all_sounds();
      cerr << "all sounds but 1 released" << endl;
    }
    cerr << "all sounds released" << endl;
  }

  /*
  if (! AudioPool::verify_sound("test.mp3")) {
    audio_cat->fatal() << "could not locate 'test.mp3'" << endl;
    exit(-1);
  }
  AudioSound* sample = AudioPool::load_sound("test.mp3");
  audio_cat->info() << "test.wav is " << sample->length() << " sec long"
                     << endl;
  audio_cat->info() << "Playing test.wav" << endl;
  AudioManager::play(sample);
  while (sample->status() == AudioSound::PLAYING) {
    AudioManager::update();
    ipc_traits::sleep(0, 1000000);
  }

  //   AudioMidi foo("test.midi");
  if (! AudioPool::verify_sound("test.midi")) {
    audio_cat->fatal() << "could not locate 'test.midi'" << endl;
    exit(-1);
  }
  AudioSound* music = AudioPool::load_sound("test.midi");
  audio_cat->info() << "Playing test.midi" << endl;
  AudioManager::play(music);
  while (music->status() == AudioSound::PLAYING) {
    AudioManager::update();
    ipc_traits::sleep(0, 1000000);
  }
  */
  return 0;
}

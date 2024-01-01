#include <iostream>
#include <fstream>
#include <vector>
#include "models/wav.cpp"
#include <SFML/Graphics.hpp>

using namespace std;


string get_song_name() {
    string song_name = "africa-toto.wav";
    //string song_name = "sample-file-3.wav";
    cout << "Hello" << "      " << '\n';
    cout << "Using song: " << song_name << '\n';
    cout << '\n';
    return song_name;
}


int main() {

    string song_name = get_song_name();

    ifstream song_in(song_name, ios::binary);

   wavFile wav_song(song_in);
   song_in.close();

    if (wav_song.is_valid_wav()) {
        cout << "Initial wav checks are correct" << '\n';
    }
    else {
        cout << "Not initially a valid wav file" << '\n';
        return 1;
    }

    wav_song.reverse_audio();

    if (wav_song.is_valid_wav()) {
        cout << "Wav checks are still correct" << '\n';
    }
    else {
        cout << "Not a valid wav file after reversal" << '\n';
        return 1;
    }

    ofstream song_out("HOME_reversed_new.wav", ios::binary);

    if (song_out.is_open()) {
        song_out.write(wav_song.get_header().data(), wav_song.get_header().size());
        song_out.write(wav_song.get_audio_data().data(), wav_song.get_audio_data().size());
        song_out.close();
    } else {
        cerr << "Failed to write file";
        cerr << "We failed";
        return 1;
    }
    return 0;
}



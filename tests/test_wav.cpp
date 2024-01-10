#include "../models/wav.cpp"
//#include "../models/flac.cpp"
#include <iostream>
#include <fstream>

using namespace std;

string get_song_name() {
    string song_name = "HOME_wav.wav";
    //string song_name = "sample-file-3.wav";
    //string song_name = "africa-toto.wav";
    //string song_name = "HOME_flac.flac";
    cout << "Hello" << "      " << '\n';
    cout << "Using song: " << song_name << '\n';
    cout << '\n';
    return song_name;
}

int test_wav_class() {
    string song_name = get_song_name();

    ifstream song_in(song_name, ios::binary);

    song_name = song_name.substr(0, song_name.find("."));

    wavFile wav_song(song_in, song_name);
    song_in.close();


    size_t currentSize = static_cast<uint8_t>(wav_song.get_header()[4]) |
                         (static_cast<uint8_t>(wav_song.get_header()[5]) << 8) |
                         (static_cast<uint8_t>(wav_song.get_header()[6]) << 16) |
                         (static_cast<uint8_t>(wav_song.get_header()[7]) << 24);

    size_t currentFmtSize = static_cast<uint8_t>(wav_song.get_fmt_data()[4]) |
                         (static_cast<uint8_t>(wav_song.get_fmt_data()[5]) << 8) |
                         (static_cast<uint8_t>(wav_song.get_fmt_data()[6]) << 16) |
                         (static_cast<uint8_t>(wav_song.get_fmt_data()[7]) << 24);

    size_t currentDataSize = static_cast<uint8_t>(wav_song.get_audio_data()[4]) |
                         (static_cast<uint8_t>(wav_song.get_audio_data()[5]) << 8) |
                         (static_cast<uint8_t>(wav_song.get_audio_data()[6]) << 16) |
                         (static_cast<uint8_t>(wav_song.get_audio_data()[7]) << 24);

    cout << currentSize << " " << currentFmtSize << " " << currentDataSize << '\n';

    // cout << "HEADER : ";
    // for (char i : wav_song.get_header()) {
    //     cout << i << " ";
    // }
    // cout << '\n';

    // cout << "fmt : ";
    // for (char i : wav_song.get_fmt_data()) {
    //     cout << i << " ";
    // }
    // cout << '\n';

    // cout << "data: ";
    // int i = 0;
    // for (char i : wav_song.get_audio_data()) {
    //     cout << i << " ";
    //     i++;
    //     if (i == 20) {
    //         break;
    //     }
    // }
    // cout << '\n';

    if (wav_song.is_valid_wav()) {
        cout << "Initial wav checks are correct" << '\n';
    }
    else {
        cout << "Not initially a valid wav file" << '\n';
        return 1;
    }

    //wav_song.reverse_audio();
    wav_song.make_8bit();
    //wav_song.apply_dither();
    //wav_song.normalize_volume(50);
    //wav_song.trim_file(30, false);

    if (wav_song.is_valid_wav()) {
        cout << "Wav checks are still correct" << '\n';
    }
    else {
        cout << "Not a valid wav file after reversal" << '\n';
        return 1;
    }

    wav_song.output_song();



    return 0;
}


// int test_flac_class() {

//     string song_name = get_song_name();

//     ifstream song_in(song_name, ios::binary);

//     song_name = song_name.substr(0, song_name.find("."));

//     flacFile(song_in, song_name);

//     return 0;
// }


int main() {

    assert(test_wav_class() == 0);

    return 0;
}
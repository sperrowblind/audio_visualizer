#include "../models/wav.cpp"
#include <iostream>
#include <fstream>

using namespace std;

string get_song_name() {
    //string song_name = "HOME_wav.wav";
    string song_name = "sample-file-3.wav";
    //string song_name = "africa-toto.wav";
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

    printf("Audio Format: %d\n", wav_song.get_fmt_chunk().audioFormat);
    printf("Number of Channels: %d\n", wav_song.get_fmt_chunk().numChannels);
    printf("Sample Rate: %d\n", wav_song.get_fmt_chunk().sampleRate);
    printf("Byte Rate: %d\n", wav_song.get_fmt_chunk().byteRate);
    printf("Block Align: %d\n", wav_song.get_fmt_chunk().blockAlign);
    printf("Bits per Sample: %d\n", wav_song.get_fmt_chunk().bitsPerSample);

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

    cout << "HEADER : ";
    for (char i : wav_song.get_header()) {
        cout << i << " ";
    }
    cout << '\n';

    cout << "fmt : ";
    for (char i : wav_song.get_fmt_data()) {
        cout << i << " ";
    }
    cout << '\n';

    cout << "data: ";
    int i = 0;
    for (char i : wav_song.get_audio_data()) {
        cout << i << " ";
        i++;
        if (i == 20) {
            break;
        }
    }
    cout << '\n';

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






    wav_song.output_song();

    // ofstream song_out("HOME_reversed_new.wav", ios::binary);

    // if (song_out.is_open()) {


    //     song_out.write(wav_song.get_header().data(), wav_song.get_header().size());
    //     song_out.write(wav_song.get_fmt_data().data(), wav_song.get_header().size());
    //     song_out.write(wav_song.get_audio_data().data(), wav_song.get_audio_data().size());
    //     song_out.close();
    // } else {
    //     cerr << "Failed to write file";
    //     cerr << "We failed";
    //     return 1;
    // }
    // song_out.close();

    return 0;
}
int main() {

    assert(test_wav_class() == 0);

    return 0;
}
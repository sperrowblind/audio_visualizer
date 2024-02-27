#include "../models/wav.hpp"

using namespace std;

string get_song_name(std::string song_in) {
    std::string song_name = song_in;
    std::cout << "Hello" << "      " << '\n';
    std::cout << "Using song: " << song_name << '\n';
    std::cout << '\n';
    return song_name;
}


wavFile load_file(std::string song_name_in) {
    std::string song_name = get_song_name(song_name_in);
    ifstream song_in(song_name, ios::binary);

    song_name = song_name.substr(0, song_name.find("."));

    wavFile wav_song(song_in, song_name);
    song_in.close();

    return wav_song;
}

int test_trim_file(wavFile &wavFile_in, int second_to_trim_in, bool is_leading_in) {
    int original_duration = wavFile_in.get_audio_file_duration();
    std::cout << "Original duration of song: " << original_duration << '\n';
    wavFile_in.trim_file(second_to_trim_in, is_leading_in);
    if (wavFile_in.get_audio_file_duration() != original_duration - second_to_trim_in) {
        std::cout << "Duration of song inconsistent after trim: " << wavFile_in.get_audio_file_duration() << '\n';
        return 1;
    }
    return 0;
}

int test_wav_class() {

    std::string first_audio = "sample-file-3.wav";
    std::string first_audio_name = first_audio.substr(0, first_audio.find("."));

    wavFile first_audio_file = load_file(first_audio);

    int testTrimSeconds[5] = {1, 10, 12, 20, 2};

    for (auto i : testTrimSeconds) {
        if (test_trim_file(first_audio_file, i, true) == 1) {
            return 1;
        }
    }

    std::cout << "Passed trimming file" << '\n';

    std::string second_audio = "HOME_wav.wav";
    std::string second_audio_name = second_audio.substr(0, second_audio.find("."));

    wavFile second_audio_file = load_file(second_audio);

    if (second_audio_file.is_valid_wav()) {
        std::cout << "Initial wav checks are correct" << '\n';
    }
    else {
        std::cout << "Not initially a valid wav file" << '\n';
        return 1;
    }

    //first_audio_file.reverse_audio();
    second_audio_file.make_8bit();
    //wav_song.apply_dither();
    //wav_song.normalize_volume(50);
    //wav_song.trim_file(30, false);


    //first_audio_file.output_song();
    second_audio_file.output_song();


    return 0;
}


int main() {

    assert(test_wav_class() == 0);

    return 0;
}


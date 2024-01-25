#include <cstring>
#include <fstream>
#include <vector>
#include <iostream>
#include <cmath>
#include <random>
#include "wav_fmt.cpp"

using namespace std;


class wavFile {
    public:

        wavFile();

        wavFile(vector<char> header_in, vector<char> audio_in, uint32_t song_size_in, string song_name_in);

        wavFile(ifstream& input_song, string song_name_in);

        vector<char> get_header();

        vector<char> get_audio_data();

        vector<char> get_fmt_data();

        fmtChunk get_fmt_chunk();

        double get_audio_file_duration();

        void updateFileSize(size_t subtractSize);

        bool is_valid_wav();

        void reverse_audio();

        void trim_file(int seconds_to_trim, bool is_leading);

        void apply_dither();

        void normalize_volume(double target_peak);

        void make_8bit();

        void output_song();


    private:
        vector<char> header;
        vector<char> fmt_data;
        vector<char> audio_data;
        uint32_t song_size;
        string song_name;

        fmtChunk fmt_chunk;

};


#include <cstring>
#include <fstream>
#include <vector>
#include <iostream>
#include "wav_fmt.cpp"

using namespace std;


class wavFile {
    public:

        wavFile(vector<char> header_in, vector<char> audio_in, uint32_t song_size_in, string song_name_in) {
            header = header_in;
            audio_data = audio_in;
            song_size = song_size_in;
            song_name = song_name_in;
        }

        wavFile(ifstream& input_song, string song_name_in) {

            song_name = song_name_in;

            if (!input_song.is_open()) {
                cout << "Something went wrong. The song isn't open. :(";
                return;
            }
            vector<char> chunk(4);
            size_t to_subtract = 0;
            while (input_song.read(chunk.data(), chunk.size())) {

                if (strncmp(chunk.data(), "RIFF", 4) == 0) {
                    cout << "found riff \n";
                    header.insert(header.end(), chunk.begin(), chunk.end());
                    input_song.read(chunk.data(), chunk.size());
                    header.insert(header.end(), chunk.begin(), chunk.end());
                    input_song.read(chunk.data(), chunk.size());
                    header.insert(header.end(), chunk.begin(), chunk.end());
                    continue;
                }
                else if (strncmp(chunk.data(), "data", 4) == 0) {
                    cout << "Found data \n";
                    audio_data.insert(audio_data.end(), chunk.begin(), chunk.end());
                    input_song.read(chunk.data(), chunk.size());
                    audio_data.insert(audio_data.end(), chunk.begin(), chunk.end());


                    int chunkSize = *reinterpret_cast<int*>(&chunk[0]);
                    if (chunkSize % 2 != 0) {
                        chunkSize++;
                    }

                    vector<char> chunkContentAudio(chunkSize);
                    input_song.read(chunkContentAudio.data(), chunkContentAudio.size());
                    audio_data.insert(audio_data.end(), chunkContentAudio.begin(), chunkContentAudio.end());
                    break;
                }
                else if (strncmp(chunk.data(), "fmt", 3) == 0) {
                    cout << "found fmt data \n";
                    fmt_data.insert(fmt_data.end(), chunk.begin(), chunk.end());
                    input_song.read(chunk.data(), chunk.size());
                    fmt_data.insert(fmt_data.end(), chunk.begin(), chunk.end());

                    int chunkSize = *reinterpret_cast<int*>(&chunk[0]);
                    if (chunkSize % 2 != 0) {
                        chunkSize++;
                    }

                    vector<char> chunkContent(chunkSize);
                    input_song.read(chunkContent.data(), chunkContent.size());
                    fmt_data.insert(fmt_data.end(), chunkContent.begin(), chunkContent.end());
                    continue;
                }

                to_subtract += 8;

                input_song.read(chunk.data(), chunk.size());

                int chunkSize = *reinterpret_cast<int*>(&chunk[0]);
                if (chunkSize % 2 != 0) {
                    chunkSize++;
                }

                vector<char> chunkContent(chunkSize);
                input_song.read(chunkContent.data(), chunkContent.size());

                to_subtract += chunkSize;
            }
            song_size = static_cast<uint32_t>(input_song.tellg()) - 8 - to_subtract;

            cout << "look at this \n";
            cout << to_subtract;

            updateFileSize(to_subtract);

            memcpy(&fmt_chunk, fmt_data.data(), sizeof(fmt_chunk));
        }

        vector<char> get_header() {
            return header;
        }

        vector<char> get_audio_data() {
            return audio_data;
        }

        vector<char> get_fmt_data() {
            return fmt_data;
        }

        fmtChunk get_fmt_chunk() {
            return fmt_chunk;
        }


        void updateFileSize(size_t subtractSize) {

            size_t currentSize = static_cast<uint8_t>(header[4]) |
                         (static_cast<uint8_t>(header[5]) << 8) |
                         (static_cast<uint8_t>(header[6]) << 16) |
                         (static_cast<uint8_t>(header[7]) << 24);

            size_t newFileSize = currentSize - subtractSize;

            header[4] = static_cast<char>(newFileSize & 0xFF);
            header[5] = static_cast<char>((newFileSize >> 8) & 0xFF);
            header[6] = static_cast<char>((newFileSize >> 16) & 0xFF);
            header[7] = static_cast<char>((newFileSize >> 24) & 0xFF);
        }


        bool is_valid_wav() {
            if (strncmp(header.data(), "RIFF", 4) != 0) {
                cout << "RIFF issue" << '\n';
                return false;
            }

            // uint32_t fileSizeFromHeader = *reinterpret_cast<uint32_t*>(&header[4]);
            // if (fileSizeFromHeader != song_size) {
            //     cout << "fileSizeFromHeader: " << fileSizeFromHeader << '\n';
            //     cout << "song_size: " << song_size << '\n';
            //     return false;
            // }

            if (strncmp(&header[8], "WAVE", 4) != 0) {
                cout << "WAVE issue" << '\n';
                return false;
            }

            if (strncmp(&fmt_data[0], "fmt ", 4) != 0) {
                cout << "fmt issue" << '\n';
                return false;
            }

            return true;
        }

        void reverse_audio() {
            for (size_t i = 8; i < audio_data.size() / 2; i += 2) {
                swap(audio_data[i], audio_data[audio_data.size() - 2 - i]);
                swap(audio_data[i + 1], audio_data[audio_data.size() - 1 - i]);
            }
        }

        void output_song() {
            string output_song_name = "new_" + song_name + ".wav";
            ofstream song_out(output_song_name, ios::binary);

            if (song_out.is_open()) {
                song_out.write(header.data(), header.size());
                song_out.write(fmt_data.data(), fmt_data.size());
                song_out.write(audio_data.data(), audio_data.size());
                song_out.close();
            } else {
                cerr << "Failed to write file";
                cerr << "We failed";
            }
            song_out.close();
        }


    private:
        vector<char> header;
        vector<char> fmt_data;
        vector<char> audio_data;
        uint32_t song_size;
        string song_name;

        fmtChunk fmt_chunk;

};


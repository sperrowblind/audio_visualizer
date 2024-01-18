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

        wavFile() {
            song_size = 0;
            song_name = "default";
        }

        wavFile(vector<char> header_in, vector<char> audio_in, uint32_t song_size_in, string song_name_in) {
            header = header_in;
            audio_data = audio_in;
            song_size = song_size_in;
            song_name = song_name_in;
        }

        wavFile(ifstream& input_song, string song_name_in) {

            song_name = song_name_in;

            if (!input_song.is_open()) {
                std::cerr << "Something went wrong. The song isn't open. :(";
                return;
            }
            vector<char> chunk(4);
            size_t to_subtract = 0;
            while (input_song.read(chunk.data(), chunk.size())) {

                if (strncmp(chunk.data(), "RIFF", 4) == 0) {
                    //cout << "found riff \n";
                    header.insert(header.end(), chunk.begin(), chunk.end());
                    input_song.read(chunk.data(), chunk.size());
                    header.insert(header.end(), chunk.begin(), chunk.end());
                    input_song.read(chunk.data(), chunk.size());
                    header.insert(header.end(), chunk.begin(), chunk.end());
                    continue;
                }
                else if (strncmp(chunk.data(), "data", 4) == 0) {
                    //cout << "Found data \n";
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
                    //cout << "found fmt data \n";
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

            //cout << "look at this \n";
            //cout << to_subtract;

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

            memcpy(&fmt_chunk, fmt_data.data(), sizeof(fmt_chunk));
            printf("Audio Format: %d\n", fmt_chunk.audioFormat);
            printf("Number of Channels: %d\n", fmt_chunk.numChannels);
            printf("Sample Rate: %d\n", fmt_chunk.sampleRate);
            printf("Byte Rate: %d\n", fmt_chunk.byteRate);
            printf("Block Align: %d\n", fmt_chunk.blockAlign);
            printf("Bits per Sample: %d\n", fmt_chunk.bitsPerSample);

            return true;
        }

        void reverse_audio() {
            for (size_t i = 8; i < audio_data.size() / 2; i += 2) {
                swap(audio_data[i], audio_data[audio_data.size() - 2 - i]);
                swap(audio_data[i + 1], audio_data[audio_data.size() - 1 - i]);
            }
        }

        void trim_file(double seconds_to_trim, bool is_leading) {
            int samples_to_trim = seconds_to_trim * fmt_chunk.sampleRate;
            if (is_leading) {
                if (samples_to_trim > 0 && samples_to_trim <= audio_data.size()) {
                    audio_data.erase(audio_data.begin() + 8, audio_data.begin() + 8 + samples_to_trim * fmt_chunk.bitsPerSample);
                }
            }
            else {
                if (samples_to_trim > 0 && samples_to_trim <= audio_data.size()) {
                    audio_data.erase(audio_data.end() - samples_to_trim * fmt_chunk.bitsPerSample, audio_data.end());
                }
            }
            updateFileSize(samples_to_trim);
            // need to update header size here
        }

        void apply_dither() {
            std::default_random_engine generator;
            std::uniform_int_distribution<int> distribution(-1, 1);

            for (size_t i = 8; i < audio_data.size(); ++i) {
                int dither_value = distribution(generator);
                audio_data[i] += static_cast<char>(dither_value);
            }
        }

        void normalize_volume(double target_peak) {
            short max_amplitude = 0;
            int bytesPerSample = (fmt_chunk.numChannels * fmt_chunk.bitsPerSample) / 8;
            for (size_t i = 8; i < audio_data.size(); i += bytesPerSample) {
                short sample = static_cast<short>((audio_data[i + 1] << 8) | (audio_data[i] & 0xFF));
                if (abs(sample) > max_amplitude) {
                    max_amplitude = abs(sample);
                }
            }

            double normalization_factor = (target_peak / max_amplitude);

            for (size_t i = 8; i < audio_data.size(); i += bytesPerSample) {
                short sample = static_cast<short>((audio_data[i + 1] << 8) | (audio_data[i] & 0xFF));
                sample = static_cast<short>(sample * normalization_factor);

                if (sample > 32767) {
                    sample = 32767;
                } else if (sample < -32768) {
                    sample = -32768;
                }

                audio_data[i] = static_cast<char>(sample & 0xFF);
                audio_data[i + 1] = static_cast<char>((sample >> 8) & 0xFF);
            }
        }


        void make_8bit() {
            if (fmt_chunk.byteRate == 8) {
                std::cerr << "It appears the file is already 8-bit" << '\n';
                std::cout << "It appears the file is already 8-bit" << '\n';
                return;
            }
            apply_dither();
            int bytesPerSample = (fmt_chunk.numChannels * fmt_chunk.bitsPerSample) / 8;
            int totalSamples = (audio_data.size() - 8) / bytesPerSample;
            printf("bytes per sample: %d\n", bytesPerSample);
            for (size_t i = 8; i < audio_data.size(); i += bytesPerSample) {
                short sample = static_cast<short>((audio_data[i + 1] << 8) | (audio_data[i] & 0xFF));

                char scaledSample = static_cast<char>((sample / 32768.0) * 127.0) + 128;

                audio_data[i / (fmt_chunk.bitsPerSample / 8)] = scaledSample;
            }
            fmt_chunk.bitsPerSample = 8;
            fmt_chunk.blockAlign = fmt_chunk.numChannels * fmt_chunk.bitsPerSample / 8;
            fmt_data[22] = 8;
            fmt_data[20] = fmt_chunk.blockAlign;

            int audio_size = totalSamples * fmt_chunk.numChannels * fmt_chunk.bitsPerSample / 8;
            printf("what the size should be: %d\n", audio_size);

            audio_data.resize(audio_size + 8);

            updateFileSize(audio_size);

            //normalize_volume(50);
        }


        void output_song() {
            string output_song_name = "new_" + song_name + ".wav";
            ofstream song_out(output_song_name, ios::binary);

            if (song_out.is_open()) {
                song_out.write(header.data(), header.size());
                //printf("Header size: %d\n", header.size());
                song_out.write(fmt_data.data(), fmt_data.size());
                //printf("Fmt size: %d\n", fmt_data.size());
                song_out.write(audio_data.data(), audio_data.size());
                //printf("Audio_data size: %d\n", audio_data.size());
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


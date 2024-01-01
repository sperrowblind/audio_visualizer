#include <cstring>
#include <fstream>
#include <vector>


using namespace std;


class wavFile {
    public:

        wavFile(vector<char> header_in, vector<char> audio_in, uint32_t song_size_in) {
            header = header_in;
            audio_data = audio_in;
            song_size = song_size_in;
        }

        wavFile(ifstream& input_song) {

            if (!input_song.is_open()) {
                cout << "Something went wrong. The song isn't open. :(";
                return;
            }
            vector<char> chunk(4);
            while (input_song.read(chunk.data(), chunk.size())) {
                header.insert(header.end(), chunk.begin(), chunk.end());

                if (strncmp(chunk.data(), "RIFF", 4) == 0) {
                    input_song.read(chunk.data(), chunk.size());
                    header.insert(header.end(), chunk.begin(), chunk.end());
                    input_song.read(chunk.data(), chunk.size());
                    header.insert(header.end(), chunk.begin(), chunk.end());
                    continue;
                }
                else if (strncmp(chunk.data(), "data", 4) == 0) {
                    cout << "Found data \n";
                    input_song.read(chunk.data(), chunk.size());
                    header.insert(header.end(), chunk.begin(), chunk.end());

                    audio_data.assign(
                        (istreambuf_iterator<char>(input_song)),
                        (istreambuf_iterator<char>()));
                    break;
                }

                input_song.read(chunk.data(), chunk.size());
                header.insert(header.end(), chunk.begin(), chunk.end());

                int chunkSize = *reinterpret_cast<int*>(&chunk[0]);
                if (chunkSize % 2 != 0) {
                    chunkSize++;
                }

                vector<char> chunkContent(chunkSize);
                input_song.read(chunkContent.data(), chunkContent.size());
                header.insert(header.end(), chunkContent.begin(), chunkContent.end());
            }
            song_size = static_cast<uint32_t>(input_song.tellg()) - 8;
        }

        vector<char> get_header() {
            return header;
        }

        vector<char> get_audio_data() {
            return audio_data;
        }

        bool is_valid_wav() {
            if (strncmp(header.data(), "RIFF", 4) != 0) {
                return false;
            }

            uint32_t fileSizeFromHeader = *reinterpret_cast<uint32_t*>(&header[4]);
            if (fileSizeFromHeader != song_size) {
                return false;
            }

            if (strncmp(&header[8], "WAVE", 4) != 0) {
                return false;
            }

            if (strncmp(&header[12], "fmt ", 4) != 0) {
                return false;
            }

            return true;
        }

        void reverse_audio() {
            for (size_t i = 0; i < audio_data.size() / 2; i += 2) {
                swap(audio_data[i], audio_data[audio_data.size() - 2 - i]);
                swap(audio_data[i + 1], audio_data[audio_data.size() - 1 - i]);
            }
        }


    private:
        vector<char> header;
        vector<char> audio_data;
        uint32_t song_size;


};
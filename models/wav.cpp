#include "wav.hpp"

using namespace std;



wavFile::wavFile() {
    song_size = 0;
    song_name = "default";
}

wavFile::wavFile(vector<char> header_in, vector<char> audio_in, uint32_t song_size_in, std::string song_name_in) {
    header = header_in;
    audio_data = audio_in;
    song_size = song_size_in;
    song_name = song_name_in;
}

wavFile::wavFile(ifstream& input_song, std::string song_name_in) {

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

    updateFileSize(to_subtract);

    memcpy(&fmt_chunk, fmt_data.data(), sizeof(fmt_chunk));
}

vector<char> wavFile::get_header() {
    return header;
}

vector<char> wavFile::get_audio_data() {
    return audio_data;
}

vector<char> wavFile::get_fmt_data() {
    return fmt_data;
}

fmtChunk wavFile::get_fmt_chunk() {
    return fmt_chunk;
}

double wavFile::get_audio_file_duration() {
    size_t bytesPerSample = (fmt_chunk.numChannels * fmt_chunk.bitsPerSample) / 8;
    size_t totalSamples = (audio_data.size() - 8) / bytesPerSample;
    double duration = static_cast<double>(totalSamples) / fmt_chunk.sampleRate;
    return floor(duration);
}

void wavFile::updateFileSize(size_t subtractSize) {
    *reinterpret_cast<uint32_t*>(&header[4]) -= static_cast<uint32_t>(subtractSize);
}


bool wavFile::is_valid_wav() {
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
    // printf("Audio Format: %d\n", fmt_chunk.audioFormat);
    // printf("Number of Channels: %d\n", fmt_chunk.numChannels);
    // printf("Sample Rate: %d\n", fmt_chunk.sampleRate);
    // printf("Byte Rate: %d\n", fmt_chunk.byteRate);
    // printf("Block Align: %d\n", fmt_chunk.blockAlign);
    // printf("Bits per Sample: %d\n", fmt_chunk.bitsPerSample);

    return true;
}

void wavFile::reverse_audio() {
    for (size_t i = 8; i < audio_data.size() / 2; i += 2) {
        swap(audio_data[i], audio_data[audio_data.size() - 2 - i]);
        swap(audio_data[i + 1], audio_data[audio_data.size() - 1 - i]);
    }
}

void wavFile::trim_file(int seconds_to_trim, bool is_leading) {
    int samples_to_trim = seconds_to_trim * fmt_chunk.sampleRate;
    int bytes_to_trim = floor(samples_to_trim * (fmt_chunk.bitsPerSample / 8) * static_cast<int>(fmt_chunk.numChannels));
    if (is_leading) {
        if (bytes_to_trim > 0 && bytes_to_trim < audio_data.size()) {
            audio_data.erase(audio_data.begin() + 8, audio_data.begin() + 8 + bytes_to_trim);
        }
    }
    else {
        if (bytes_to_trim > 0 && bytes_to_trim < audio_data.size()) {
            audio_data.resize(audio_data.size() - bytes_to_trim);
        }
    }
    updateFileSize(bytes_to_trim);
}

void wavFile::apply_dither() {
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(-1, 1);

    for (size_t i = 8; i < audio_data.size(); ++i) {
        int dither_value = distribution(generator);
        audio_data[i] += static_cast<char>(dither_value);
    }
}

void wavFile::normalize_volume(double target_peak) {
    short max_amplitude = 0;
    size_t bytesPerSample = (fmt_chunk.numChannels * fmt_chunk.bitsPerSample) / 8;
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


void wavFile::make_8bit() {
    if (fmt_chunk.bitsPerSample == 8) {
        std::cerr << "It appears the file is already 8-bit" << '\n';
        std::cout << "It appears the file is already 8-bit" << '\n';
        return;
    }
    size_t numSamples = (audio_data.size()-8) / (fmt_chunk.bitsPerSample / 8);
    for (size_t i = 8; i < numSamples; ++i) {
        short sample = *(reinterpret_cast<short*>(&audio_data[i * (fmt_chunk.bitsPerSample / 8)]));
        int sample8bit = static_cast<int8_t>(sample >> (fmt_chunk.bitsPerSample - 8));
        sample8bit = std::min(std::max(sample8bit, -128), 127);

        audio_data[i * (fmt_chunk.bitsPerSample / 8)] = static_cast<char>(sample8bit);
    }

    fmt_chunk.bitsPerSample = 8;
    fmt_chunk.blockAlign = fmt_chunk.numChannels * fmt_chunk.bitsPerSample / 8;
    fmt_data[22] = 8;
    fmt_data[20] = fmt_chunk.blockAlign;

    int audio_size = numSamples * fmt_chunk.bitsPerSample / 8;

    audio_data.resize(audio_size * 2);

    updateFileSize(audio_size * 2);

    normalize_volume(50);

}


void wavFile::output_song() {
    std::string output_song_name = "new_" + song_name + ".wav";
    ofstream song_out(output_song_name, ios::binary);

    if (song_out.is_open()) {
        song_out.write(header.data(), header.size());
        //printf("Header size: %d\n", header.size());
        song_out.write(fmt_data.data(), fmt_data.size());
        //printf("Fmt size: %d\n", fmt_data.size());
        song_out.write(audio_data.data(), audio_data.size());
        //printf("Audio_data size: %d\n", audio_data.size());
        song_out.close();
    }
    else {
        cerr << "Failed to write file";
        cerr << "We failed";
    }
    song_out.close();
}


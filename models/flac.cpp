#include <cstring>
#include <fstream>
#include <vector>
#include <iostream>
#include "wav.cpp"
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>




class flacFile {
    public:

        // flacFile(std::ifstream& input_song, std::string song_name_in) {

        //     song_name = song_name_in;

        //     if (!input_song.is_open()) {
        //         std::cout << "Something went wrong. The song isn't open. :(";
        //         return;
        //     }
        //     std::vector<char> chunk(50);

        //     input_song.read(chunk.data(), chunk.size());

        //     for (char i : chunk) {
        //         std::cout << i << " ";
        //     }


        // }

        flacFile(std::ifstream& flac_file_in, std::string song_name_in) {
            flac_to_wav_conversion(flac_file_in, song_name_in);
        }

        void flac_to_wav_conversion(std::ifstream& flac_file_in, std::string song_name_in) {
            AVFormatContext* formatCtx = nullptr;
            if (avformat_open_input(&formatCtx, "dummy", nullptr, nullptr) < 0) {
                std::cerr << "Error: Could not open input context." << std::endl;
                return;
            }

            if (avformat_find_stream_info(formatCtx, nullptr) < 0) {
                std::cerr << "Error: Could not find stream information." << std::endl;
                avformat_close_input(&formatCtx);
                return;
            }
            const AVCodec *codec_temp = avcodec_find_encoder(AV_CODEC_ID_H264);
            AVCodec *codec = const_cast<AVCodec *>(codec_temp);
            if (!codec) {
                std::cerr << "Error: FLAC decoder not found." << std::endl;
                avformat_close_input(&formatCtx);
                return;
            }

            AVCodecContext* codecCtx = avcodec_alloc_context3(codec);
            if (!codecCtx) {
                std::cerr << "Error: Could not allocate codec context." << std::endl;
                avformat_close_input(&formatCtx);
                return;
            }

            if (avcodec_open2(codecCtx, codec, nullptr) < 0) {
                std::cerr << "Error: Could not open codec." << std::endl;
                avcodec_free_context(&codecCtx);
                avformat_close_input(&formatCtx);
                return;
            }

            AVPacket packet;
            //av_init_packet(&packet);
            packet.data = nullptr;
            packet.size = 0;

            // Read packets and decode FLAC
            while (av_read_frame(formatCtx, &packet) >= 0) {
                if (packet.stream_index >= 0) {
                    if (avcodec_send_packet(codecCtx, &packet) < 0) {
                        std::cerr << "Error: Failed to send packet to decoder." << std::endl;
                        break;
                    }

                    AVFrame* frame = av_frame_alloc();
                    if (!frame) {
                        std::cerr << "Error: Failed to allocate frame." << std::endl;
                        av_packet_unref(&packet);
                        break;
                    }

                    if (avcodec_receive_frame(codecCtx, frame) >= 0) {
                        std::cout << frame;
                        //std::cout << frame->key_frame;
                        // Process decoded frame (extract header and audio data)
                        if (*frame->data[0] == 1) {
                            // Extract header data (RIFF and fmt chunks)
                            // Replace this with your code to extract the header information

                            std::vector<char> headerData; // Replace with actual header data
                            std::vector<char> audioData;  // Replace with actual audio data

                            // Create a WAV object from the extracted data
                            wavFile newFile(headerData, audioData, 10, song_name_in);
                            converted_wav = newFile;
                        }
                    }

                    av_frame_free(&frame);
                }

                av_packet_unref(&packet);
            }

            avcodec_free_context(&codecCtx);
            avformat_close_input(&formatCtx);
        }





    private:

        wavFile converted_wav;


        std::vector<char> header;
        std::vector<char> fmt_data;
        std::vector<char> audio_data;
        uint32_t song_size;
        std::string song_name;


};
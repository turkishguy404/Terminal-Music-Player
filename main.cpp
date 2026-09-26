//
#include <iostream>
#include <string>
#include "portable-file-dialogs.h"
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <thread>
#include <chrono>
#include <conio.h>
#include <filesystem>
#include <vector>
//
namespace fs = std::filesystem;
//
ma_engine engine;
ma_result result;
ma_result music_play;
ma_decoder decoder;
ma_sound sound;
ma_uint64 totalFrames;
ma_uint64 targetFrame;
//
std::vector<std::string> filenames;
std::vector<std::string> fileadress;
//
std::string x;
char z = ' ';
double durationInSeconds;
double y = 0;
//
int boot_music() {
    int time = 0;
    system("cls");
    result = ma_decoder_init_file(x.c_str(), NULL, &decoder);
    music_play = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS || music_play != MA_SUCCESS) {
        printf("File not Found!\n");
        return -1;
    }
    result = ma_decoder_get_length_in_pcm_frames(&decoder, &totalFrames);
    if (result != MA_SUCCESS) {
        ma_decoder_uninit(&decoder);
        return -1;
    }
    durationInSeconds = (double)totalFrames / decoder.outputSampleRate;
    ma_decoder_uninit(&decoder);
    music_play = ma_sound_init_from_file(&engine, x.c_str(), 0, NULL, NULL, &sound);
    ma_uint32 sampleRate = ma_engine_get_sample_rate(&engine);
    ma_sound_start(&sound);
    bool k = false;
    auto g = std::chrono::high_resolution_clock::now();
    auto h = std::chrono::high_resolution_clock::now();
    std::cout << "Playing... (Press esc to quit space to pause)" << std::endl;
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        if (_kbhit()) {
            if (y >= int(durationInSeconds)) {
                break;
            }
            z = _getch();
            if (z == ' ') {
                if (k == true) {
                    k = false;
                    ma_sound_start(&sound);
                }
                else if (k == false) {
                    k = true;
                    ma_sound_stop(&sound);
                }
            }
            if (z == '\u001b') {
                y = int(durationInSeconds);
                break;
                k = true;
            }
        }
        h = std::chrono::high_resolution_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(h - g).count() >= durationInSeconds * 1000) {
            break;
        }
    }
        ma_sound_uninit(&sound);
        ma_engine_uninit(&engine);
        return 0;
    
}
    //
    int look_files(std::string path) {
        path = fs::current_path().string() + path;
        if (!fs::exists(path) || !fs::is_directory(path)) {
            std::cerr << "Error Pathway not found!" << path << std::endl;
            return -1;
        }

        try {
            for (const auto& entry : fs::directory_iterator(path)) {
                if (entry.is_regular_file()) {
                    std::string ext = entry.path().extension().string();
                    if (ext == ".mp3" || ext == ".wav" || ext == ".flac") {
                        std::cout << "Music File: " << entry.path().filename().string() << " extention:" << entry.path().extension().string() << std::endl;
                        filenames.push_back(entry.path().filename().string());
                        fileadress.push_back(entry.path().string());
                    }
                }
            }
        }
        catch (const fs::filesystem_error& e) {
            std::cerr << "Accecibility error: " << e.what() << std::endl;
            return -1;
        }

        return 0;
    }
    //
    int main() {
        std::cout << "====Terminal=Music=Player====\n";
        look_files("/Music");
        while (true) {
            std::cout << "\nPlease Choose Song:";
            std::cin >> x;
            x = fileadress[stoi(x) - 1];
            boot_music();
        }
        system("PAUSE");
        return 0;
    }
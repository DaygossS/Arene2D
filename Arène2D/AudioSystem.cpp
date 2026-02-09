#include "AudioSystem.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

AudioSystem::AudioSystem() : m_currentState(MusicState::None) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    m_menuTheme = "../Assets/Music/Menu1.ogg";
    m_pauseTheme = "../Assets/Music/Pausemenu1.ogg";
    m_gameOverTheme = "../Assets/Music/Gameover1.ogg";

    m_gamePlaylist = {
        "../Assets/Music/Music1.ogg",
        "../Assets/Music/Music2.ogg",
        "../Assets/Music/Music3.ogg",
        "../Assets/Music/Music4.ogg",
        "../Assets/Music/Music5.ogg",
        "../Assets/Music/Music6.ogg"
    };

    m_victoryPlaylist = {
        "../Assets/Music/Victory1.ogg",
        "../Assets/Music/Victory2.ogg"
    };
}

bool AudioSystem::init() {
    return true;
}

void AudioSystem::update() {
    m_activeSounds.remove_if([](const sf::Sound& s) {
        return s.getStatus() == sf::SoundSource::Status::Stopped;
        });

    if (m_currentState == MusicState::Game && m_music.getStatus() == sf::SoundSource::Status::Stopped) {
        playRandomTrack(m_gamePlaylist);
    }
}

void AudioSystem::playMenuMusic() {
    if (m_currentState == MusicState::Menu) return;

    if (m_music.openFromFile(m_menuTheme)) {
        m_music.setLooping(true);
        m_music.setVolume(50.f);
        m_music.play();
        m_currentState = MusicState::Menu;
    }
}

void AudioSystem::playGameMusic() {
    if (m_currentState == MusicState::Game && m_music.getStatus() == sf::SoundSource::Status::Playing) return;

    m_currentState = MusicState::Game;
    playRandomTrack(m_gamePlaylist);
}

void AudioSystem::playPauseMusic() {
    if (m_currentState == MusicState::Pause) return;

    if (m_music.openFromFile(m_pauseTheme)) {
        m_music.setLooping(true);
        m_music.setVolume(30.f);
        m_music.play();
        m_currentState = MusicState::Pause;
    }
}

void AudioSystem::playGameOverMusic() {
    if (m_currentState == MusicState::GameOver) return;

    if (m_music.openFromFile(m_gameOverTheme)) {
        m_music.setLooping(false);
        m_music.setVolume(50.f);
        m_music.play();
        m_currentState = MusicState::GameOver;
    }
}

void AudioSystem::playVictoryMusic() {
    if (m_currentState == MusicState::Victory) return;

    m_currentState = MusicState::Victory;
    playRandomTrack(m_victoryPlaylist);
}

void AudioSystem::playRandomTrack(const std::vector<std::string>& playlist) {
    if (playlist.empty()) return;

    int index = std::rand() % playlist.size();

    if (m_music.openFromFile(playlist[index])) {
        m_music.setLooping(false);
        m_music.setVolume(40.f);
        m_music.play();
    }
}

void AudioSystem::stopMusic() {
    m_music.stop();
    m_currentState = MusicState::None;
}

void AudioSystem::playSound(const std::string& key) {
    if (m_soundBuffers.find(key) == m_soundBuffers.end()) return;
    m_activeSounds.emplace_back(m_soundBuffers[key]);
    m_activeSounds.back().play();
}
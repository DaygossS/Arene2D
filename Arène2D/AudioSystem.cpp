#include "AudioSystem.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

AudioSystem::AudioSystem()
    : m_currentState(MusicState::None),
    m_currentMusicVolume(50.f),
    m_currentSfxVolume(50.f)
{
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

void AudioSystem::setGlobalVolume(float musicVolume, float sfxVolume) {
    m_currentMusicVolume = musicVolume;
    m_currentSfxVolume = sfxVolume;

    if (m_music.getStatus() != sf::SoundSource::Status::Stopped) {
        m_music.setVolume(m_currentMusicVolume);
    }
}

void AudioSystem::playMenuMusic() {
    if (m_currentState == MusicState::Menu) return;

    if (m_music.openFromFile(m_menuTheme)) {
        m_music.setLooping(true);
        m_music.setVolume(m_currentMusicVolume);
        m_music.play();
        m_currentState = MusicState::Menu;
    }
}

void AudioSystem::playGameMusic() {
    if (m_currentState == MusicState::Game) return;

    m_currentState = MusicState::Game;
    playRandomTrack(m_gamePlaylist);
}

void AudioSystem::playPauseMusic() {
    if (m_currentState == MusicState::Pause) return;

    if (m_music.openFromFile(m_pauseTheme)) {
        m_music.setLooping(true);
        m_music.setVolume(m_currentMusicVolume);
        m_music.play();
        m_currentState = MusicState::Pause;
    }
}

void AudioSystem::playGameOverMusic() {
    if (m_currentState == MusicState::GameOver) return;

    if (m_music.openFromFile(m_gameOverTheme)) {
        m_music.setLooping(false);
        m_music.setVolume(m_currentMusicVolume);
        m_music.play();
        m_currentState = MusicState::GameOver;
    }
}

void AudioSystem::playVictoryMusic() {
    if (m_currentState == MusicState::Victory) return;

    m_currentState = MusicState::Victory;
    playRandomTrack(m_victoryPlaylist);
}

void AudioSystem::playSound(const std::string& key) {
    if (m_soundBuffers.count(key)) {
        m_activeSounds.emplace_back(m_soundBuffers[key]);
        m_activeSounds.back().setVolume(m_currentSfxVolume);
        m_activeSounds.back().play();
    }
}

void AudioSystem::stopMusic() {
    m_music.stop();
    m_currentState = MusicState::None;
}

void AudioSystem::playRandomTrack(const std::vector<std::string>& playlist) {
    if (playlist.empty()) return;

    int index = std::rand() % playlist.size();

    if (m_music.openFromFile(playlist[index])) {
        m_music.setLooping(false);
        m_music.setVolume(m_currentMusicVolume);
        m_music.play();
    }
}
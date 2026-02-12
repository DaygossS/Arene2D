#pragma once
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include <map>
#include <list>

enum class MusicState {
    None,
    Menu,
    Game,
    Pause,
    GameOver,
    Victory
};

class AudioSystem {
public:
    AudioSystem();

    bool init();
    void update();

    void setGlobalVolume(float musicVolume, float sfxVolume);

    void playMenuMusic();
    void playGameMusic();
    void playPauseMusic();
    void playGameOverMusic();
    void playVictoryMusic();

    void playSound(const std::string& key);
    void stopMusic();

private:
    void playRandomTrack(const std::vector<std::string>& playlist);

    sf::Music m_music;
    MusicState m_currentState;

    float m_currentMusicVolume;
    float m_currentSfxVolume;

    std::vector<std::string> m_gamePlaylist;
    std::vector<std::string> m_victoryPlaylist;

    std::string m_menuTheme;
    std::string m_pauseTheme;
    std::string m_gameOverTheme;

    std::map<std::string, sf::SoundBuffer> m_soundBuffers;
    std::list<sf::Sound> m_activeSounds;
};
#pragma once
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include <map>
#include <list> 

// On définit les différents états musicaux
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

    // Canal principal (Menu, Jeu, GameOver, Victoire)
    sf::Music m_music;
    // Canal secondaire (Uniquement pour la musique de pause)
    sf::Music m_pauseMusicChannel;

    MusicState m_currentState;

    float m_currentMusicVolume;
    float m_currentSfxVolume;

    // Playlists (Listes de fichiers)
    std::vector<std::string> m_gamePlaylist;
    std::vector<std::string> m_victoryPlaylist;

    // Chemins uniques
    std::string m_menuTheme;
    std::string m_pauseTheme;
    std::string m_gameOverTheme;

    // Bruitages (SFX)
    std::map<std::string, sf::SoundBuffer> m_soundBuffers;
    std::list<sf::Sound> m_activeSounds;
};
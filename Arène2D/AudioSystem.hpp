#pragma once
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include <map>

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

    bool init(); // Charge les bruitages
    void update(); // Gère le passage à la musique suivante

    // Fonctions spécifiques pour simplifier la vie
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
#ifndef GAME_H
#define GAME_H
#include "appstate.h"
#include "../objects/object.h"
#include "../objects/player.h"
#include "../objects/enemy.h"
#include "../objects/bullet.h"
#include "../objects/brick.h"
#include "../objects/eagle.h"
#include "../objects/bonus.h"
#include <vector>
#include <string>

/**
 * @brief Klasa odpowiada za ruch wszytkich czołgów oraz interaakcje między czołgami oraz między czołgami a innymi obiektami na mapie
 */
class Game : public AppState
{
public:
    /**
     * Domyślny konstruktor - umożliwia grę dla jednego gracza
     */
    Game();
    /**
     * Konstruktor pozwalający podać początkową liczbę graczy. Liczba graczy może być równa 1 lub 2, każda inna wartość spowoduje uruchomienie gry dla jednego gracza.
     * Konstruktor jest wywoływany w @a Menu::nextState.
     * @param players_count - liczba graczy 1 lub 2
     */
    Game(int players_count);
    /**
     * Konstruktor przyjmujący już isteniejących graczy.
     * Wywoływany w @a Score::nextState
     * @param players - kontener z graczami
     * @param previous_level - zmienna przechowująca numer poprzedniego poziomu
     */
    Game(std::vector<Player*> players, int previous_level);

    ~Game();
    /**
     * Funkcja zwraca @a true jeśli gracza zniszczył wszystkich przeciwników albo gdy orzełek został trafiony lub gracz stracił wszystkie życia, czyli nastąpiła przegrana.
     * @return @a true lub @a false
     */
    bool finished() const;
    /**
     * Funkcja wyświetla na początku rundy jej numer. W czasie rozgrywki funkcja odpowiada za rysowanie poziomu (murków, kamieni, wody, lodu, krzewów),
     * graczy, wrogów, bonusów, orzełka, statusu gry na panelu po prawej stronie (pozostałych przeciwników, pozostałe życia graczy, nr rundy).
     * Po przegranej lub podczas pauzy wyświetla odpowiędnią informację na środku ekranu.
     */
    void draw();
    /**
     * Funkcja aktualizuje stan wszystkich obiektów na planszy (czołgów, bonusów, przeszkód). Sprawdza ponadto kolizje między czołgami, między czołgami i elementami poziomu oraz między pociskami a czołgami i elementami mapy.
     * Naspępuje tu usuwanie zniszczonych obiektów, dodawanie nowych wrogich czołgów oraz sprawdzenie warunków zakończenia rundy.
     * @param dt - czas od ostatniego wywołania funkcji w milisekundach
     */
    void update(Uint32 dt);
    /**
     * Następuje tu reakcja na klawisze:
     * @li Enter - pauza gry
     * @li Esc - powrót do menu
     * @li N - przejście do następnej rundy, jeśli gra nie jest przegrana
     * @li B - przejście do poprzedniej rundy, jeśli gra nie jest przegrana
     * @li T - pokazanie ścieżek do celów wrogich czołgów
     * @param ev - wskaźnik na unię SDL_Event przechowującą typ i parametry różnych zdarzeń, w tym zdarzeń klawiatury
     */
    void eventProcess(SDL_Event* ev);
    /**
     * Przejście do następnyego stanu.
     * @return wskaźnik na obiekty klasy @a Scores jeżeli gracz przeszedł rundę lub przegrał. Jeżeli gracz wcisną Esc funkcja zwraca wskaźnik na obiekt @a Menu.
     */
    AppState* nextState();

private:
    /**
     * Wczytanie mapy poziomu z pliku
     * @param path - ścieżka do pliku mapy
     */
    void loadLevel(std::string path);
    /**
     * Usuwanie pozostałych wrogów, graczy, obiektów mapy oraz bonusów
     */
    void clearLevel();
    /**
     * Wczytanie nowego poziomu i utworzenie nowych graczy jeśli jeszcze nie istnieją.
     * @see Game::loadLevel(std::string path)
     */
    void nextLevel();
    /**
     * Tworzenie nowego wroga jeżeli liczba przeciwników na planszy jest mniejsza od 4 przy założeniu, że nie stworzono już wszystkich 20 wrogów na mapie.
     * Funkcja generuje różne poziomy pancerza wrogów w zależności od poziomu; im wyższy numer rundy tym większa szansa, żeby przeciwnik miał czwarty poziom pancerza.
     * Numer poziomu pancerza mówi ile razy należy trafić pociskiem w przeciwnika aby go zniszczyć. Numer ten przyjmuje wartości od 1 do 4 i odpowiada różnym kolorom czołgu.
     * Wygenerowany przeciwnik ma dodatkowe szanse na to, że trafienie w niego spowoduje powstanie bonusu na planszy.
     */
    void generateEnemy();
    /**
     * Funkcja generuje losowy bonus na mapie i ustawia go w pozycji nie kolidującej z orzełkiem.
     */
    void generateBonus();

    /**
     * Sprawdzenie czy czołg może swobodnie jechać naprzód, jeżeli nie zatrzymuje go. Funkcja nie pozwala na wyjazd poza planszę.
     * Jeżeli czołg wjechał na lód powoduje to jego poślizg. Jeżeli czołg posiada bonus "Łódź" może przechodzić przez wodę. Czołgi nie mogą przejechać orzełka.
     * @param tank - czółg, dla którego sprawdzamy kolizje
     * @param dt - ostatnia zmiana czasu, przy założeniu niewielkich zmian w kolejnych krokach czasowych możemy przewidywać kolejne położenie czołgu i odpowiednio reagować.
     */
    void checkCollisionTankWithLevel(Tank* tank, Uint32 dt);
    /**
     * Sprawdzenie czy nie występuje kolizja między badanymi czołgami, jeśli tak oba są zatrzymywane.
     * @param tank1
     * @param tank2
     * @param dt
     */
    void checkCollisionTwoTanks(Tank* tank1, Tank* tank2, Uint32 dt);
    /**
     * Sprawdznie czy wybrany pocisk nie koliduje z jakimś elementem mapy (woda i lód są pomijane). Jeżeli tak pocisk i obiekt są niszczone.
     * Jeśli trafiono orzełka to następuje przegrana.
     * @param bullet - pocisk
     */
    void checkCollisionBulletWithLevel(Bullet* bullet);
    /**
     * Sprawdzenie kolizji pocisku z krzewami (krzakami) na mapie. Niszczenie krzaków i pocisku nastepuje wtedy, gdy ma on zwiększone obrażenia.
     * @param bullet - pocisk
     * @see Bullet::increased_damage
     */
    void checkCollisionBulletWithBush(Bullet* bullet);
    /**
     * Sprawdzanie czy dany gracz trafił w wybranego przeciwnika. Jeśli tak gracz dostaje punkty a przeciwnik traci jeden poziom pancerza.
     * @param player - gracz
     * @param enemy - przeciwnik
     */
    void checkCollisionPlayerBulletsWithEnemy(Player* player, Enemy* enemy);
    /**
     * Sprawdzenie czy przeciwnik nie trafił pociskiem w gracza. Jeżeli tak to gracz traci jedno życie o ile nie miał osłonki.
     * @param enemy - przeciwnik
     * @param player - gracz
     */
    void checkCollisionEnemyBulletsWithPlayer(Enemy* enemy, Player* player);
    /**
     * Jeżeli dwa pociski się zderzą oba zostają niszczone.
     * @param bullet1
     * @param bullet2
     */
    void checkCollisionTwoBullets(Bullet* bullet1, Bullet* bullet2);
    /**
     * Sprawdzenie czy gracz nie wziął bonusu. Jeśli tak następuje odpowiednia reakcja:
     * @li Granat - widoczni wrogowie zostają zniszczeni
     * @li Hełm - gracz dostaje czasową osłonę od wszystkich pocisków
     * @li Zegar - zatrzymanie widocznych przeciwników
     * @li Łopata - Stworzenie czasowo kamiennego muru wokół orzełka
     * @li Czołg - zwiększenie o 1 liczby żyć gracza
     * @li Gwiazda - ulepszenie czołgu gracza (zwiększenie szybkości, liczby pocisków)
     * @li Broń - maksymalne ulepszenie gracza
     * @li Łódź - możliwość przejazdu przez wodę
     * Za zdobycie bonusu gracz dostaje dodatkowe punkty.
     * @param player
     * @param bonus
     */
    void checkCollisionPlayerWithBonus(Player* player, Bonus* bonus);

    /**
     * Liczaba kolumn siatki mapy.
     */
    int m_level_columns_count;
    /**
     * Liczaba wierszy siatki mapy.
     */
    int m_level_rows_count;
    /**
     * Przeszkody na mapie.
     */
    std::vector< std::vector <Object*> > m_level;
    /**
     * Krzaki na mapie.
     */
    std::vector<Object*> m_bushes;

    /**
     * Zbiór wrogów.
     */
    std::vector<Enemy*> m_enemies;
    /**
     * Zbiór pozostałych graczy.
     */
    std::vector<Player*> m_players;
    /**
     * Zbiór zabity graczy.
     */
    std::vector<Player*> m_killed_players;
    /**
     * Zbiór bonusów na mapie.
     */
    std::vector<Bonus*> m_bonuses;
    /**
     * Obiekt orzełka.
     */
    Eagle* m_eagle;

    /**
     * Obecny numer poziomu.
     */
    int m_current_level;
    /**
     * Liczba graczy w wybranym trybie gry 1 lub 2.
     */
    int m_player_count;
    /**
     * Liczba pozostałych wrogów do zabicia na danym poziomie
     */
    int m_enemy_to_kill;

    /**
     * Zmienna przechowyje, czy w tym momencie wyświetlany jest ekran startowy poziomu.
     */
    bool m_level_start_screen;
    /**
     * Zmienna przechowuje informacje, czy orzełek jest chroniony kamiennym murem.
     */
    bool m_protect_eagle;
    /**
     * Czas jak długo wyświetlany jest już ekran startowy poziomu.
     */
    Uint32 m_level_start_time;
    /**
     * Czas od ostatniego stworzenia przeciwnika.
     */
    Uint32 m_enemy_redy_time;
    /**
     * Czas jaki minął od wygrania mapy.
     */
    Uint32 m_level_end_time;
    /**
     * Czas jak długo orzełek jest już chroniony przez kamienny mur.
     */
    Uint32 m_protect_eagle_time;

    /**
     * Stan przegranej.
     */
    bool m_game_over;
    /**
     * Pozycja napisu "GAME OVER" jeśli @a m_game_over jest równe @a true.
     */
    double m_game_over_position;
    /**
     * Zmienna przechowuje informację, czy należy zakończyć bieżący stan gry i przejść do wyświetlania wyników lub manu gry.
     */
    bool m_finished;
    /**
     * Zmienna mówi czy włączona została pauza.
     */
    bool m_pause;
    /**
     * Numer pozycji now stworzonego przeciwnika. Zmieniana przy każdym tworzeniu przeciwnika.
     */
    int m_enemy_respown_position;
};

#endif // GAME_H

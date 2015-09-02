#ifndef SERVER_H
#define SERVER_H

#include "gamestate.h"
#include "../objects/player.h"

#include <vector>
#include <map>
#include <winsock2.h>

class Server : public GameState
{
public:
    Server(Game* parent);

    ~Server();

    /**
     * Funkcja zwraca @a true po określonym czasie wyświetlania ekranu punktów.
     * @return @a true lub @a false
     */
    bool finished() const;
    /**
     * Funkcja odpowiada za rysowanie odpowiednich tekstów i czołgów graczy na ekranie.
     */
    void draw();
    /**
     * Funkcaj odpowiada za zmianę licznika punktów.
     * @param dt - czas od ostatniego wywołania funkcji pozwala określić czas wyświetlania okna wyników
     */
    void update(Uint32 dt);
    /**
     * Funkcja odpowiada na wciśnięcie klawisza Enter, co kończy odliczanie punktów i przyspiesza przejście do następnego stanu.
     * @param ev - wskaźnik na unię SDL_Event przechowującą typ i parametry różnych zdarzeń
     */
    void eventProcess(SDL_Event* ev);

    /**
     * Obsługa eventów, które przyszły przez sieć.
     * @param ev
     */
    void eventProcess(EventsWrapper& ev);

    /**
     * Funkcja zwraca wskaźnik na obiekt będący następnym stanem aplikacji. Jeśli gracz przegrał następnym stanem jest @a Menu jeśli przeszedł rundę następnym stanem jest @a Game.
     * @return wskaźnik na następny stan
     */
    GameState* nextState();

private:
    void getNames();
    void sendNames();
    /**
     * Kontener przechowujący wszystkie napisy jakie pojawiają się w menu.
     */
    std::vector<std::string> m_menu_texts;
    /**
     * Indeks wybranej pozycji menu.
     */
    int m_menu_index;
    Player* m_tank_pointer;
    bool m_start_game;

    std::map<SOCKET, std::string> m_player_name;
    unsigned m_get_names_time;
    unsigned m_send_names_time;
};

#endif // SERVER_H

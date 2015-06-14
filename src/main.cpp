/**
 * @mainpage
 * @par Tanks
 * Gra w czołgi wzorowana na Battle City / Tank 1990 umożliwiająca grę przez jednego lub dwóch graczy
 * @par Sterowanie
 * W menu gry przemieszczenie kursora w postaci czołgu odbywa się za pomocą strzałek @a Góra, @a Dół. Wybranie zaznaczonego pola należy dokonać za pomącą przycisku @a Enter lub @a Spacji.
 * Grając jako gracz pierwszy (żółty czołg) należy przemieszczać się za pomocą strzałek @a Góra, @a Dół, @a Prawo, @a Lewo oraz strzelać za pomocą @a Prawago @a Controla.
 * Gracz drugi (zielony) porusza się klawiszamie @a W, @a S, @a A, @a D, oraz strzela @a Lewym @a Controlem.
 * Rozgrywkę można zatrzymać i ponownie wznowić przyciskiem @a Enter.
 * @author Krystian Kałużny
 * @date 12.05.215
 * @version 1.0
 * @par Kontakt:
 * @a k.kaluzny141@gmail.com
*/

#include "app.h"

int main( int argc, char* args[] )
{
    App app;
    app.run();

    return 0;
}

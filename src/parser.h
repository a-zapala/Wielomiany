/** @file
   Interfejs parsera Wielomianow

   @author Andrzej Zapała
   @copyright Uniwersytet Warszawski
   @date 2017-05-20
*/
#ifndef WIELOMIANY_PARSER_H
#define WIELOMIANY_PARSER_H

/**
 * Rozpznaje czy parsowac wielomian czy komende
 * @param line : tablica charow
 * @return czy parsowac wielomian
 */
bool recognisePoly(char *line);

/**
 * Funkcja parsuje dane wejsciowe zapisane w tablicy charow,
 * gdy blad column zawiera znak,
 * w ktorym nastapil blad
 * @param line : tablica charow
 * @param column : index
 * @return czy jest poprawny wielomian
 */
bool parsePoly(const char *line,int *column);

/**
 * Funkcja parsujaca argument funkcji DEG_BY
 * @param line : tablica charow
 * @return czy argument poprawny
 */
bool parsedegByArrgument(const char *line);

/**
 * Funkcja parsujaca argument funkcji AT
 * @param line : tablica charow
 * @return  czy argument poprawny
 */
bool parsedAtArrgument(const char *line);

/**
 * Tworzy tablice stringow z komendami potrzebna do parsowania komend
 * @return tablica stringow
 */
char **createArrayCommand();

/**
 * Parsuje komende
 * @param line :tablica charow
 * @param arrayOfCommand :tablica komend
 * @return czy sparsowano komende
 */
int parseCommand(char *line, char **arrayOfCommand);

bool parseComposeArgument(const char *line);

#endif //WIELOMIANY_PARSER_H

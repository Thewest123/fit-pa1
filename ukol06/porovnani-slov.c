#ifndef __PROGTEST__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#endif /* __PROGTEST__ */

// Alias for compare function
typedef int (*Compare)(const void *, const void *);

// Struct for a word
typedef struct
{
    char *text;
} Word;

// Array management inspired from our lector @wrzecond
// https://gitlab.fit.cvut.cz/wrzecond/pa1-2021-cviceni/tree/master/cv08

// Struct for the array of Word, contains words and keeps track of its size
typedef struct
{
    size_t size, maxSize, uniqueCount;
    Word *data;
} Words;

void initWords(Words *wordsArray)
{
    memset(wordsArray, 0, sizeof(*wordsArray));
}

void addToWords(Words *wordsArray, Word newWord)
{
    if (wordsArray->size >= wordsArray->maxSize)
    {
        wordsArray->maxSize = 2 * wordsArray->maxSize + 5;
        wordsArray->data = (Word *)realloc(wordsArray->data, wordsArray->maxSize * sizeof(wordsArray->data[0]));
    }

    wordsArray->data[wordsArray->size++] = newWord;
}

void freeWords(Words *wordsArray)
{
    // Free every dynamically allocated string for text
    size_t size = wordsArray->size;
    for (size_t i = 0; i < size; i++)
    {
        free(wordsArray->data[i].text);
    }

    free(wordsArray->data);
}

/**
 * @brief Split string by words an add them to Words array
 * 
 * @param inputString 
 * @param outputList 
 */
void splitWords(const char *inputString, Words *outputList)
{
    //printf("IN: %s\n", inputString);

    // Get max output length from input string
    size_t outputLength = strlen(inputString) + 1;

    //printf("SIZE: %d\n", outputLength);

    // Allocate sufficient memory space (output string can't be longer than input string)
    char *outputString = (char *)calloc(outputLength, sizeof(char));

    // Save pointer to the beggining of output string
    char *outputStringStart = outputString;

    // Flag to check if we're in a word or delimiter
    int isInWord = 0;

    for (size_t i = 0; i < outputLength; i++)
    {

        if (i != (outputLength - 1) && isalpha(*inputString))
        {
            isInWord = 1;

            // Copy lowered char from input to ouput
            *outputString = tolower(*inputString);

            // Advance the output pointer
            outputString++;
        }
        else if (isInWord)
        {
            // End the output string (word)
            *outputString = '\0';

            // Reset the output pointer for next word
            outputString = outputStringStart;

            // Copy the word to the outputList
            Word newWord;
            newWord.text = strdup(outputString);

            addToWords(outputList, newWord);

            isInWord = 0;
        }

        // Advance the input pointer
        inputString++;
    }

    // Free allocted memory
    free(outputStringStart);
}

int wordCmp(const void *a, const void *b)
{
    Word *wordA = (Word *)a;
    Word *wordB = (Word *)b;

    return strcmp(wordA->text, wordB->text);
}

/**
 * @brief Compare sorted arrays that they are the same
 * 
 * @param a 
 * @param b 
 * @return int 
 */
int compareArrays(Words *a, Words *b)
{
    size_t sizeA = a->size;
    size_t sizeB = a->size;

    // If uniqCount is different, we know arrays don't match, no need to check further
    if (a->uniqueCount != b->uniqueCount)
        return 0;

    size_t i = 0;
    size_t j = 0;

    // Compare words from both arrays
    // (Arrays are filled with NULL pointers on positions where were duplicates)
    while (1)
    {
        // If we've searched through the whole array and they've matched so far, they're the same
        if (i >= sizeA || j >= sizeB)
            return 1;

        // If we can access both (no null pointers), compare them
        if ((a->data[i].text) != NULL && (b->data[j].text) != NULL)
        {
            if (strcmp(a->data[i].text, b->data[j].text) != 0)
                return 0;

            // Advance for next words
            i++;
            j++;
        }

        // If one of pointers is null, advance to next until it's valid
        if ((a->data[i].text) == NULL)
            i++;

        if ((b->data[j].text) == NULL)
            j++;
    }
}

void removeDuplicates(Words *array)
{
    size_t size = array->size;

    // Can't check for (i+1)-th item if there is only one
    if (size < 1)
        return;

    size_t uniq = 0;

    for (size_t i = 0; i < size - 1; i++)
    {
        if (strcmp(array->data[i].text, array->data[i + 1].text) == 0)
        {
            // If words are the same, remove the first one
            free(array->data[i].text);

            // Set the pointer to null, so we don't free it again or try to access it
            array->data[i].text = NULL;

            continue;
        }

        uniq++;
    }

    array->uniqueCount = uniq;
}

int sameWords(const char *a, const char *b)
{

    Words wordsA;
    initWords(&wordsA);

    Words wordsB;
    initWords(&wordsB);

    splitWords(a, &wordsA);
    splitWords(b, &wordsB);

    qsort(wordsA.data, wordsA.size, sizeof(wordsA.data[0]), (Compare)wordCmp);
    qsort(wordsB.data, wordsB.size, sizeof(wordsB.data[0]), (Compare)wordCmp);

    removeDuplicates(&wordsA);
    removeDuplicates(&wordsB);

    int result = compareArrays(&wordsA, &wordsB);

    freeWords(&wordsA);
    freeWords(&wordsB);

    return result;
}

#ifndef __PROGTEST__
int main(int argc, char *argv[])
{
    assert(sameWords("hello", "lorem ipsum dolor sit amet hello world") == 0);
    assert(sameWords(" ", "lorem ipsum dolor sit amet hello world") == 0);
    assert(sameWords(" ", "               ") == 1);
    assert(sameWords("lorem ipsum dolor sit amet hello world", "hello") == 0);
    assert(sameWords("hello", "hello hello hello hello hello,hello.") == 1);
    assert(sameWords("hello hello hello hello hello,hello.", "hello") == 1);
    assert(sameWords("hello lorem ipsum hello ipsum hello hello", "abc abc abc abc def abc") == 0);
    assert(sameWords("xyz students.", "HELLO xyz studEnts!") == 0);
    assert(sameWords("Hello students.", "HELLO studEnts!") == 1);
    assert(sameWords(" He said 'hello!'", "'Hello.' he   said.") == 1);
    assert(sameWords("He said he would do it.", "IT said: 'He would do it.'") == 1);
    assert(
        sameWords(
            "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Etiam libero arcu, tempus eu mauris nec, bibendum rhoncus erat. Sed iaculis urna quam, id elementum lorem accumsan id. Nam luctus vulputate metus eu vulputate. Phasellus venenatis, turpis interdum mattis volutpat, massa est convallis ante, eget volutpat lectus lorem tempor urna. Proin nec auctor purus. Maecenas vel ante sit amet dolor accumsan efficitur et vel arcu. Quisque ullamcorper mollis justo. Suspendisse ultrices, magna a rutrum egestas, ante urna tincidunt justo, a consequat quam nibh a risus. Nam quis facilisis dui. Donec accumsan vitae turpis in sodales. In hac habitasse platea dictumst. Proin rhoncus malesuada fringilla. Phasellus vel lectus facilisis, ultrices sem a, feugiat eros. Maecenas in tortor velit. Curabitur lobortis lectus ex, et maximus nibh consectetur quis. Nam nec nibh sed velit malesuada lobortis. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Etiam libero arcu, tempus eu mauris nec, bibendum rhoncus erat. Sed iaculis urna quam, id elementum lorem accumsan id. Nam luctus vulputate metus eu vulputate. Phasellus venenatis, turpis interdum mattis volutpat, massa est convallis ante, eget volutpat lectus lorem tempor urna. Proin nec auctor purus. Maecenas vel ante sit amet dolor accumsan efficitur et vel arcu. Quisque ullamcorper mollis justo. Suspendisse ultrices, magna a rutrum egestas, ante urna tincidunt justo, a consequat quam nibh a risus. Nam quis facilisis dui. Donec accumsan vitae turpis in sodales. In hac habitasse platea dictumst. Proin rhoncus malesuada fringilla. Phasellus vel lectus facilisis, ultrices sem a, feugiat eros. Maecenas in tortor velit. Curabitur lobortis lectus ex, et maximus nibh consectetur quis. Nam nec nibh sed velit malesuada lobortis. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Etiam libero arcu, tempus eu mauris nec, bibendum rhoncus erat. Sed iaculis urna quam, id elementum lorem accumsan id. Nam luctus vulputate metus eu vulputate. Phasellus venenatis, turpis interdum mattis volutpat, massa est convallis ante, eget volutpat lectus lorem tempor urna. Proin nec auctor purus. Maecenas vel ante sit amet dolor accumsan efficitur et vel arcu. Quisque ullamcorper mollis justo. Suspendisse ultrices, magna a rutrum egestas, ante urna tincidunt justo, a consequat quam nibh a risus. Nam quis facilisis dui. Donec accumsan vitae turpis in sodales. In hac habitasse platea dictumst. Proin rhoncus malesuada fringilla. Phasellus vel lectus facilisis, ultrices sem a, feugiat eros. Maecenas in tortor velit. Curabitur lobortis lectus ex, et maximus nibh consectetur quis. Nam nec nibh sed velit malesuada lobortis. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Etiam libero arcu, tempus eu mauris nec, bibendum rhoncus erat. Sed iaculis urna quam, id elementum lorem accumsan id. Nam luctus vulputate metus eu vulputate. Phasellus venenatis, turpis interdum mattis volutpat, massa est convallis ante, eget volutpat lectus lorem tempor urna. Proin nec auctor purus. Maecenas vel ante sit amet dolor accumsan efficitur et vel arcu. Quisque ullamcorper mollis justo. Suspendisse ultrices, magna a rutrum egestas, ante urna tincidunt justo, a consequat quam nibh a risus. Nam quis facilisis dui. Donec accumsan vitae turpis in sodales. In hac habitasse platea dictumst. Proin rhoncus malesuada fringilla. Phasellus vel lectus facilisis, ultrices sem a, feugiat eros. Maecenas in tortor velit. Curabitur lobortis lectus ex, et maximus nibh consectetur quis. Nam nec nibh sed velit malesuada lobortis. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Etiam libero arcu, tempus eu mauris nec, bibendum rhoncus erat. Sed iaculis urna quam, id elementum lorem accumsan id. Nam luctus vulputate metus eu vulputate. Phasellus venenatis, turpis interdum mattis volutpat, massa est convallis ante, eget volutpat lectus lorem tempor urna. Proin nec auctor purus. Maecenas vel ante sit amet dolor accumsan efficitur et vel arcu. Quisque ullamcorper mollis justo. Suspendisse ultrices, magna a rutrum egestas, ante urna tincidunt justo, a consequat quam nibh a risus. Nam quis facilisis dui. Donec accumsan vitae turpis in sodales. In hac habitasse platea dictumst. Proin rhoncus malesuada fringilla. Phasellus vel lectus facilisis, ultrices sem a, feugiat eros. Maecenas in tortor velit. Curabitur lobortis lectus ex, et maximus nibh consectetur quis. Nam nec nibh sed velit malesuada lobortis.",
            "Phasellus vel lectus facilisis, ultrices sem a, feugiat eros. Maecenas in tortor velit. Curabitur lobortis lectus ex, et maximus nibh consectetur quis. Nam nec nibh sed velit malesuada lobortis. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin nec auctor purus. Maecenas vel ante sit amet dolor accumsan efficitur et vel arcu. Quisque ullamcorper mollis justo. Suspendisse ultrices, magna a rutrum egestas, ante urna tincidunt justo, a consequat quam nibh a risus. Nam quis facilisis dui. Donec accumsan vitae turpis in sodales. Etiam libero arcu, tempus eu mauris nec, bibendum rhoncus erat. Sed iaculis urna quam, id elementum lorem accumsan id. Nam luctus vulputate metus eu vulputate. Phasellus venenatis, turpis interdum mattis volutpat, massa est convallis ante, eget volutpat lectus lorem tempor urna. In hac habitasse platea dictumst. Proin rhoncus malesuada fringilla.") == 1);
    assert(
        sameWords(
            "whtcDClvDFyMWNwxUcLxYIjnDFlKkVoVqNwOsMPONzXzQygnQkOxyBLQRRIVDqRdOprIIaUxPsLTPOdYVdRSvcDlwRWgBVIqsWJdAqgUYdNqUTrkSOmphZlANTjAdCjQagTJusUKKliaMwDGvDEesacRQnBaruntYerUqJqNMlAtePsStkLVVtNyIudclNAWOIuYEBSUTMVFqAkpKXkPolWNRKGUoFDhGQdLFAZvSiEvasaNgBExdhIxdjLCnIohfDPIJHXQOMddzWibzqnmiTkSQFhiCQPAbsFKdimbJCofISOBCXwLEosOrBAsnaqSdVRqdReGhNwuVAqZjehlvqNqKlabfyaAwSAIfpLohuIAnuQKFiNFPakbpuNABQsUOsDmeChaZwBbdyOAiXukmMeQgrzwoWGsmRvpooJPAhYvbzXoJRyOcOorwrIbNaMPyHwMLzECyqMxKeCvWcBXxxDHVngHurriVwnOOtsPvFAYaFBVLdDUlhLwSNysjuSyFEkFGHTWljrLHrWmFbJQhTwoXzFvQTLymFBVNONBdDiOYiiMTOQmnWxHTqFceMkmchynVZENwTWYaSmONmworhImsmWUdNYVJTypXcAEZJwFMkgaJoApLBySGhvOMsDZieQUKXezjzVHAdDbSdLmcttFRcvSdmceJDltAXOBXihWvVeZACrlUXJWpvOauVmlBwCrqheuNzAaDWlqkiparwlyeHzwjnjGUGwPUideuBoISeHDoIpgsKvdTmCnbPmtFPumoZxsfoQcQGZGlFAatcZisTNtJTGzjjhBhUMymkMgpUpcgxOEzdofWKUwVyJgMszOSmylWlRMLGpneQuCZLLMzaoGhcSuesrZaIdCzFKUynPajGzVqVxHfqowcBOdflCvNZnpdYdigZQNAkrnAuhYfapdXnLSKtfheYuurxFqYIYxDXPsMeRdlvuGOTgBTbiZTfVzodbfdGldHUhTKDMrcOUQzHWWNsHDpZARxCRvrMBPbyPucAkMwgBVzSFpbYJgegOzVILvXHSjlvYaHXwMYMNbivxaFRYOHqpFumRJsldNUwiTBNhHOBNglylFhXAFIhYrHlWxVqYtRLjLgGXhKTlThHygAeBWaMkdUMBQJMmhDMaatHzkvrEmyTKTdlOuoebumApQHoPAXchIQBuRbjTpkwTFMqBAfiVbWaLFJvzdxfEKiJsvXlDaqQyPhPpNHiiaxLrAANqZDxhCMLMzhEHkctIMSKGQbBAwFDUAAJcEtezWAYkwHNMrRgRqvlCDohoEUqXtiaASpbCqhFumxUVDaJYSfcbdYFvlaQufCdomnJNLUGiDnxfYjZXGRGGneaPqKKwvHWAUJgpIvOUWlSUchYhvEbYeMbhUJzEutIeHxPRlotOPuMCnoojHRLAbnxKtGOvPadOESEiWdpNlNVwPwPeayVtKpHqSWneAfrPIKkzRGczEJSFxqLzxiLjwrlyxhuNkUXZucQYOuWQeGpxRzEFrZZglmhevyzldmlLEadRPsJXceVTovUQfFHxvYukaQjVYHHVJUUdaRFeeIvpqhrgBJHEStESqVgMmkySDqgyiSZRwoxgsWJYNndPgipdIQqeGetEUvAfgjpdHlOwZBESvEecqoGvdqusBJLcSgMVsGcAqXVYiCCSpxjvDjjrkijUvaiDijeAIgrgobGczoSLjfXjwPckDpLkvCzkHNWYTlBPRuBqrBuMklmiEjxtUkfkGWYuABqrplOLDvQNOGzriiAUYwajqQYXXogipEfmuPyazdNOKQRUqsVzczESntpmjNuFKgbXJeLGwgQUbYxJFVSvpDhlXbvoAUceLGgybxvxhZHMizESlhtZEgBrWCTIvtrwICugNQFrkKQPtjKGBSFWpzhvWiAgiHesxVIVApMYDCJSdnylwlrSlLtsYsGbZtiXfHTXZUcQpXvBcIPgdJVsCNJSFBCtqZiHAcXQTlqpEtFegkANSfCQpjqZyquUCIuEyjYyjQYCjhXPsqEwDziJQBBcBayTPZClFlLkTQYjSNsKfhWjRuDNPAILJAHqXnMOtvpBUTQoZpZEjEKRTZshjWMJqwkyZMZJqNiQWYROOfvzMlAggHIvpSJDqzurFzGmuhiXxnQaMvLILZFEAdbiYhyUspTZaGbeUkILAvrItuezEevVEuUfFJfigiSbsfDCjuRusfIkClNXpwHvhndcfbEaBpTgAjMcswPSbxHmNHFqBxZfNgCkJxJkRGydLKNNpifHiWxmUspGXqsqwpDvdOypOLcccfcVHMDJnCDIxaWSpCKCjHQlEADmjmuPwsQehpGbvuKnobaJiUixJuJuAMXBbZPnlwyVzeFfjCqXvvzagnvjLakCnxtKmxkXmMBHRLCftqhOYrfjkMjeiIqSGyyPTsVhJsrCAgQTWhVatCGKyfbttCHqWsLnklLapwvZKOuAgqoZtmnReKQnjnEjqkbXzgyBUyZYhibcIXOFpRMsWsjXhmkbGNMsUiZhmODsFtqOnMWKuiOVkoEblfAvJAbAwsrpmKXmTudHsGAMAkullDuVqPAZEMNZlEIOtmEXSbQeGmBAmFmgpSBMGumimYczNuoCGxsbcQqnMLBBupllEgnrKKHoonZEjOgCqLCncuFXBXzBCiDgXnHpdKJnMTFcNagYugOudgMQOiXlybEbgdrKhNLhuuplZcMqeirreaUIDdBhreKVXuKzRNDaRCZLTwpCWxwEDVnqDztvDEgNjPIsZlhsNdZaIgTYkKPCLYYFilkcAnvfHcgZnnwXpktLhaONNUCFrKzpnrWfLXYQlbiNWfjGcSQTjEhWBfqOlAlSxCwHvXutqjTItZrPwrtjwJoVjcRIzEluaNsxdNfrgNVNXoDoCnFalcdgiuufPgaAmNIdIOnAjKEidTczOtJCxuiBDorYcgQpHZhbFtYafLRVzfoFMmhnBOLtpPGHrpapMiLXbtfDitGmnZCgGFMdBHzjVnxXCJSOIMmoZyVSOEodaFTRSFlVFroRHvYffjztPwlaqASaJarxCqsRNKCCATbpfICKRSzJHfugaVhFXoeVZmXMKrTpJbnRJAvrzUOSQhwavdRRbwemoiASVrXsjphTmzXAEBpqhpgNrVwapiKqtJxixTHjJIxgBzGDqbezHuieMoHQHfTopZMohyUIlmKKCkDAjhvyEyEZqlNVNDGChfdsZXVDinGvcHrMnDFWJTXqVMscqpeFgkXjjMYwMCwyIvzkyApDdydwIfGAtyFOXjNxTjjLeAykSoewZAGzTczKjIfMrTjGPUKqsRJPpqgxQjaJbndKiHOcpxwcoNvAdQWBCpYYigbQxCAByRRyeOWTsQLAYNzqyGEjCgYxbtrBIEabKiKKZEFchRdxzvyNmjiBYTsVIxhqadGznEZFgsFRbdskoQIpzfStypxsjGKtsYdAVPOKgmCwVGEgbGJtSSvEWbiJuwZgzxzitDoLaEhvURwCizVBFDjHITJIIuvFykOXmuWIxiIdmtfnJeoekZGgChHKNZAYQxlgBpphkLhZgSPvIKbeGAlwECGICLEwlSojqnAtyihikrSPIEeBuuhNpKuhWhDzfdtOcbjQlVXDrKcLKpLLtHCJXMgLpnQDKOjtumInyggkKamkiPkbCiPeZTzVwxCoyCwRQlTTvpkhTtZLpOOEtJxgOAVyCxTvVwduimoOfEMqCrDPcfOelmJwzsCSMUzNugScEjCjDPwFwCxzKFEwFtnsTVyoqhrYscssuaNhjCGNnWOvkJSndUQNpFykvkHJyCynbxNNoGWqYOZsuZaMQlFJyOXINIiHNNLOXhryGCAdnpkPXKPgcAlMgNHIkPhuIWaIUJlkVmiIhIookRMMtyxPAwgRkxYkhbjrgGGBhNCBGfYYzdcRnBcaDlUttgZLJOsYGbieGhreICzXePSqOhmxmszvUApddhXHpiFrgskojFppGpJIRNJiMdnssMdViXamLuRSLrwALayOcsyGl",
            "whtcDClvDFyMWNwxUcLxYIjnDFlKkVoVqNwOsMPONzXzQygnQkOxyBLQRRIVDqRdOprIIaUxPsLTPOdYVdRSvcDlwRWgBVIqsWJdAqgUYdNqUTrkSOmphZlANTjAdCjQagTJusUKKliaMwDGvDEesacRQnBaruntYerUqJqNMlAtePsStkLVVtNyIudclNAWOIuYEBSUTMVFqAkpKXkPolWNRKGUoFDhGQdLFAZvSiEvasaNgBExdhIxdjLCnIohfDPIJHXQOMddzWibzqnmiTkSQFhiCQPAbsFKdimbJCofISOBCXwLEosOrBAsnaqSdVRqdReGhNwuVAqZjehlvqNqKlabfyaAwSAIfpLohuIAnuQKFiNFPakbpuNABQsUOsDmeChaZwBbdyOAiXukmMeQgrzwoWGsmRvpooJPAhYvbzXoJRyOcOorwrIbNaMPyHwMLzECyqMxKeCvWcBXxxDHVngHurriVwnOOtsPvFAYaFBVLdDUlhLwSNysjuSyFEkFGHTWljrLHrWmFbJQhTwoXzFvQTLymFBVNONBdDiOYiiMTOQmnWxHTqFceMkmchynVZENwTWYaSmONmworhImsmWUdNYVJTypXcAEZJwFMkgaJoApLBySGhvOMsDZieQUKXezjzVHAdDbSdLmcttFRcvSdmceJDltAXOBXihWvVeZACrlUXJWpvOauVmlBwCrqheuNzAaDWlqkiparwlyeHzwjnjGUGwPUideuBoISeHDoIpgsKvdTmCnbPmtFPumoZxsfoQcQGZGlFAatcZisTNtJTGzjjhBhUMymkMgpUpcgxOEzdofWKUwVyJgMszOSmylWlRMLGpneQuCZLLMzaoGhcSuesrZaIdCzFKUynPajGzVqVxHfqowcBOdflCvNZnpdYdigZQNAkrnAuhYfapdXnLSKtfheYuurxFqYIYxDXPsMeRdlvuGOTgBTbiZTfVzodbfdGldHUhTKDMrcOUQzHWWNsHDpZARxCRvrMBPbyPucAkMwgBVzSFpbYJgegOzVILvXHSjlvYaHXwMYMNbivxaFRYOHqpFumRJsldNUwiTBNhHOBNglylFhXAFIhYrHlWxVqYtRLjLgGXhKTlThHygAeBWaMkdUMBQJMmhDMaatHzkvrEmyTKTdlOuoebumApQHoPAXchIQBuRbjTpkwTFMqBAfiVbWaLFJvzdxfEKiJsvXlDaqQyPhPpNHiiaxLrAANqZDxhCMLMzhEHkctIMSKGQbBAwFDUAAJcEtezWAYkwHNMrRgRqvlCDohoEUqXtiaASpbCqhFumxUVDaJYSfcbdYFvlaQufCdomnJNLUGiDnxfYjZXGRGGneaPqKKwvHWAUJgpIvOUWlSUchYhvEbYeMbhUJzEutIeHxPRlotOPuMCnoojHRLAbnxKtGOvPadOESEiWdpNlNVwPwPeayVtKpHqSWneAfrPIKkzRGczEJSFxqLzxiLjwrlyxhuNkUXZucQYOuWQeGpxRzEFrZZglmhevyzldmlLEadRPsJXceVTovUQfFHxvYukaQjVYHHVJUUdaRFeeIvpqhrgBJHEStESqVgMmkySDqgyiSZRwoxgsWJYNndPgipdIQqeGetEUvAfgjpdHlOwZBESvEecqoGvdqusBJLcSgMVsGcAqXVYiCCSpxjvDjjrkijUvaiDijeAIgrgobGczoSLjfXjwPckDpLkvCzkHNWYTlBPRuBqrBuMklmiEjxtUkfkGWYuABqrplOLDvQNOGzriiAUYwajqQYXXogipEfmuPyazdNOKQRUqsVzczESntpmjNuFKgbXJeLGwgQUbYxJFVSvpDhlXbvoAUceLGgybxvxhZHMizESlhtZEgBrWCTIvtrwICugNQFrkKQPtjKGBSFWpzhvWiAgiHesxVIVApMYDCJSdnylwlrSlLtsYsGbZtiXfHTXZUcQpXvBcIPgdJVsCNJSFBCtqZiHAcXQTlqpEtFegkANSfCQpjqZyquUCIuEyjYyjQYCjhXPsqEwDziJQBBcBayTPZClFlLkTQYjSNsKfhWjRuDNPAILJAHqXnMOtvpBUTQoZpZEjEKRTZshjWMJqwkyZMZJqNiQWYROOfvzMlAggHIvpSJDqzurFzGmuhiXxnQaMvLILZFEAdbiYhyUspTZaGbeUkILAvrItuezEevVEuUfFJfigiSbsfDCjuRusfIkClNXpwHvhndcfbEaBpTgAjMcswPSbxHmNHFqBxZfNgCkJxJkRGydLKNNpifHiWxmUspGXqsqwpDvdOypOLcccfcVHMDJnCDIxaWSpCKCjHQlEADmjmuPwsQehpGbvuKnobaJiUixJuJuAMXBbZPnlwyVzeFfjCqXvvzagnvjLakCnxtKmxkXmMBHRLCftqhOYrfjkMjeiIqSGyyPTsVhJsrCAgQTWhVatCGKyfbttCHqWsLnklLapwvZKOuAgqoZtmnReKQnjnEjqkbXzgyBUyZYhibcIXOFpRMsWsjXhmkbGNMsUiZhmODsFtqOnMWKuiOVkoEblfAvJAbAwsrpmKXmTudHsGAMAkullDuVqPAZEMNZlEIOtmEXSbQeGmBAmFmgpSBMGumimYczNuoCGxsbcQqnMLBBupllEgnrKKHoonZEjOgCqLCncuFXBXzBCiDgXnHpdKJnMTFcNagYugOudgMQOiXlybEbgdrKhNLhuuplZcMqeirreaUIDdBhreKVXuKzRNDaRCZLTwpCWxwEDVnqDztvDEgNjPIsZlhsNdZaIgTYkKPCLYYFilkcAnvfHcgZnnwXpktLhaONNUCFrKzpnrWfLXYQlbiNWfjGcSQTjEhWBfqOlAlSxCwHvXutqjTItZrPwrtjwJoVjcRIzEluaNsxdNfrgNVNXoDoCnFalcdgiuufPgaAmNIdIOnAjKEidTczOtJCxuiBDorYcgQpHZhbFtYafLRVzfoFMmhnBOLtpPGHrpapMiLXbtfDitGmnZCgGFMdBHzjVnxXCJSOIMmoZyVSOEodaFTRSFlVFroRHvYffjztPwlaqASaJarxCqsRNKCCATbpfICKRSzJHfugaVhFXoeVZmXMKrTpJbnRJAvrzUOSQhwavdRRbwemoiASVrXsjphTmzXAEBpqhpgNrVwapiKqtJxixTHjJIxgBzGDqbezHuieMoHQHfTopZMohyUIlmKKCkDAjhvyEyEZqlNVNDGChfdsZXVDinGvcHrMnDFWJTXqVMscqpeFgkXjjMYwMCwyIvzkyApDdydwIfGAtyFOXjNxTjjLeAykSoewZAGzTczKjIfMrTjGPUKqsRJPpqgxQjaJbndKiHOcpxwcoNvAdQWBCpYYigbQxCAByRRyeOWTsQLAYNzqyGEjCgYxbtrBIEabKiKKZEFchRdxzvyNmjiBYTsVIxhqadGznEZFgsFRbdskoQIpzfStypxsjGKtsYdAVPOKgmCwVGEgbGJtSSvEWbiJuwZgzxzitDoLaEhvURwCizVBFDjHITJIIuvFykOXmuWIxiIdmtfnJeoekZGgChHKNZAYQxlgBpphkLhZgSPvIKbeGAlwECGICLEwlSojqnAtyihikrSPIEeBuuhNpKuhWhDzfdtOcbjQlVXDrKcLKpLLtHCJXMgLpnQDKOjtumInyggkKamkiPkbCiPeZTzVwxCoyCwRQlTTvpkhTtZLpOOEtJxgOAVyCxTvVwduimoOfEMqCrDPcfOelmJwzsCSMUzNugScEjCjDPwFwCxzKFEwFtnsTVyoqhrYscssuaNhjCGNnWOvkJSndUQNpFykvkHJyCynbxNNoGWqYOZsuZaMQlFJyOXINIiHNNLOXhryGCAdnpkPXKPgcAlMgNHIkPhuIWaIUJlkVmiIhIookRMMtyxPAwgRkxYkhbjrgGGBhNCBGfYYzdcRnBcaDlUttgZLJOsYGbieGhreICzXePSqOhmxmszvUApddhXHpiFrgskojFppGpJIRNJiMdnssMdViXamLuRSLrwALayOcsyGl") == 1);
    assert(sameWords("one two lorem ipsum three.", "one two five") == 0);
    assert(sameWords("one two three", "one two five") == 0);

    return 0;
}
#endif /* __PROGTEST__ */
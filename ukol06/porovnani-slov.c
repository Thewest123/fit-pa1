#ifndef __PROGTEST__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#endif /* __PROGTEST__ */

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
    size_t size, maxSize;
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
    for (size_t i = 0; i < wordsArray->size; i++)
    {
        free(wordsArray->data[i].text);
    }

    free(wordsArray->data);
}

void splitWords(const char *inputString, Words *outputList)
{
    printf("IN: %s\n", inputString);

    // Get max output length from input string
    int outputLength = strlen(inputString) + 1;

    printf("SIZE: %d\n", outputLength);

    // Allocate sufficient memory space (output string can't be longer than input string)
    char *outputString = (char *)calloc(outputLength, sizeof(char));

    // Save pointer to the beggining of output string
    char *outputStringStart = outputString;

    // Flag to check if we're in a word or delimiter
    int isInWord = 0;

    for (int i = 0; i < outputLength; i++)
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

            printf("OUT: %s\n", outputString);

            isInWord = 0;
        }

        // Advance the input pointer
        inputString++;
    }

    // Free allocted memory
    free(outputStringStart);
}

int sameWords(const char *a, const char *b)
{

    Words wordsA;
    initWords(&wordsA);

    Words wordsB;
    initWords(&wordsB);

    splitWords(a, &wordsA);
    splitWords(b, &wordsB);

    freeWords(&wordsA);
    freeWords(&wordsB);

    return 1;
}

#ifndef __PROGTEST__
int main(int argc, char *argv[])
{
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
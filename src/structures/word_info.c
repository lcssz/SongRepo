/**
 * @file word_info.c
 * @brief Implementação das funções para gerenciar WordInfo e SongInfo
 * @author AED2 - Algoritmos e Estruturas de Dados II
 * @version 1.0
 */

#include "../../include/structures/word_info.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief Cria uma nova estrutura SongInfo
 * @param title Título da música
 * @param author Autor da música
 * @param verse_snippet Trecho da estrofe
 * @param frequency Frequência da palavra na música
 * @return Ponteiro para SongInfo criado ou NULL se falhar
 */
SongInfo* create_song_info(const char *title, const char *author, const char *verse_snippet, int frequency) {
    SongInfo *song = malloc(sizeof(SongInfo));
    if (!song) return NULL;
    
    song->title = strdup(title);
    song->author = strdup(author);
    song->verse_snippet = strdup(verse_snippet);
    song->frequency_in_song = frequency;
    
    if (!song->title || !song->author || !song->verse_snippet) {
        free_song_info(song);
        return NULL;
    }
    
    return song;
}

/**
 * @brief Libera a memória de uma estrutura SongInfo
 * @param song SongInfo a ser liberado
 */
void free_song_info(SongInfo *song) {
    if (!song) return;
    
    free(song->title);
    free(song->author);
    free(song->verse_snippet);
    free(song);
}

/**
 * @brief Cria uma nova estrutura WordInfo
 * @param word Palavra a ser armazenada
 * @return Ponteiro para WordInfo criado ou NULL se falhar
 */
WordInfo* create_word_info(const char *word) {
    WordInfo *info = malloc(sizeof(WordInfo));
    if (!info) return NULL;
    
    info->word = strdup(word);
    if (!info->word) {
        free(info);
        return NULL;
    }
    
    info->songs = malloc(sizeof(SongInfo) * 10);
    if (!info->songs) {
        free(info->word);
        free(info);
        return NULL;
    }
    
    info->num_songs = 0;
    info->capacity = 10;
    info->total_frequency = 0;
    
    return info;
}

/**
 * @brief Adiciona uma música a uma palavra
 * @param word_info WordInfo onde adicionar a música
 * @param title Título da música
 * @param author Autor da música
 * @param verse_snippet Trecho da estrofe
 * @param frequency Frequência da palavra na música
 */
void add_song_to_word(WordInfo *word_info, const char *title, const char *author, const char *verse_snippet, int frequency) {
    if (!word_info) return;
    
    for (int i = 0; i < word_info->num_songs; i++) {
        if (strcmp(word_info->songs[i].title, title) == 0 && 
            strcmp(word_info->songs[i].author, author) == 0) {
            if (frequency > word_info->songs[i].frequency_in_song) {
                word_info->total_frequency -= word_info->songs[i].frequency_in_song;
                word_info->songs[i].frequency_in_song = frequency;
                word_info->total_frequency += frequency;
                free(word_info->songs[i].verse_snippet);
                word_info->songs[i].verse_snippet = strdup(verse_snippet);
            }
            return;
        }
    }
    
    if (word_info->num_songs >= word_info->capacity) {
        int new_capacity = word_info->capacity * 2;
        SongInfo *new_songs = realloc(word_info->songs, sizeof(SongInfo) * new_capacity);
        if (!new_songs) return;
        
        word_info->songs = new_songs;
        word_info->capacity = new_capacity;
    }
    
    word_info->songs[word_info->num_songs].title = strdup(title);
    word_info->songs[word_info->num_songs].author = strdup(author);
    word_info->songs[word_info->num_songs].verse_snippet = strdup(verse_snippet);
    word_info->songs[word_info->num_songs].frequency_in_song = frequency;
    
    if (word_info->songs[word_info->num_songs].title && 
        word_info->songs[word_info->num_songs].author && 
        word_info->songs[word_info->num_songs].verse_snippet) {
        word_info->num_songs++;
        word_info->total_frequency += frequency;
    }
}

/**
 * @brief Libera a memória de uma estrutura WordInfo
 * @param word_info WordInfo a ser liberado
 */
void free_word_info(WordInfo *word_info) {
    if (!word_info) return;
    
    for (int i = 0; i < word_info->num_songs; i++) {
        free(word_info->songs[i].title);
        free(word_info->songs[i].author);
        free(word_info->songs[i].verse_snippet);
    }
    
    free(word_info->songs);
    free(word_info->word);
    free(word_info);
}

//
// Created by uberdever on 06.08.2020.
//

#ifndef ALPHABETA_HASH_TABLE_H
#define ALPHABETA_HASH_TABLE_H

#include "../alpha.h"

/*
 * Hash table set on fixed array, collision resolving method: open addressing.
 * Designed for handling pointer data, but otherwise work too
 * Contents of pointed data must be cleared separately via arr_delete_contents
 * Meant to be used with unique keys, otherwise, get/set functions won't return different elements, just first one.
 */

#define HT_RAW_ELEMENTS_NUM 3
#define HT_KEY_LEN 48

#define ht_bucket(T)           \
    typedef struct ht_node_##T \
    {                          \
        char key[HT_KEY_LEN];  \
        T value;               \
    }

#define _ht_get_raw_data(ht) ((uint *)(ht)-HT_RAW_ELEMENTS_NUM)
#define _ht_get_return(ht) (*((uint *)(ht)-1)) /* Return contains added/removed/found index, or 0, if element is not present in table */
#define _ht_get_index_from_key(ht, key) (delta_hash_string(key) % ht_get_cap(ht))
#define _ht_get_raw_elements_num HT_RAW_ELEMENTS_NUM

#define ht_get_cap(ht) (*_ht_get_raw_data(ht))
#define ht_get_size(ht) (*((uint *)(ht)-2))
#define ht_get_found_ptr(ht) ((ht) + (_ht_get_return(ht)) - 1)
#define ht_get_added_ptr(ht) ht_get_found_ptr(ht)
#define ht_get_found(ht) (*ht_get_found_ptr(ht))
#define ht_get_added(ht) ht_get_found(ht)
#define ht_get_added_index(ht) _ht_get_return(ht)
#define ht_get_found_index(ht) _ht_get_return(ht)

#define ht_is_found(ht) (_ht_get_return(ht) != 0)

#define ht_new(ht, c)                                                        \
    do                                                                       \
    {                                                                        \
        (ht) = malloc(c * sizeof(*ht) + HT_RAW_ELEMENTS_NUM * sizeof(uint)); \
        ALLOC_CHECK(ht);                                                     \
        (ht) = (void *)((uint *)(ht) + HT_RAW_ELEMENTS_NUM);                 \
        ht_get_cap(ht) = c;                                                  \
        ht_get_size(ht) = 0;                                                 \
        _ht_get_return(ht) = 0;                                              \
        memset(ht, 0, c * sizeof(*ht));                                      \
    } while (0) // Avoiding use of GCC extensions

#define ht_free(ht) (free(_ht_get_raw_data(ht)), ht = NULL)

#define ht_add(ht, key_, el)                                       \
    do                                                             \
    {                                                              \
        const uint32 hash__ = _ht_get_index_from_key(ht, key_);    \
        uint ind__ = 0;                                            \
        uint32 probe__ = hash__;                                   \
        _ht_get_return(ht) = 1;                                    \
        while (ht[probe__].key[0] != '\0')                         \
        {                                                          \
            ind__++;                                               \
            probe__ = (hash__ + (ind__ * ind__)) % ht_get_cap(ht); \
            if (unlikely(probe__ == hash__))                       \
            {                                                      \
                _ht_get_return(ht) = 0;                            \
                break;                                             \
            }                                                      \
        }                                                          \
        if (likely(_ht_get_return(ht)))                            \
        {                                                          \
            ht_get_size(ht)++;                                     \
            (ht)[probe__].value = el;                              \
            strncat((ht)[probe__].key, key_, HT_KEY_LEN);          \
            _ht_get_return(ht) = probe__ + 1;                      \
        }                                                          \
    } while (0)

// Get is separated into search and receive function for ability to handling the search results by user
#define ht_search(ht, key_)                                            \
    do                                                                 \
    {                                                                  \
        const uint32 hash__ = _ht_get_index_from_key(ht, key_);        \
        if (likely((ht)[hash__].key[0]))                               \
        {                                                              \
            uint ind__ = 0;                                            \
            uint32 probe__ = hash__;                                   \
            _ht_get_return(ht) = 1;                                    \
            while (strcmp(key_, ht[probe__].key) != 0)                 \
            {                                                          \
                ind__++;                                               \
                probe__ = (hash__ + (ind__ * ind__)) % ht_get_cap(ht); \
                if (unlikely(probe__ == hash__))                       \
                {                                                      \
                    _ht_get_return(ht) = 0;                            \
                    break;                                             \
                }                                                      \
            }                                                          \
            if (likely(_ht_get_return(ht)))                            \
            {                                                          \
                _ht_get_return(ht) = probe__ + 1;                      \
            }                                                          \
        }                                                              \
        else                                                           \
        {                                                              \
            _ht_get_return(ht) = 0;                                    \
        }                                                              \
    } while (0)

#define ht_delete(ht, key_)  \
    do                       \
    {                        \
        ht_search(ht, key_); \
        ht_delete_found(ht); \
    } while (0);

#define ht_delete_found(ht) \
    (ht_is_found(ht) ? (ht_get_found(ht).key[0] = '\0', --ht_get_size(ht)) : 0)

// Hash table set on fixed array, collision resolving method: chains.
// Designed for handling pointer data, but otherwise work too
// Contents of pointed data must be cleared separately via arr_delete_contents
// Meant to be used with unique keys, otherwise, get/set functions won't return different elements, just first one.
// First element is not pointer, but an element, so search is faster, but delete and add is slightly slower
#if 0
#define HTL_RAW_ELEMENTS_NUM 2
#define HTL_KEY_LEN 48

typedef struct htl_node_head
{
    char key[HTL_KEY_LEN];
    struct htl_node_head *next;
    struct htl_node_head *prev;
} htl_node_head;

#define htl_bucket(T)              \
    typedef struct htl_node_##T    \
    {                              \
        char key[HTL_KEY_LEN];     \
        struct htl_node_##T *next; \
        struct htl_node_##T *prev; \
        T value;                   \
    }

#define _htl_get_raw_data(htl) ((uint *)(htl)-HTL_RAW_ELEMENTS_NUM)
#define _htl_get_index_from_key(htl, key) (delta_hash_string(key) % htl_get_cap(htl))

#define htl_get_cap(htl) (*_htl_get_raw_data(htl))
#define htl_get_size(htl) (*((uint *)(htl)-1))

#define htl_new(htl, c)                                                         \
    do                                                                          \
    {                                                                           \
        (htl) = malloc(c * sizeof(*htl) + HTL_RAW_ELEMENTS_NUM * sizeof(uint)); \
        ALLOC_CHECK(htl);                                                       \
        (htl) = (void *)((uint *)(htl) + HTL_RAW_ELEMENTS_NUM);                 \
        htl_get_cap(htl) = c;                                                   \
        htl_get_size(htl) = 0;                                                  \
        memset(htl, 0, c * sizeof(*htl));                                       \
    } while (0)

#define htl_free(htl)                                    \
    do                                                   \
    {                                                    \
        if (htl)                                         \
        {                                                \
            const uint cap__ = htl_get_cap(htl);         \
            for (uint i = 0; i < cap__; i++)             \
            {                                            \
                while (htl[i].next)                      \
                {                                        \
                    void *ptr__ = (htl)[i].next;         \
                    (htl)[i].next = (htl)[i].next->next; \
                    free(ptr__);                         \
                };                                       \
            }                                            \
            free(_htl_get_raw_data(htl));                \
            htl = NULL;                                  \
        }                                                \
    } while (0)

#define htl_add(htl, key_, el)                                         \
    do                                                                 \
    {                                                                  \
        const uint32 hash__ = _htl_get_index_from_key(htl, key_);      \
        if (likely((htl)[hash__].prev != NULL))                        \
        {                                                              \
            (htl)[hash__].prev->next = malloc(sizeof(*htl));           \
            ALLOC_CHECK(htl[hash__].prev->next);                       \
            memset(htl[hash__].prev->next, 0, sizeof(*htl));           \
            htl[hash__].prev->next->value = el;                        \
            strncat((htl)[hash__].prev->next->key, key_, HTL_KEY_LEN); \
            (htl)[hash__].prev->next->prev = (htl)[hash__].prev;       \
            (htl)[hash__].prev = (htl)[hash__].prev->next;             \
        }                                                              \
        else                                                           \
        {                                                              \
            htl[hash__].value = el;                                    \
            strncat((htl)[hash__].key, key_, HTL_KEY_LEN);             \
            (htl)[hash__].prev = &(htl)[hash__];                       \
        }                                                              \
        htl_get_size(htl)++;                                           \
    } while (0)

#define htl_search(htl, key_, ptr)                                   \
    do                                                               \
    {                                                                \
        const uint32 hash__ = _htl_get_index_from_key(htl, key_);    \
        if (likely((htl)[hash__].key[0]))                            \
        {                                                            \
            (ptr) = &(htl)[hash__];                                  \
            while (((ptr) != NULL) && strcmp(key_, (ptr)->key) != 0) \
            {                                                        \
                (ptr) = (ptr)->next;                                 \
            }                                                        \
        }                                                            \
        else                                                         \
        {                                                            \
            (ptr) = NULL;                                            \
        }                                                            \
    } while (0)

/* #define htl_delete(htl, key_) \  Fuck that, I won't do delete
do {                          \
    const uint32 hash_del_ = _htl_get_index_from_key(htl, key_);\
    if ((htl)[hash_del_].prev)   \
    {                         \
        htl_search(htl, key_, (htl)[hash_del_].prev->next); \
        if ((htl)[hash_del_].prev->next == &(htl)[hash_del_])                      \
        {                     \
            if ((htl)[hash_del_].next) {                      \
                (htl)[hash_del_].value = (htl)[hash_del_].next->value;\
                strncat((htl)[hash_del_].key, (htl)[hash_del_].next->key, HTL_KEY_LEN);                \
                (htl)[hash_del_].next->prev = (htl)[hash_del_].prev;                  \
                void* htl_ptr__ = (htl)[hash_del_].next->next;                  \
                free((htl)[hash_del_].next);                  \
                (htl)[hash_del_].next = htl_ptr__;                 \
            } else {(htl)[hash_del_].key[0] = '\0'; (htl)[hash_del_].prev = NULL;}                      \
            htl_get_size(htl)--;                                                 \
        } else {              \
            (htl)[hash_del_].prev->prev;                  \
        }                      \
    }                          \
} while (0)\ */
#endif
#define string_hash(str) (hash_f((str), strlen(str)))

static uint32 hash_f(const char *data, uint len) //nginx murmur hash function
{
    uint32 h, k;

    h = 0 ^ len;

    while (len >= 4)
    {
        k = data[0];
        k |= data[1] << 8;
        k |= data[2] << 16;
        k |= data[3] << 24;

        k *= 0x5bd1e995;
        k ^= k >> 24;
        k *= 0x5bd1e995;

        h *= 0x5bd1e995;
        h ^= k;

        data += 4;
        len -= 4;
    }

    switch (len)
    {
    case 3:
        h ^= data[2] << 16;
        /* fall through */
    case 2:
        h ^= data[1] << 8;
        /* fall through */
    case 1:
        h ^= data[0];
        h *= 0x5bd1e995;
    default:
        break;
    }

    h ^= h >> 13;
    h *= 0x5bd1e995;
    h ^= h >> 15;

    return h;
}
#define mapls_key_len 32

#define mapls_register(T)                                                       \
    typedef struct mls_bucket_##T##_t                                           \
    {                                                                           \
        char key[mapls_key_len];                                                \
        struct mls_bucket_##T##_t *n;                                           \
        T data;                                                                 \
    } mls_bucket_##T##_t;                                                       \
                                                                                \
    struct mls_##T##_t                                                          \
    {                                                                           \
        uint cap;                                                               \
                                                                                \
        void (*new)(struct mls_##T##_t *, uint);                                \
        void (*free)(struct mls_##T##_t *);                                     \
        void (*add)(struct mls_##T##_t *, char *, T);                           \
        T(*search)                                                              \
        (struct mls_##T##_t * this, char *key);                                 \
        T *(*search_ptr)(struct mls_##T##_t * this, char *key);                 \
                                                                                \
        mls_bucket_##T##_t *data;                                               \
    };                                                                          \
                                                                                \
    static void mapls_##T##_new(struct mls_##T##_t *this, uint cap)             \
    {                                                                           \
        ARR_ALLOC(this->data, cap);                                             \
        ALLOC_CHECK(this->data);                                                \
        memset(this->data, 0, sizeof(mls_bucket_##T##_t) * cap);                \
        this->cap = cap;                                                        \
    }                                                                           \
    static void mapls_##T##_free(struct mls_##T##_t *this)                      \
    {                                                                           \
        for (uint i = 0; i < this->cap; i++)                                    \
        {                                                                       \
            while (this->data[i].n)                                             \
            {                                                                   \
                mls_bucket_##T##_t *ptr = this->data[i].n;                      \
                this->data[i].n = this->data[i].n->n;                           \
                free(ptr);                                                      \
            }                                                                   \
        }                                                                       \
                                                                                \
        free(this->data);                                                       \
        this->cap = 0;                                                          \
    }                                                                           \
    static void mapls_##T##_add(struct mls_##T##_t *this, char *key, T element) \
    {                                                                           \
        const uint32 hash = string_hash(key);                                   \
        mls_bucket_##T##_t *ptr = &this->data[hash % this->cap];                \
        if (ptr->key[0] == 0)                                                   \
        {                                                                       \
            strcpy(ptr->key, key);                                              \
            ptr->data = element;                                                \
            ptr->n = NULL;                                                      \
            return;                                                             \
        }                                                                       \
        while (ptr->n)                                                          \
        {                                                                       \
            ptr = ptr->n;                                                       \
        }                                                                       \
        ptr->n = malloc(sizeof(mls_bucket_##T##_t));                            \
        ALLOC_CHECK(ptr->n);                                                    \
        strcpy(ptr->n->key, key);                                               \
        ptr->n->data = element;                                                 \
        ptr->n->n = NULL;                                                       \
    }                                                                           \
    static T mapls_##T##_search(struct mls_##T##_t *this, char *key)            \
    {                                                                           \
        const uint32 hash = string_hash(key);                                   \
        mls_bucket_##T##_t *ptr = &this->data[hash % this->cap];                \
        while (ptr)                                                             \
        {                                                                       \
            if (strcmp(ptr->key, key) == 0)                                     \
                return ptr->data;                                               \
            ptr = ptr->n;                                                       \
        }                                                                       \
        return (T){0};                                                          \
    }                                                                           \
    static T *mapls_##T##_search_ptr(struct mls_##T##_t *this, char *key)       \
    {                                                                           \
        const uint32 hash = string_hash(key);                                   \
        mls_bucket_##T##_t *ptr = &this->data[hash % this->cap];                \
        while (ptr)                                                             \
        {                                                                       \
            if (strcmp(ptr->key, key) == 0)                                     \
                return &ptr->data;                                              \
            ptr = ptr->n;                                                       \
        }                                                                       \
        return NULL;                                                            \
    }

#define mapls_new(T, name, cap)                 \
    (name).new = mapls_##T##_new;               \
    (name).free = mapls_##T##_free;             \
    (name).add = mapls_##T##_add;               \
    (name).search = mapls_##T##_search;         \
    (name).search_ptr = mapls_##T##_search_ptr; \
    (name).new(&(name), cap);

#define mapls_add(m, key, el) (m).add(&(m), (key), (el))
#define mapls_free(m) (m).free(&(m))
#define mapls_search(m, key) (m).search(&(m), key)
#define mapls_search_ptr(m, key) (m).search_ptr(&(m), key)

#define mapl_key_len 32

#define mapl_register(T, K)                                                                            \
                                                                                                       \
    typedef struct ml_bucket_##T##K##_t                                                                \
    {                                                                                                  \
        K key;                                                                                         \
        bool is_filled;                                                                                \
        struct ml_bucket_##T##K##_t *n;                                                                \
        T data;                                                                                        \
    } ml_bucket_##T##K##_t;                                                                            \
                                                                                                       \
    struct ml_##T##K##_t                                                                               \
    {                                                                                                  \
        uint cap;                                                                                      \
                                                                                                       \
        void (*new)(struct ml_##T##K##_t *, uint);                                                     \
        void (*free)(struct ml_##T##K##_t *);                                                          \
        void (*add)(struct ml_##T##K##_t *, const K *, size_t, T);                                     \
        T(*search)                                                                                     \
        (struct ml_##T##K##_t * this, const K *key, size_t key_len);                                   \
        T *(*search_ptr)(struct ml_##T##K##_t * this, const K *key, size_t key_len);                   \
                                                                                                       \
        ml_bucket_##T##K##_t *data;                                                                    \
    };                                                                                                 \
                                                                                                       \
    static void mapl_##T##K##_new(struct ml_##T##K##_t *this, uint cap)                                \
    {                                                                                                  \
        ARR_ALLOC(this->data, cap);                                                                    \
        ALLOC_CHECK(this->data);                                                                       \
        memset(this->data, 0, sizeof(ml_bucket_##T##K##_t) * cap);                                     \
        this->cap = cap;                                                                               \
    }                                                                                                  \
    static void mapl_##T##K##_free(struct ml_##T##K##_t *this)                                         \
    {                                                                                                  \
        for (uint i = 0; i < this->cap; i++)                                                           \
        {                                                                                              \
            while (this->data[i].n)                                                                    \
            {                                                                                          \
                ml_bucket_##T##K##_t *ptr = this->data[i].n;                                           \
                this->data[i].n = this->data[i].n->n;                                                  \
                free(ptr);                                                                             \
            }                                                                                          \
        }                                                                                              \
                                                                                                       \
        free(this->data);                                                                              \
        this->cap = 0;                                                                                 \
    }                                                                                                  \
    static void mapl_##T##K##_add(struct ml_##T##K##_t *this, const K *key, size_t key_len, T element) \
    {                                                                                                  \
        const uint32 hash = hash_f((const char *)key, key_len);                                        \
        ml_bucket_##T##K##_t *ptr = &this->data[hash % this->cap];                                     \
        if (!ptr->is_filled)                                                                           \
        {                                                                                              \
            ptr->key = *key;                                                                           \
            ptr->data = element;                                                                       \
            ptr->n = NULL;                                                                             \
            ptr->is_filled = true;                                                                     \
            return;                                                                                    \
        }                                                                                              \
        while (ptr->n)                                                                                 \
        {                                                                                              \
            ptr = ptr->n;                                                                              \
        }                                                                                              \
        ptr->n = malloc(sizeof(ml_bucket_##T##K##_t));                                                 \
        ALLOC_CHECK(ptr->n);                                                                           \
        ptr->n->key = *key;                                                                               \
        ptr->n->data = element;                                                                        \
        ptr->n->is_filled = true;                                                                      \
        ptr->n->n = NULL;                                                                              \
    }                                                                                                  \
    static T mapl_##T##K##_search(struct ml_##T##K##_t *this, const K *key, size_t key_len)            \
    {                                                                                                  \
        const uint32 hash = hash_f((const char *)key, key_len);                                        \
        ml_bucket_##T##K##_t *ptr = &this->data[hash % this->cap];                                     \
        while (ptr)                                                                                    \
        {                                                                                              \
            if (hash_f((const char *)&ptr->key, key_len) == hash)                                      \
                return ptr->data;                                                                      \
            ptr = ptr->n;                                                                              \
        }                                                                                              \
        return (T){0};                                                                                 \
    }                                                                                                  \
    static T *mapl_##T##K##_search_ptr(struct ml_##T##K##_t *this, const K *key, size_t key_len)       \
    {                                                                                                  \
        const uint32 hash = hash_f((const char *)key, key_len);                                        \
        ml_bucket_##T##K##_t *ptr = &this->data[hash % this->cap];                                     \
        while (ptr)                                                                                    \
        {                                                                                              \
            if (hash_f((const char *)&ptr->key, key_len) == hash)                                      \
                return &ptr->data;                                                                     \
            ptr = ptr->n;                                                                              \
        }                                                                                              \
        return NULL;                                                                                   \
    }

#define mapl_new(T, K, name, cap)                 \
    (name).new = mapl_##T##K##_new;               \
    (name).free = mapl_##T##K##_free;             \
    (name).add = mapl_##T##K##_add;               \
    (name).search = mapl_##T##K##_search;         \
    (name).search_ptr = mapl_##T##K##_search_ptr; \
    (name).new(&(name), cap);

#define mapl_add(m, key, key_len, el) (m).add(&(m), (key), (key_len), (el))
#define mapl_free(m) (m).free(&(m))
#define mapl_search(m, key, key_len) (m).search(&(m), key, key_len)
#define mapl_search_ptr(m, key, key_len) (m).search_ptr(&(m), key, key_len)

#endif //ALPHABETA_HASH_TABLE_H

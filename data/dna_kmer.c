
#include "dna_kmer.h"

#include "dna_codec.h"

#include <helpers/dna_helper.h>

#include <system/packer.h>
#include <system/memory.h>

#include <hash/murmur_hash_2_64_a.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <stdint.h>

#include <inttypes.h>

/*
#define BSAL_DNA_SEQUENCE_DEBUG
*/
void bsal_dna_kmer_init(struct bsal_dna_kmer *sequence, char *data,
                struct bsal_dna_codec *codec)
{
    int encoded_length;
    int kmer_length;

    if (data == NULL) {
        sequence->encoded_data = NULL;
        kmer_length = 0;
    } else {

        kmer_length = strlen(data);

        encoded_length = bsal_dna_codec_encoded_length(kmer_length);
        sequence->encoded_data = bsal_malloc(encoded_length);
        bsal_dna_codec_encode(codec, kmer_length, data, sequence->encoded_data);
    }
}

void bsal_dna_kmer_destroy(struct bsal_dna_kmer *sequence)
{
    if (sequence->encoded_data != NULL) {
        bsal_free(sequence->encoded_data);
        sequence->encoded_data = NULL;
    }

    sequence->encoded_data = NULL;
}

int bsal_dna_kmer_pack_size(struct bsal_dna_kmer *sequence, int kmer_length)
{
    return bsal_dna_kmer_pack_unpack(sequence, NULL, BSAL_PACKER_OPERATION_DRY_RUN,
                    kmer_length);
}

int bsal_dna_kmer_unpack(struct bsal_dna_kmer *sequence,
                void *buffer, int kmer_length)
{
    return bsal_dna_kmer_pack_unpack(sequence, buffer, BSAL_PACKER_OPERATION_UNPACK, kmer_length);
}

int bsal_dna_kmer_pack_store_key(struct bsal_dna_kmer *self,
                void *buffer, int kmer_length, struct bsal_dna_codec *codec)
{
    struct bsal_dna_kmer kmer2;
    int bytes;

    bsal_dna_kmer_init_copy(&kmer2, self, kmer_length);
    bsal_dna_kmer_reverse_complement_self(&kmer2, kmer_length, codec);

    if (bsal_dna_kmer_is_lower(&kmer2, self, kmer_length, codec)) {
        bytes = bsal_dna_kmer_pack(&kmer2, buffer, kmer_length);
    } else {
        bytes = bsal_dna_kmer_pack(self, buffer, kmer_length);
    }

    bsal_dna_kmer_destroy(&kmer2);

    return bytes;
}

int bsal_dna_kmer_pack(struct bsal_dna_kmer *sequence,
                void *buffer, int kmer_length)
{
    return bsal_dna_kmer_pack_unpack(sequence, buffer, BSAL_PACKER_OPERATION_PACK, kmer_length);
}

int bsal_dna_kmer_pack_unpack(struct bsal_dna_kmer *sequence,
                void *buffer, int operation, int kmer_length)
{
    struct bsal_packer packer;
    int offset;
    int encoded_length;

    bsal_packer_init(&packer, operation, buffer);

    /* don't pack the kmer length...
     */
#if 0
    bsal_packer_work(&packer, kmer_length, sizeof(kmer_length));
#endif

    encoded_length = bsal_dna_codec_encoded_length(kmer_length);

    /* encode in 2 bits instead !
     */
    if (operation == BSAL_PACKER_OPERATION_UNPACK) {

        if (kmer_length > 0) {
            sequence->encoded_data = bsal_malloc(encoded_length);
        } else {
            sequence->encoded_data = NULL;
        }
    }

    if (kmer_length > 0) {
        bsal_packer_work(&packer, sequence->encoded_data, encoded_length);
    }

    offset = bsal_packer_worked_bytes(&packer);

    bsal_packer_destroy(&packer);

    return offset;
}

void bsal_dna_kmer_init_random(struct bsal_dna_kmer *sequence, int kmer_length,
        struct bsal_dna_codec *codec)
{
    char *dna;
    int i;
    int code;

    dna = (char *)bsal_malloc(kmer_length + 1);

    for (i = 0; i < kmer_length; i++) {
        code = rand() % 4;

        if (code == 0) {
            dna[i] = 'A';
        } else if (code == 1) {
            dna[i] = 'T';
        } else if (code == 2) {
            dna[i] = 'C';
        } else if (code == 3) {
            dna[i] = 'G';
        }
    }

    dna[kmer_length] = '\0';

    bsal_dna_kmer_init(sequence, dna, codec);
    bsal_free(dna);
}

void bsal_dna_kmer_init_mock(struct bsal_dna_kmer *sequence, int kmer_length,
                struct bsal_dna_codec *codec)
{
    char *dna;
    int i;

    dna = (char *)bsal_malloc(kmer_length + 1);

    for (i = 0; i < kmer_length; i++) {
        dna[i] = 'A';
    }

    dna[kmer_length] = '\0';

    bsal_dna_kmer_init(sequence, dna, codec);
    bsal_free(dna);
}

void bsal_dna_kmer_init_copy(struct bsal_dna_kmer *self, struct bsal_dna_kmer *other, int kmer_length)
{
    int encoded_length;

    encoded_length = bsal_dna_codec_encoded_length(kmer_length);
    self->encoded_data = bsal_malloc(encoded_length);
    memcpy(self->encoded_data, other->encoded_data, encoded_length);
}

void bsal_dna_kmer_print(struct bsal_dna_kmer *self, int kmer_length,
                struct bsal_dna_codec *codec)
{
    char *dna_sequence;

    dna_sequence = bsal_malloc(kmer_length + 1);

    bsal_dna_codec_decode(codec, kmer_length, self->encoded_data, dna_sequence);

    printf("KMER length: %d nucleotides, sequence: %s\n", kmer_length,
                   dna_sequence);

    bsal_free(dna_sequence);
    dna_sequence = NULL;
}

uint64_t bsal_dna_kmer_hash(struct bsal_dna_kmer *self, int kmer_length)
{
    unsigned int seed;
    /*char *sequence;*/
    int encoded_length;
    uint64_t hash;

    encoded_length = bsal_dna_codec_encoded_length(kmer_length);
    seed = 0xcaa9cfcf;

    /*
     * comment this block
     */
#if 0
    hash = bsal_murmur_hash_2_64_a(self->encoded_data, kmer_length, seed);

    return hash;

    /* Decode sequence because otherwise we'll get a bad performance
     * Update: this is not true. The encoded data is good enough even
     * if it is shorter.
     */
    sequence = bsal_malloc(kmer_length + 1);
    bsal_dna_kmer_get_sequence(self, sequence);
#endif

    /*hash = bsal_murmur_hash_2_64_a(sequence, kmer_length, seed);*/

    hash = bsal_murmur_hash_2_64_a(self->encoded_data, encoded_length, seed);
/*
    printf("%s %" PRIu64 "\n", sequence, hash);
    bsal_free(sequence);
*/

    return hash;
}

int bsal_dna_kmer_store_index(struct bsal_dna_kmer *self, int stores, int kmer_length,
                struct bsal_dna_codec *codec)
{
    uint64_t hash;
    int store_index;
    struct bsal_dna_kmer kmer2;

    bsal_dna_kmer_init_copy(&kmer2, self, kmer_length);
    bsal_dna_kmer_reverse_complement_self(&kmer2, kmer_length, codec);

    if (bsal_dna_kmer_is_lower(&kmer2, self, kmer_length, codec)) {
        hash = bsal_dna_kmer_hash(&kmer2, kmer_length);
    } else {
        hash = bsal_dna_kmer_hash(self, kmer_length);
    }

    store_index = hash % stores;

    bsal_dna_kmer_destroy(&kmer2);

    return store_index;
}

void bsal_dna_kmer_get_sequence(struct bsal_dna_kmer *self, char *sequence, int kmer_length,
                struct bsal_dna_codec *codec)
{
        /*
    printf("get sequence %d\n", kmer_length);
    */
    bsal_dna_codec_decode(codec, kmer_length, self->encoded_data, sequence);
}

/*
 * This function is useless...
 */
int bsal_dna_kmer_length(struct bsal_dna_kmer *self, int kmer_length)
{
    return kmer_length;
}

void bsal_dna_kmer_reverse_complement_self(struct bsal_dna_kmer *self, int kmer_length,
                struct bsal_dna_codec *codec)
{
    char *sequence;

    sequence = bsal_malloc(kmer_length + 1);
    bsal_dna_kmer_get_sequence(self, sequence, kmer_length, codec);

#ifdef BSAL_DNA_KMER_DEBUG
    printf("DEBUG %p before %s\n", (void *)self, sequence);
#endif

    bsal_dna_helper_reverse_complement_in_place(sequence);

#ifdef BSAL_DNA_KMER_DEBUG
    printf("DEBUG %p after %s\n", (void *)self, sequence);
#endif

    bsal_dna_kmer_destroy(self);
    bsal_dna_kmer_init(self, sequence, codec);

    bsal_free(sequence);
    sequence = NULL;
}

int bsal_dna_kmer_is_lower(struct bsal_dna_kmer *self, struct bsal_dna_kmer *other, int kmer_length,
                struct bsal_dna_codec *codec)
{
    int result;

    result = bsal_dna_kmer_compare(self, other, kmer_length, codec);

    if (result < 0) {
        return 1;
    }
    return 0;

}

int bsal_dna_kmer_compare(struct bsal_dna_kmer *self, struct bsal_dna_kmer *other, int kmer_length,
                struct bsal_dna_codec *codec)
{
    int encoded_length;
    encoded_length = bsal_dna_codec_encoded_length(kmer_length);

    return memcmp(self->encoded_data, other->encoded_data, encoded_length);

#if 0
    char *sequence1;
    char *sequence2;
    int result;

    sequence1 = bsal_malloc(kmer_length + 1);
    sequence2 = bsal_malloc(kmer_length + 1);

    bsal_dna_kmer_get_sequence(self, sequence1, kmer_length, codec);
    bsal_dna_kmer_get_sequence(other, sequence2, kmer_length, codec);

    result = strcmp(sequence1, sequence2);

    bsal_free(sequence1);
    bsal_free(sequence2);

    return result;
#endif
}

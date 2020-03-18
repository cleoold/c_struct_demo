#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "darray.h"
#include "structdemo.h"

static void donothing(void *_) {}

static void getRecord(  int64_t userid,
                        int64_t timestamp,
                        double score,
                        char *title_buf,
                        Array_D *records_array,
                        Array_D *strings_array) {
    Record *record;
    size_t title_len;
    register int i;

    title_len = strlen(title_buf);
    /* create record object and push to the array */
    if ((record = malloc(sizeof(Record))) == NULL)
        err(1, "malloc error");
    record->userid = userid;
    record->timestamp = timestamp;
    record->score = score;
    record->title.len = title_len;
    record->title.pos = strings_array->length;

    if (records_array->pushback(records_array, record) == NULL)
        err(1, "array error");

    for (i = 0; i < title_len+1; ++i)
        if (strings_array->pushback(strings_array,
            (void*)(long)title_buf[i]) == NULL)
            err(1, "array error");
}

int main(void) {
    RecoHeader header;
    Array_D *records, *strings;
    FILE *fp;
    register int i;

    /* initialize dynamic storage */
    records = Array_D_new(), strings = Array_D_new();

    /* prepare records */
    getRecord(53295835563, 1584402981, 8.88, "An interesting zhihu article.",
        records, strings);
    getRecord(53295835563, 1584403000, 7.25, "A not-so-interesting zhihu article that nobody reads.",
        records, strings);
    getRecord(52562400923, 1584408001, -1.09, "一些乱七八糟的东西。",
        records, strings);

    /* create header content */
    bzero(&header, sizeof header);
    header.version = VERSION;
    header.magic = RECO_MAGIC;
    header.nrecords = records->length;
    header.record_pos = sizeof header;
    header.string_pos = sizeof header + records->length*sizeof(Record);
    strcpy(header.brief, "Struct demo file");

    /* write these things to file */
    /* opens file */
    if ((fp = fopen("structdemo_c.bin", "w")) == NULL)
        err(1, "cannot open file");
    /* write header */
    if (fwrite(&header, 1, sizeof header, fp) != sizeof header)
        err(1, "fwrite header failed");
    /* write records themselves */
    for (i = 0; i < records->length; ++i)
        if (fwrite(records->content[i], 1,
            sizeof(Record), fp) != sizeof(Record))
            err(1, "fwrite records failed");
    /* write string area */
    for (i = 0; i < strings->length; ++i) {
        if (fwrite((char*)&strings->content[i],
            1, sizeof(char), fp) != sizeof(char))
            err(1, "fwrite strings failed");
    }

    /* cleanup */
    fclose(fp);
    records->free(records, free);
    strings->free(strings, donothing);
}

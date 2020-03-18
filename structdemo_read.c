#include <sys/mman.h>
#include <err.h>
#include <stdio.h>
#include <sys/stat.h>
#include "structdemo.h"

int main(int argc, char** argv) {
    FILE *fp;
    struct stat fs;
    void *mb;
    RecoHeader *hdr;
    Record *rcd;
    register int i;
    char *thetitle;

    if (argc == 1) {
        fprintf(stderr, "usage: %s <filename>\n", argv[0]);
        return 1;
    }

    /* opens file and gets ots attributes */
    if ((fp = fopen(argv[1], "r+")) == NULL)
        err(1, "cannot open file");
    if (fstat(fileno(fp), &fs) < 0)
        err(1, "cannot stat file");
    /* get all content */
    if ((mb = mmap(NULL, fs.st_size, PROT_READ | PROT_WRITE,
        MAP_SHARED, fileno(fp), 0)) == MAP_FAILED)
        err(1, "cannot mmap");

    /* print header content! */
    hdr = (RecoHeader*)mb;

    /* verify binary file is correct */
    if (hdr->version != VERSION || hdr->magic != RECO_MAGIC)
        errx(127, "unsupported file type");

    printf( "--- HEADER ---\n"
            "\tversion: %ld\n"
            "\tmagic: %ld\n"
            "\t# of records: %ld\n"
            "\tbrief: %s\n"
            "\trecord offset: %lu\n"
            "\tstring offset: %lu\n", hdr->version, hdr->magic, hdr->nrecords,
            hdr->brief, hdr->record_pos, hdr->string_pos);

    /* print the records (and strings)! */
    for (i = 0; i < hdr->nrecords; ++i) {
        /* must cast hdr to (char*) so that pointer addition is byte-wise */
        rcd = (Record*)((char*)hdr + hdr->record_pos + i*sizeof(Record));
        thetitle = ((char*)hdr + hdr->string_pos + rcd->title.pos);
        printf( "--- ITEM %d ---\n"
                "\tuserid: %ld\n"
                "\ttime: %ld\n"
                "\tscore: %lf\n"
                "\ttitle: %s\n", i, rcd->userid, rcd->timestamp, rcd->score,
                thetitle);
    }

    /* clean up */
    fclose(fp);
    munmap(mb, fs.st_size);
}

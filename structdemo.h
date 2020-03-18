#include <stdint.h>

#define VERSION 1
#define RECO_MAGIC 0x123456789

#pragma pack(4)
typedef struct RecoHeader {
    int64_t version;      // 0-7
    int64_t magic;        // 8-15
    int64_t nrecords;     // 16-23    number of records
    uint64_t record_pos;  // 24-31    offset of the first Record relative to file
    uint64_t string_pos;  // 32-39    offset of the string content relative to file
    char brief[24];       // 40-63    max bytes 24
} RecoHeader;             // 64 bytes wide

typedef struct Record
{
    int64_t userid;       // 0-7
    int64_t timestamp;    // 8-15
    double score;         // 16-23
    struct {
        uint64_t pos;     // 24-31
        int64_t len;      // 32-39 strlen(string), not including trailling '\0'
    } title;
} Record;                 // 40 bytes wide
#pragma pack()

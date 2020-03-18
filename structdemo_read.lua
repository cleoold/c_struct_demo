
-- convert bytes into the RecoHeader representation, bytes is 64 bytes wide
local function RecoHeader(bytes)
    return {
        version    = ('i8'):unpack(bytes:sub(1, 8)),
        magic      = ('i8'):unpack(bytes:sub(9, 16)),
        nrecords   = ('i8'):unpack(bytes:sub(17, 24)),
        record_pos = ('I8'):unpack(bytes:sub(25, 32)),
        string_pos = ('I8'):unpack(bytes:sub(33, 40)),
        brief      = ('z'):unpack(bytes:sub(41, 64)) -- not null terminated
    }
end

-- convert bytes into Record, recbytes is 40 bytes wide
local function Record(bytes, stringsbytes)
    local r = {
        userid    = ('i8'):unpack(bytes:sub(1, 8)),
        timestamp = ('i8'):unpack(bytes:sub(9, 16)),
        score     = ('d'):unpack(bytes:sub(17, 24)),
        title     = {
            pos   = ('I8'):unpack(bytes:sub(25, 32)),
            len   = ('i8'):unpack(bytes:sub(33, 40))
        }
    }
    r.title._str = stringsbytes:sub(r.title.pos+1, r.title.pos+1+r.title.len)
    return r
end

function Main(arg)

    if #arg == 0 then
        io.stderr:write(string.format('usage: %s <filename>\n', arg[0]))
        os.exit(1)
    end

    -- opens file
    local fp, errmsg, errcode = io.open(arg[1], 'r')
    if fp == nil then error(errmsg, errcode) end
    -- load all content
    local entire_content = fp:read('a')
    fp:close()

    -- get the header part of the string
    local headerbytes = entire_content:sub(1, 64)
    -- convert it to the struct
    local header = RecoHeader(headerbytes)

    -- verify file format
    if header.version ~= 1 or header.magic ~= 0x123456789 then
        io.stderr:write('unsupported file type\n')
        os.exit(127)
    end

    io.write(string.format(
        '--- HEADER ---\n'
        ..'\tversion: %d\n'
        ..'\tmagic: %d\n'
        ..'\t# of records: %d\n'
        ..'\tbrief: %s\n'
        ..'\trecord offset: %u\n'
        ..'\tstring offset: %u\n', header.version, header.magic, header.nrecords,
        header.brief, header.record_pos, header.string_pos
    ))

    -- we can get the record and string parts after obtaining the header info
    local recordsbytes = entire_content:sub(65, 65+40*header.nrecords)
    local stringsbytes = entire_content:sub(header.string_pos+1)
    -- iterate every 40 bytes to print the records
    local i = 0
    for bys in recordsbytes:gmatch(('.'):rep(40)) do
        local rcd = Record(bys, stringsbytes)
        io.write(string.format(
            '--- ITEM %d ---\n'
            ..'\tuserid: %d\n'
            ..'\ttime: %d\n'
            ..'\tscore: %f\n'
            ..'\ttitle: %s\n', i, rcd.userid, rcd.timestamp, rcd.score,
            rcd.title._str
        ))
        i = i+1
    end
end

Main(arg)

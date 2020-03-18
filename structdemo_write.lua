
local function RecoHeaderToBytes(recoheader)
    return ('i8i8i8I8I8'):pack(
        recoheader.version, recoheader.magic, recoheader.nrecords,
        recoheader.record_pos, recoheader.string_pos
    )..recoheader.brief..('\0'):rep(24-#recoheader.brief)
end

-- convert bytes into Record, recbytes is 40 bytes wide
local function RecordToBytes(reco)
    return ('i8i8dI8i8'):pack(
        reco.userid, reco.timestamp, reco.score, reco.title.pos, reco.title.len
    )
end

-- title is not null terminated
local function getRecord(   userid, timestamp, score, title,
                            ra, sa)
    -- append record
    ra[#ra+1] = {
        userid = userid, timestamp = timestamp, score = score,
        title = {
            pos = #sa, len = #title
        }
    }
    -- append string char by char
    for c in title:gmatch('.') do
        sa[#sa+1] = c
    end
    sa[#sa+1] = '\0'
end

function Main(arg)
    local records, stringchars = {}, {}

    -- prepare records
    getRecord(53295835563, 1584402981, 8.88, 'An interesting zhihu article.',
        records, stringchars);
    getRecord(53295835563, 1584403000, 7.25, 'A not-so-interesting zhihu article that nobody reads.',
        records, stringchars);
    getRecord(52562400923, 1584408001, -1.09, '一些乱七八糟的东西。',
        records, stringchars);

    -- create header content
    local header = {
        version = 1, magic = 0x123456789, nrecords = #records,
        record_pos = 64, string_pos = 64 + 40*#records,
        brief = 'Struct demo file'
    }

    -- open file
    local fp, errmsg, errcode = io.open('structdemo_lua.bin', 'w')
    if fp == nil then error(errmsg, errcode) end
    -- write these
    fp:write(RecoHeaderToBytes(header))
    for _, t in ipairs(records) do fp:write(RecordToBytes(t)) end
    for _, b in ipairs(stringchars) do fp:write(b) end

    -- clean up
    fp:close()
end

Main(arg)

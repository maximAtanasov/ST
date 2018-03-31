--log types
ERROR = 0x01
SUCCESS = 0x02
INFO = 0x04

function log(type, string)
    logLua(type, tostring(string))
end
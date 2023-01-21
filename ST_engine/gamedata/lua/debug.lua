-- This file is part of the "ST" project.
-- You may use, distribute or modify this code under the terms
-- of the GNU General Public License version 2.
-- See LICENCE.txt in the root directory of the project.
--
-- Author: Maxim Atanasov
-- E-mail: maxim.atanasov@protonmail.com

--log types
ERROR = 0x01
SUCCESS = 0x02
INFO = 0x04

function log(type, string)
    logLua(type, tostring(string))
end

function clear()
    consoleClear()
end